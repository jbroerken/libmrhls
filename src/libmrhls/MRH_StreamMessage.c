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
#include <string.h>
#include <stdio.h>

// External

// Project
#include "../../include/libmrhls/libmrhls/MRH_StreamMessage.h"
#include "./Error/MRH_LocalStreamErrorInternal.h"


//*************************************************************************************
// Buffer
//*************************************************************************************

int MRH_LS_MessageToBuffer(MRH_Uint8* p_Buffer, MRH_Uint32* p_Size, MRH_LS_Message e_Message, const void* p_Data)
{
    if (p_Buffer == NULL || p_Size == NULL || e_Message > MRH_LS_MESSAGE_MAX || p_Data == NULL)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    MRH_Uint32 u32_Message = (MRH_Uint32)e_Message;
    MRH_Uint32 u32_TotalSize = sizeof(MRH_Uint32);
    
    memcpy(p_Buffer, &u32_Message, sizeof(u32_TotalSize));
    p_Buffer += u32_TotalSize;
    
    switch (e_Message)
    {
        case MRH_LS_M_VERSION:
        {
            const MRH_LS_M_Version_Data* p_Cast = (const MRH_LS_M_Version_Data*)p_Data;
            
            memcpy(p_Buffer, &(p_Cast->u32_Version), sizeof(MRH_Uint32));
            
            u32_TotalSize += sizeof(MRH_Uint32);
            break;
        }
        case MRH_LS_M_CUSTOM:
        {
            const MRH_LS_M_Custom_Data* p_Cast = (const MRH_LS_M_Custom_Data*)p_Data;
            
            // Custom buffer size issues
            if (p_Cast->u32_Size > MRH_STREAM_MESSAGE_CUSTOM_BUFFER_SIZE)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            else if (p_Cast->u32_Size > 0)
            {
                memcpy(p_Buffer, p_Cast->p_Buffer, p_Cast->u32_Size);
            }
            
            u32_TotalSize += p_Cast->u32_Size;
            break;
        }
        case MRH_LS_M_STRING:
        {
            const MRH_LS_M_String_Data* p_Cast = (const MRH_LS_M_String_Data*)p_Data;
            MRH_Uint32 u32_Length = strnlen(p_Cast->p_String, MRH_STREAM_MESSAGE_BUFFER_SIZE);
            
            if (u32_Length > 0)
            {
                memcpy(p_Buffer, p_Cast->p_String, u32_Length);
            }
            
            memcpy(p_Buffer, p_Cast->p_String, u32_Length);
            
            u32_TotalSize += u32_Length;
            break;
        }
        case MRH_LS_M_AUDIO:
        {
            const MRH_LS_M_Audio_Data* p_Cast = (const MRH_LS_M_Audio_Data*)p_Data;
            MRH_Uint32 u32_SampleSize = p_Cast->u32_Samples * sizeof(MRH_Sint16);
            
            memcpy(p_Buffer, &(p_Cast->u32_KHz), sizeof(MRH_Uint32));
            
            // Samples buffer size issues
            if (p_Cast->u32_Samples > MRH_STREAM_MESSAGE_AUDIO_BUFFER_SIZE)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            else if (p_Cast->u32_Samples > 0)
            {
                memcpy(&(p_Buffer[sizeof(MRH_Uint32)]), p_Cast->p_Samples, u32_SampleSize);
            }
            
            u32_TotalSize += (sizeof(MRH_Uint32) + u32_SampleSize);
            break;
        }
        case MRH_LS_M_LOCATION:
        {
            const MRH_LS_M_Location_Data* p_Cast = (const MRH_LS_M_Location_Data*)p_Data;
            
            memcpy(p_Buffer, &(p_Cast->f64_Latitude), sizeof(MRH_Sfloat64));
            memcpy(&(p_Buffer[sizeof(MRH_Sfloat64)]), &(p_Cast->f64_Longtitude), sizeof(MRH_Sfloat64));
            memcpy(&(p_Buffer[sizeof(MRH_Sfloat64) * 2]), &(p_Cast->f64_Elevation), sizeof(MRH_Sfloat64));
            memcpy(&(p_Buffer[sizeof(MRH_Sfloat64) * 3]), &(p_Cast->f64_Facing), sizeof(MRH_Sfloat64));
            
            u32_TotalSize += (sizeof(MRH_Sfloat64) * 4);
            break;
        }
            
        default:
        {
            MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
            return -1;
        }
    }
    
    *p_Size = u32_TotalSize;
    
    return 0;
}

MRH_LS_Message MRH_LS_GetBufferMessage(const MRH_Uint8* p_Buffer)
{
    if (p_Buffer == NULL)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return MRH_LS_M_UNK;
    }
    
    return (MRH_LS_Message)*((MRH_Uint32*)p_Buffer);
}

int MRH_LS_BufferToMessage(void* p_Data, const MRH_Uint8* p_Buffer, MRH_Uint32 u32_Size)
{
    if (p_Buffer == NULL || p_Data == NULL || u32_Size > MRH_STREAM_MESSAGE_BUFFER_SIZE)
    {
        MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
        return -1;
    }
    
    MRH_LS_Message e_Message = MRH_LS_GetBufferMessage(p_Buffer);
    p_Buffer += sizeof(MRH_Uint32);
    u32_Size -= sizeof(MRH_Uint32);
    
    switch (e_Message)
    {
        case MRH_LS_M_VERSION:
        {
            if (u32_Size < sizeof(MRH_Uint32))
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            
            MRH_LS_M_Version_Data* p_Cast = (MRH_LS_M_Version_Data*)p_Data;
            
            memcpy(&(p_Cast->u32_Version), p_Buffer, sizeof(MRH_Uint32));
            break;
        }
        case MRH_LS_M_CUSTOM:
        {
            if (u32_Size == 0)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            
            MRH_LS_M_Custom_Data* p_Cast = (MRH_LS_M_Custom_Data*)p_Data;
            
            if (u32_Size > MRH_STREAM_MESSAGE_CUSTOM_BUFFER_SIZE)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            else if (u32_Size > 0)
            {
                memcpy(p_Cast->p_Buffer, p_Buffer, p_Cast->u32_Size);
            }
            
            p_Cast->u32_Size = u32_Size;
            break;
        }
        case MRH_LS_M_STRING:
        {
            if (u32_Size == 0)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            
            MRH_LS_M_String_Data* p_Cast = (MRH_LS_M_String_Data*)p_Data;
            
            memset(p_Cast->p_String, '\0', MRH_STREAM_MESSAGE_BUFFER_SIZE);
            
            if (u32_Size > 0)
            {
                memcpy(p_Cast->p_String, p_Buffer, u32_Size);
            }
            break;
        }
        case MRH_LS_M_AUDIO:
        {
            if (u32_Size < sizeof(MRH_Uint32))
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            
            MRH_LS_M_Audio_Data* p_Cast = (MRH_LS_M_Audio_Data*)p_Data;
            
            memcpy(&(p_Cast->u32_KHz), p_Buffer, sizeof(MRH_Uint32));
            u32_Size -= sizeof(MRH_Uint32);
            
            p_Cast->u32_Samples = u32_Size / sizeof(MRH_Sint16);
            
            if (p_Cast->u32_Samples > MRH_STREAM_MESSAGE_AUDIO_BUFFER_SIZE)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            else if (p_Cast->u32_Samples > 0)
            {
                memcpy(p_Cast->p_Samples, &(p_Buffer[sizeof(MRH_Uint32)]), u32_Size);
            }
            break;
        }
        case MRH_LS_M_LOCATION:
        {
            if (u32_Size < sizeof(MRH_Sfloat64) * 4)
            {
                MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
                return -1;
            }
            
            MRH_LS_M_Location_Data* p_Cast = (MRH_LS_M_Location_Data*)p_Data;
            
            memcpy(&(p_Cast->f64_Latitude), p_Buffer, sizeof(MRH_Sfloat64));
            memcpy(&(p_Cast->f64_Longtitude), &(p_Buffer[sizeof(MRH_Sfloat64)]), sizeof(MRH_Sfloat64));
            memcpy(&(p_Cast->f64_Elevation), &(p_Buffer[sizeof(MRH_Sfloat64) * 2]), sizeof(MRH_Sfloat64));
            memcpy(&(p_Cast->f64_Facing), &(p_Buffer[sizeof(MRH_Sfloat64) * 3]), sizeof(MRH_Sfloat64));
            break;
        }
            
        default:
        {
            MRH_ERR_SetLocalStreamError(MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM);
            return -1;
        }
    }
    
    return 0;
}
