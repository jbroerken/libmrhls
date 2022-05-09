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

#ifndef MRH_LocalStreamInternal_h
#define MRH_LocalStreamInternal_h

// C

// External

// Project
#include "../../include/libmrhls/libmrhls/MRH_LocalStream.h"

// Pre-defined
#define LOCAL_STREAM_FILE_PATH_SIZE 1024
#define STREAM_MESSAGE_SEND_BUFFER_SIZE 2048


#ifdef __cplusplus
extern "C"
{
#endif
    
    //*************************************************************************************
    // Message
    //*************************************************************************************
    
    typedef struct MRH_CurrentStreamMessage_t
    {
        MRH_Uint8 p_Buffer[STREAM_MESSAGE_SEND_BUFFER_SIZE];
        MRH_Uint32 u32_Handled;
        
    }MRH_CurrentStreamMessage;
    
    //*************************************************************************************
    // Stream
    //*************************************************************************************
    
    struct MRH_LocalStream_t
    {
        int i_ConnectionFD;
        int i_MessageFD;
        
        char p_FilePath[LOCAL_STREAM_FILE_PATH_SIZE];
        int i_IsServer;
        
        MRH_CurrentStreamMessage c_Send;
        MRH_CurrentStreamMessage c_Receive;
    };
    
#ifdef __cplusplus
}
#endif


#endif /* MRH_LocalStreamInternal_h */
