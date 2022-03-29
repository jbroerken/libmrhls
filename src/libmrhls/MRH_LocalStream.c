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
// Message
//*************************************************************************************

static inline void MRH_ResetMessage(MRH_CurrentStreamMessage* p_Message)
{
    // @NOTE: Only reset size and type
    memset(p_Message->p_Buffer, '\0', sizeof(MRH_Uint32) * 2);
    p_Message->u32_Handled = 0;
}

static inline MRH_Uint32 MRH_GetTotalSize(MRH_CurrentStreamMessage* p_Message)
{
    MRH_Uint32 u32_TotalSize = *((MRH_Uint32*)(p_Message->p_Buffer));
    
    if (u32_TotalSize > STREAM_MESSAGE_SEND_BUFFER_SIZE)
    {
        u32_TotalSize = STREAM_MESSAGE_SEND_BUFFER_SIZE;
    }
    
    return u32_TotalSize;
}

//*************************************************************************************
// Open
//*************************************************************************************

static MRH_LocalStream* MRH_CreateStreamObject(const char* p_FilePath, int i_IsServer)
{
    MRH_LocalStream* p_Stream = (MRH_LocalStream*)malloc(sizeof(MRH_LocalStream));
    
    if (p_Stream == NULL)
    {
        return NULL;
    }
    
    MRH_ResetMessage(&(p_Stream->c_Send));
    MRH_ResetMessage(&(p_Stream->c_Recieve));
    
    p_Stream->i_ConnectionFD = -1;
    p_Stream->i_MessageFD = -1;
    p_Stream->i_IsServer = i_IsServer;
    
    memset(p_Stream->p_FilePath, '\0', LOCAL_STREAM_FILE_PATH_SIZE);
    strncpy(p_Stream->p_FilePath, p_FilePath, LOCAL_STREAM_FILE_PATH_SIZE);
    
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
    MRH_ResetMessage(&(p_Stream->c_Send));
    MRH_ResetMessage(&(p_Stream->c_Recieve));
    
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
    MRH_ResetMessage(&(p_Stream->c_Send));
    MRH_ResetMessage(&(p_Stream->c_Recieve));
    
    p_Stream->i_MessageFD = i_FD;
    
    return 0;
}

int MRH_LS_Connect(MRH_LocalStream* p_Stream)
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

static inline MRH_Uint32 MRH_GetReadSize(MRH_CurrentStreamMessage* p_Message)
{
    if (p_Message->u32_Handled < sizeof(MRH_Uint32))
    {
        return sizeof(MRH_Uint32);
    }
    
    return MRH_GetTotalSize(p_Message);
}

int MRH_LS_Read(MRH_LocalStream* p_Stream, int i_TimeoutMS, MRH_Uint8* p_Buffer, MRH_Uint32* p_Size)
{
    if (p_Stream == NULL || p_Stream->i_MessageFD < 0)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
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
            return 1; // Nothing yet, retry
            
        default:
            break;
    }
    
    // Now read
    MRH_CurrentStreamMessage* p_Recieve = &(p_Stream->c_Recieve);
    ssize_t ss_Read;
    MRH_Uint32 u32_ReadSize;
    
    do
    {
        // We need the current read size first
        u32_ReadSize = MRH_GetReadSize(p_Recieve);
        
        // All read?
        if (u32_ReadSize == p_Stream->c_Recieve.u32_Handled)
        {
            // Correct to remove size value
            u32_ReadSize -= sizeof(MRH_Uint32);
            
            // Copy read info
            *p_Size = u32_ReadSize;
            memcpy(p_Buffer,
                   &(p_Recieve->p_Buffer[sizeof(MRH_Uint32)]),
                   u32_ReadSize);
            
            // Reset handled
            MRH_ResetMessage(&(p_Stream->c_Recieve));
            
            // Finished
            return 0;
        }
        
        // We need more data, read
        ss_Read = read(p_Stream->i_MessageFD, 
                       &(p_Recieve->p_Buffer[p_Recieve->u32_Handled]), 
                       u32_ReadSize - p_Recieve->u32_Handled);
        
        // Failed?
        if (ss_Read < 0)
        {
            if (errno != EWOULDBLOCK && errno != EAGAIN)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_SOCKET_READ);
                return -1;
            }
        }
        else
        {
            // Add newly read bytes
            p_Recieve->u32_Handled += ss_Read;
        }
    }
    while (ss_Read > 0);
    
    // Unfinished
    return 1;
}

//*************************************************************************************
// Write
//*************************************************************************************

int MRH_LS_Write(MRH_LocalStream* p_Stream, const MRH_Uint8* p_Buffer, MRH_Uint32 u32_Size)
{
    if (p_Stream == NULL || p_Stream->i_MessageFD < 0)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    // Replace with given buffer?
    MRH_CurrentStreamMessage* p_Send = &(p_Stream->c_Send);
    
    if (MRH_LS_GetWriteMessageSet(p_Stream) < 0)
    {
        // Valid source given?
        if (p_Buffer == NULL || u32_Size > STREAM_MESSAGE_SEND_BUFFER_SIZE - (sizeof(MRH_Uint32) * 2))
        {
            MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
            return -1;
        }
        else
        {
            p_Send->u32_Handled = u32_Size + sizeof(MRH_Uint32);
            
            memcpy(p_Send->p_Buffer, &u32_Size, sizeof(MRH_Uint32));
            memcpy(&(p_Send->p_Buffer[sizeof(MRH_Uint32)]), p_Buffer, u32_Size);
        }
    }
    
    // Set, now write
    ssize_t ss_Write;
    MRH_Uint32 u32_WriteSize;
    
    do
    {
        // First get remaining write size
        u32_WriteSize = MRH_GetTotalSize(p_Send) - p_Send->u32_Handled;
        
        // Check total written
        if (u32_WriteSize == 0)
        {
            // Reset handled
            MRH_ResetMessage(&(p_Stream->c_Recieve));
            
            // Finished
            return 0;
        }
        
        ss_Write = write(p_Stream->i_MessageFD, 
                         &(p_Send->p_Buffer[p_Send->u32_Handled]), 
                         u32_WriteSize);
        
        // Failed to write?
        if (ss_Write < 0)
        {
            if (errno != EWOULDBLOCK && errno != EAGAIN)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_SOCKET_WRITE);
                return -1;
            }
        }
        else
        {
            // Add bytes written
            p_Send->u32_Handled += ss_Write;
        }
    }
    while (ss_Write > 0);
    
    // Not finished
    return 1;
}

int MRH_LS_WriteContinue(MRH_LocalStream* p_Stream)
{
    return MRH_LS_Write(p_Stream, NULL, 0);
}

//*************************************************************************************
// Disconnect
//*************************************************************************************

void MRH_LS_Disconnect(MRH_LocalStream* p_Stream)
{
    if (p_Stream == NULL)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return;
    }
    
    close(p_Stream->i_MessageFD);
    p_Stream->i_MessageFD = -1;
    
    // @NOTE: No reset, only on connect!
}

//*************************************************************************************
// Close
//*************************************************************************************

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
    
    free(p_Stream);
    
    return NULL;
}

//*************************************************************************************
// Getters
//*************************************************************************************

int MRH_LS_GetConnected(MRH_LocalStream* p_Stream)
{
    if (p_Stream == NULL || p_Stream->i_MessageFD < 0)
    {
        return -1;
    }
    
    return 0;
}

int MRH_LS_GetWriteMessageSet(MRH_LocalStream* p_Stream)
{
    if (p_Stream == NULL)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    else if (p_Stream->c_Send.u32_Handled == MRH_GetTotalSize(&(p_Stream->c_Send)))
    {
        return -1;
    }
    
    return 0;
}
