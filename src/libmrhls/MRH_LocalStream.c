/**
 *  libmrhls
 *  Copyright (C) 2021 - 2022 Jens Brörken
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would be
 *     appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such, and must not be
 *     misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source distribution.
 */

// C
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>

// External

// Project
#include "./MRH_LocalStreamInternal.h"
#include "./Error/MRH_LocalStreamErrorInternal.h"


//*************************************************************************************
// Open
//*************************************************************************************

static MRH_CurrentStreamMessage* MRH_CreateStreamMessage(void)
{
    MRH_CurrentStreamMessage* p_Message = (MRH_CurrentStreamMessage*)malloc(sizeof(MRH_CurrentStreamMessage));
    
    if (p_Message == NULL)
    {
        return NULL;
    }
    
    p_Message->u32_SizeBuffer = sizeof(MRH_Uint32) * 2;
    p_Message->p_Buffer = (MRH_Uint8*)malloc(p_Message->u32_SizeBuffer);
    
    if (p_Message->p_Buffer == NULL)
    {
        free(p_Message);
        return NULL;
    }
    
    p_Message->u32_SizeTotal = 0;
    p_Message->u32_SizeHandled = 0;
    
    return p_Message;
}

static MRH_LocalStream* MRH_CreateStreamObject(const char* p_FilePath, int i_IsServer)
{
    MRH_LocalStream* p_Stream = (MRH_LocalStream*)malloc(sizeof(MRH_LocalStream));
    
    if (p_Stream == NULL)
    {
        return NULL;
    }
    else if ((p_Stream->p_Recieve = MRH_CreateStreamMessage()) == NULL)
    {
        free(p_Stream);
        return NULL;
    }
    else if ((p_Stream->p_Send = MRH_CreateStreamMessage()) == NULL)
    {
        free(p_Stream->p_Recieve);
        free(p_Stream);
        return NULL;
    }
    
    p_Stream->i_ConnectionFD = -1;
    p_Stream->i_MessageFD = -1;
    p_Stream->i_IsServer = i_IsServer;
    
    memset(p_Stream->p_FilePath, '\0', 2048);
    strncpy(p_Stream->p_FilePath, p_FilePath, 2048);
    
    return p_Stream;
}

MRH_LocalStream* MRH_LS_Open(const char* p_FilePath, int i_Create)
{
    MRH_LocalStream* p_Stream = MRH_CreateStreamObject(p_FilePath, i_Create < 0 ? -1 : 0);
    
    if (p_Stream == NULL)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_MALLOC);
        return NULL;
    }
    else if (i_Create < 0)
    {
        // No socket creation, return early
        return p_Stream;
    }
    
    // @NOTE: No error check, might be closed correctly last time
    unlink(p_FilePath);
    
    // Create a file descriptor first
    int i_FD;
    
    if ((i_FD = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_SOCKET_CREATE);
        return MRH_LS_Close(p_Stream);
    }
    
    // Setup socket for connections
    struct sockaddr_un c_Address;
    memset(&c_Address, '\0', sizeof(c_Address));
    c_Address.sun_family = AF_UNIX;
    strcpy(c_Address.sun_path, p_FilePath);
    
    if (bind(i_FD, (struct sockaddr*)&c_Address, sizeof(c_Address)) < 0 ||
        fcntl(i_FD, F_SETFL, fcntl(i_FD, F_GETFL, 0) | O_NONBLOCK) < 0 ||
        listen(i_FD, 1) < 0) // 1 = Connection backlog, only 1 is allowed
    {
        close(i_FD);
        
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_SOCKET_CREATE);
        return MRH_LS_Close(p_Stream);
    }
    
    p_Stream->i_ConnectionFD = i_FD;
    
    return p_Stream;
}

//*************************************************************************************
// Connect
//*************************************************************************************

static void MRH_ResetOnConnection(MRH_LocalStream* p_Stream)
{
    // For sending we simply reset currently handled to send from
    // the beginning
    p_Stream->p_Send->u32_SizeHandled = 0;
    
    // Recieving needs to have both total and handled reset
    p_Stream->p_Recieve->u32_SizeTotal = 0;
    p_Stream->p_Recieve->u32_SizeHandled = 0;
}

static int MRH_AcceptStreamConnection(MRH_LocalStream* p_Stream)
{
    int i_FD;
    struct sockaddr_un c_Adress;
    socklen_t us_ClientLen;
    
    if ((i_FD = accept(p_Stream->i_ConnectionFD, (struct sockaddr*)&c_Adress, &us_ClientLen)) < 0 || 
        fcntl(i_FD, F_SETFL, fcntl(i_FD, F_GETFL, 0) | O_NONBLOCK) < 0)
    {
        close(i_FD);
        
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_SOCKET_CONNECT);
        return -1;
    }
    
    // Reset current send state to send and recieve from
    // beginning (for currently set messages)
    MRH_ResetOnConnection(p_Stream);
    
    p_Stream->i_MessageFD = i_FD;
    
    return 0;
}

static int MRH_ConnectToStream(MRH_LocalStream* p_Stream)
{
    int i_FD;
    struct sockaddr_un c_Address;
    socklen_t us_AddressLength;
    
    if ((i_FD = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_SOCKET_CREATE);
        return -1;
    }
    
    us_AddressLength = sizeof(c_Address);
    memset(&c_Address, 0, us_AddressLength);
    c_Address.sun_family = AF_UNIX;
    strcpy(c_Address.sun_path, p_Stream->p_FilePath);
    
    if (connect(i_FD, (struct sockaddr*)&c_Address, us_AddressLength) < 0 ||
        fcntl(i_FD, F_SETFL, fcntl(i_FD, F_GETFL, 0) | O_NONBLOCK) < 0)
    {
        close(i_FD);
        
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_SOCKET_CONNECT);
        return -1;
    }
    
    // Reset current send state to send and recieve from
    // beginning (for currently set messages)
    MRH_ResetOnConnection(p_Stream);
    
    p_Stream->i_MessageFD = i_FD;
    
    return 0;
}

static inline int MRH_ConnectLocalStream(MRH_LocalStream* p_Stream)
{
    if (p_Stream->i_IsServer < 0)
    {
        return MRH_ConnectToStream(p_Stream);
    }
    else
    {
        return MRH_AcceptStreamConnection(p_Stream);
    }
}

//*************************************************************************************
// Read
//*************************************************************************************

static inline int MRH_UpdateReadSize(MRH_CurrentStreamMessage* p_Message)
{
    if (p_Message->u32_SizeTotal < sizeof(MRH_Uint32))
    {
        p_Message->u32_SizeTotal = sizeof(MRH_Uint32);
        return 0;
    }
    else if (p_Message->u32_SizeTotal < (*(MRH_Uint32*)(p_Message->p_Buffer)))
    {
        p_Message->u32_SizeTotal = (*(MRH_Uint32*)(p_Message->p_Buffer));
        return 0;
    }
    
    return -1;
}

int MRH_LS_Read(MRH_LocalStream* p_Stream, int i_TimeoutMS)
{
    if (p_Stream == NULL)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    else if (p_Stream->p_Recieve->u32_SizeHandled > 0 && 
             (p_Stream->p_Recieve->u32_SizeHandled == p_Stream->p_Recieve->u32_SizeTotal))
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_MESSAGE_AVAILABLE);
        return -1;
    }
    else if (p_Stream->i_MessageFD < 0 && MRH_ConnectLocalStream(p_Stream) < 0)
    {
        return -1;
    }
    
    // Poll for data first
    struct pollfd s_PollFD;
    s_PollFD.fd = p_Stream->i_MessageFD;
    s_PollFD.events = POLLIN;
    
    switch (poll(&s_PollFD, (nfds_t)1, i_TimeoutMS))
    {
        case -1:
            MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_SOCKET_POLL);
            return -1;
        case 0:
            MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_SOCKET_NO_DATA);
            return 0;
            
        default:
            break;
    }
    
    // Now read
    MRH_CurrentStreamMessage* p_Recieve = p_Stream->p_Recieve;
    ssize_t ss_Read;
    
    MRH_UpdateReadSize(p_Recieve);
    
    size_t us_Remaining = p_Recieve->u32_SizeTotal - p_Recieve->u32_SizeHandled;
    
    do
    {
        ss_Read = read(p_Stream->i_MessageFD, 
                       &(p_Recieve->p_Buffer[p_Recieve->u32_SizeHandled]), 
                       us_Remaining);
        
        if (ss_Read < 0)
        {
            if (errno != EWOULDBLOCK && errno != EAGAIN)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_SOCKET_READ);
                return -1;
            }
        }
        else if (us_Remaining == 0)
        {
            if (MRH_UpdateReadSize(p_Recieve) < 0)
            {
                break;
            }
            
            // Realloc to to match buffer
            if (p_Recieve->u32_SizeTotal > p_Recieve->u32_SizeBuffer)
            {
                MRH_Uint8* p_Buffer = (MRH_Uint8*)realloc(p_Recieve->p_Buffer, p_Recieve->u32_SizeTotal);
            
                if (p_Buffer == NULL)
                {
                    MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_MALLOC);
                    return -1;
                }
                
                p_Recieve->p_Buffer = p_Buffer;
                p_Recieve->u32_SizeBuffer = p_Recieve->u32_SizeTotal;
            }
            
            us_Remaining = p_Recieve->u32_SizeTotal - p_Recieve->u32_SizeHandled;
        }
        else
        {
            us_Remaining -= ss_Read;
            p_Recieve->u32_SizeHandled += ss_Read;
        }
    }
    while (ss_Read > 0);
    
    if (us_Remaining == 0)
    {
        // Finished
        return 0;
    }
    
    // Unfinished
    return 1;
}

//*************************************************************************************
// Write
//*************************************************************************************

int MRH_LS_Write(MRH_LocalStream* p_Stream)
{
    if (p_Stream == NULL)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    else if (p_Stream->i_MessageFD < 0 && MRH_ConnectLocalStream(p_Stream) < 0)
    {
        return -1;
    }
    
    MRH_CurrentStreamMessage* p_Send = p_Stream->p_Send;
    ssize_t ss_Write;
    size_t us_Remaining = p_Send->u32_SizeTotal - p_Send->u32_SizeHandled;
    
    do
    {
        ss_Write = write(p_Stream->i_MessageFD, 
                         &(p_Send->p_Buffer[p_Send->u32_SizeHandled]), 
                         us_Remaining);
        
        if (ss_Write < 0)
        {
            if (errno != EWOULDBLOCK && errno != EAGAIN)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_SOCKET_WRITE);
                return -1;
            }
        }
        
        p_Send->u32_SizeHandled += ss_Write;
        us_Remaining -= ss_Write;
    }
    while (ss_Write > 0 && us_Remaining > 0);
    
    if (us_Remaining == 0)
    {
        // Finished
        return 0;
    }
    
    // Not finished
    return 1;
}

//*************************************************************************************
// Close
//*************************************************************************************

static void CloseCurrentMessage(MRH_CurrentStreamMessage* p_Message)
{
    free(p_Message->p_Buffer);
    free(p_Message);
}

MRH_LocalStream* MRH_LS_Close(MRH_LocalStream* p_Stream)
{
    if (p_Stream == NULL)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return NULL;
    }
    
    close(p_Stream->i_ConnectionFD);
    close(p_Stream->i_MessageFD);
    
    unlink(p_Stream->p_FilePath);
    
    CloseCurrentMessage(p_Stream->p_Recieve);
    CloseCurrentMessage(p_Stream->p_Send);
    
    free(p_Stream);
    
    return NULL;
}

//*************************************************************************************
// Getters
//*************************************************************************************

static inline MRH_StreamMessage MRH_GetStreamMessageType(MRH_CurrentStreamMessage* p_Message)
{
    // @NOTE: Not buffer size, but total read size
    if (p_Message->u32_SizeTotal < (sizeof(MRH_Uint32) * 2))
    {
        return MRH_LSM_UNK;
    }
    
    // Message id is at pos 4, 4 long
    return (MRH_StreamMessage)(*((MRH_Uint32*)&(p_Message->p_Buffer[sizeof(MRH_Uint32)])));
}

static inline void MRH_GetStreamMessageData(MRH_CurrentStreamMessage* p_Message, MRH_Uint8** p_Buffer, MRH_Uint32* p_SizeTotal)
{
    MRH_Uint32 u32_Required = sizeof(MRH_Uint32) * 2;
    
    if (p_Buffer != NULL)
    {
        if (p_Message->u32_SizeTotal > u32_Required)
        {
            *p_Buffer = &(p_Message->p_Buffer[u32_Required]);
        }
        else
        {
            *p_Buffer = NULL;
        }
    }
    
    if (p_SizeTotal != NULL)
    {
        if (p_Message->u32_SizeTotal >= u32_Required)
        {
            *p_SizeTotal = p_Message->u32_SizeTotal - u32_Required;
        }
        else
        {
            *p_SizeTotal = 0;
        }
    }
}

MRH_StreamMessage MRH_LS_GetLastMessage(MRH_LocalStream* p_Stream)
{
    if (p_Stream == NULL)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return MRH_LSM_UNK;
    }
    
    return MRH_GetStreamMessageType(p_Stream->p_Recieve);
}

int MRH_LS_GetLastMessageData(MRH_LocalStream* p_Stream, void* p_Data)
{
    if (p_Stream == NULL || p_Data == NULL)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    // Get the size and buffer of the message data.
    MRH_Uint8* p_Buffer;
    MRH_Uint32 u32_SizeTotal;
    
    MRH_GetStreamMessageData(p_Stream->p_Recieve, &p_Buffer, &u32_SizeTotal);
    
    // Next reset the message, so that reading works even after a 
    // failure
    p_Stream->p_Recieve->u32_SizeTotal = 0;
    p_Stream->p_Recieve->u32_SizeHandled = 0;
    
    // Can get message data?
    if (p_Buffer == NULL)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    switch (MRH_GetStreamMessageType(p_Stream->p_Recieve))
    {
        case MRH_LSM_VERSION:
        {
            ((MRH_LSM_Version_Data*)p_Data)->u32_Version = *((MRH_Uint32*)p_Buffer);
            break;
        }
        case MRH_LSM_CUSTOM:
        {
            MRH_LSM_Custom_Data* p_Custom = (MRH_LSM_Custom_Data*)p_Data;
            
            p_Custom->u32_Size = u32_SizeTotal;
            
            if ((p_Custom->p_Buffer = (MRH_Uint8*)malloc(p_Custom->u32_Size)) == NULL)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_MALLOC);
                return -1;
            }
            
            memcpy(p_Custom->p_Buffer, p_Buffer, u32_SizeTotal);
            break;
        }
        case MRH_LSM_STRING:
        {
            MRH_LSM_String_Data* p_String = (MRH_LSM_String_Data*)p_Data;
            
            p_String->u32_Size = u32_SizeTotal;
            
            if ((p_String->p_String = (char*)malloc(p_String->u32_Size)) == NULL)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_MALLOC);
                return -1;
            }
            
            memcpy(p_String->p_String, p_Buffer, u32_SizeTotal);
            break;
        }
        case MRH_LSM_AUDIO:
        {
            MRH_LSM_Audio_Data* p_Audio = (MRH_LSM_Audio_Data*)p_Data;
            
            p_Audio->u8_Channels = p_Buffer[0];
            p_Audio->u32_KHz = *((MRH_Uint32*)&(p_Buffer[sizeof(MRH_Uint8)]));
            
            MRH_Uint32 u32_Start = sizeof(MRH_Uint8) + sizeof(MRH_Uint32);
            size_t us_SampleSize = u32_SizeTotal - u32_Start;
            
            if ((p_Audio->p_Samples = (MRH_Sint16*)malloc(us_SampleSize)) == NULL)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_MALLOC);
                return -1;
            }
            
            memcpy(p_Audio->p_Samples, &(p_Buffer[u32_Start]), us_SampleSize);
            break;
        }
        case MRH_LSM_LOCATION:
        {
            MRH_LSM_Location_Data* p_Location = (MRH_LSM_Location_Data*)p_Data;
            
            memcpy(&(p_Location->f64_Latitude), p_Buffer, sizeof(MRH_Sfloat64));
            memcpy(&(p_Location->f64_Longtitude), &(p_Buffer[sizeof(MRH_Sfloat64)]), sizeof(MRH_Sfloat64));
            memcpy(&(p_Location->f64_Elevation), &(p_Buffer[sizeof(MRH_Sfloat64) * 2]), sizeof(MRH_Sfloat64));
            memcpy(&(p_Location->f64_Facing), &(p_Buffer[sizeof(MRH_Sfloat64) * 3]), sizeof(MRH_Sfloat64));
            break;
        }
            
        default:
        {
            MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_MESSAGE_NO_DATA);
            return -1;
        }
    }
    
    return 0;
}

//*************************************************************************************
// Setters
//*************************************************************************************

int MRH_LS_SetMessage(MRH_LocalStream* p_Stream, MRH_StreamMessage e_Message, const void* p_Data)
{
    if (p_Stream == NULL || e_Message > MRH_STREAM_MESSAGE_MAX)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    MRH_CurrentStreamMessage* p_Send = p_Stream->p_Send;
    MRH_Uint32 u32_Message = e_Message;
    
    // Get the required size first
    switch (u32_Message)
    {
        case MRH_LSM_VERSION:
        {
            if (p_Data == NULL)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            
            p_Send->u32_SizeTotal = sizeof(MRH_Uint32);
            break;
        }
        case MRH_LSM_CUSTOM:
        {
            if (p_Data != NULL)
            {
                p_Send->u32_SizeTotal = ((const MRH_LSM_Custom_Data*)p_Data)->u32_Size;
            }
            else
            {
                p_Send->u32_SizeTotal = 0;
            }
            break;
        }
        case MRH_LSM_STRING:
        {
            if (p_Data == NULL)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            
            p_Send->u32_SizeTotal = ((const MRH_LSM_String_Data*)p_Data)->u32_Size;
            break;
        }
        case MRH_LSM_AUDIO:
        {
            if (p_Data == NULL)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            
            const MRH_LSM_Audio_Data* p_Audio = (const MRH_LSM_Audio_Data*)p_Data;
            
            p_Send->u32_SizeTotal = sizeof(MRH_Uint8) + sizeof(MRH_Uint32);
            p_Send->u32_SizeTotal += p_Audio->u32_Samples * sizeof(MRH_Sint16);
            break;
        }
        case MRH_LSM_LOCATION:
        {
            if (p_Data == NULL)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            
            p_Send->u32_SizeTotal = sizeof(MRH_Sfloat64) * 4;
            break;
        }
            
        default:
        {
            break;
        }
    }
    
    // Apennd message and size bytes
    p_Send->u32_SizeTotal += (sizeof(MRH_Uint32) * 2);
    
    // Do we need to expand the send buffer?
    if (p_Send->u32_SizeBuffer < p_Send->u32_SizeTotal)
    {
        MRH_Uint8* p_Buffer = (MRH_Uint8*)realloc(p_Send->p_Buffer, p_Send->u32_SizeTotal);
        
        if (p_Buffer == NULL)
        {
            MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_MALLOC);
            return -1;
        }
        
        p_Send->p_Buffer = p_Buffer;
        p_Send->u32_SizeBuffer = p_Send->u32_SizeTotal;
    }
    
    // Copy message info
    memcpy(p_Send->p_Buffer, &(p_Send->u32_SizeTotal), sizeof(MRH_Uint32));
    memcpy(&(p_Send->p_Buffer[sizeof(MRH_Uint32)]), &u32_Message, sizeof(MRH_Uint32));
    
    // Copy message data
    MRH_Uint8* p_Buffer;
    MRH_Uint32 u32_SizeTotal;
    
    MRH_GetStreamMessageData(p_Send, &p_Buffer, &u32_SizeTotal);
    
    switch (u32_Message)
    {
        case MRH_LSM_VERSION:
        {
            memcpy(p_Buffer, &(((MRH_LSM_Version_Data*)p_Data)->u32_Version), u32_SizeTotal);
            break;
        }
        case MRH_LSM_CUSTOM:
        {
            if (p_Data != NULL)
            {
                memcpy(p_Buffer, ((MRH_LSM_Custom_Data*)p_Data)->p_Buffer, u32_SizeTotal);
            }
            break;
        }
        case MRH_LSM_STRING:
        {
            memcpy(p_Buffer, ((MRH_LSM_String_Data*)p_Data)->p_String, u32_SizeTotal);
            break;
        }
        case MRH_LSM_AUDIO:
        {
            const MRH_LSM_Audio_Data* p_Audio = (const MRH_LSM_Audio_Data*)p_Data;
            
            memcpy(p_Buffer, &(p_Audio->u8_Channels), sizeof(MRH_Uint8));
            memcpy(&(p_Buffer[sizeof(MRH_Uint8)]), &(p_Audio->u32_KHz), sizeof(MRH_Uint32));
            memcpy(&(p_Buffer[sizeof(MRH_Uint32)]), p_Audio->p_Samples, p_Audio->u32_Samples * sizeof(MRH_Sint16));
            break;
        }
        case MRH_LSM_LOCATION:
        {
            const MRH_LSM_Location_Data* p_Location = (const MRH_LSM_Location_Data*)p_Data;
            
            memcpy(p_Buffer, &(p_Location->f64_Latitude), sizeof(MRH_Sfloat64));
            memcpy(&(p_Buffer[sizeof(MRH_Sfloat64)]), &(p_Location->f64_Longtitude), sizeof(MRH_Sfloat64));
            memcpy(&(p_Buffer[sizeof(MRH_Sfloat64) * 2]), &(p_Location->f64_Elevation), sizeof(MRH_Sfloat64));
            memcpy(&(p_Buffer[sizeof(MRH_Sfloat64) * 3]), &(p_Location->f64_Facing), sizeof(MRH_Sfloat64));
            break;
        }
            
        default:
        {
            break;
        }
    }
    
    // Setup sending info
    p_Send->u32_SizeHandled = 0;
    
    return 0;
}
