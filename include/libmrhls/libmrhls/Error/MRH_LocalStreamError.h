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

#ifndef MRH_LocalStreamError_h
#define MRH_LocalStreamError_h

// C
#include <stddef.h>

// External

// Project


#ifdef __cplusplus
extern "C"
{
#endif

    //*************************************************************************************
    // Types
    //*************************************************************************************

    typedef enum
    {
        // None
        MRH_LOCAL_STREAM_ERROR_NONE = -1,
        
        // Unknown
        MRH_LOCAL_STREAM_ERROR_UNK = 0,
        
        // General
        MRH_LOCAL_STREAM_ERROR_GENERAL_RESULT_FAIL = 1,
        MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM = 2,
        MRH_LOCAL_STREAM_ERROR_GENERAL_MALLOC = 3,
        
        // Socket
        MRH_LOCAL_STREAM_ERROR_SOCKET_CREATE = 4,
        MRH_LOCAL_STREAM_ERROR_SOCKET_CONNECT = 5,
        MRH_LOCAL_STREAM_ERROR_SOCKET_READ = 6,
        MRH_LOCAL_STREAM_ERROR_SOCKET_WRITE = 7,
        MRH_LOCAL_STREAM_ERROR_SOCKET_NO_DATA,
        MRH_LOCAL_STREAM_ERROR_SOCKET_POLL,
        
        // Message
        MRH_LOCAL_STREAM_ERROR_MESSAGE_NO_DATA,
        MRH_LOCAL_STREAM_ERROR_MESSAGE_AVAILABLE,
        
        // Bounds
        MRH_LOCAL_STREAM_ERROR_TYPE_MAX = MRH_LOCAL_STREAM_ERROR_MESSAGE_AVAILABLE,

        MRH_LOCAL_STREAM_ERROR_TYPE_COUNT = MRH_LOCAL_STREAM_ERROR_TYPE_MAX + 1

    }MRH_Local_Stream_Error_Type;

    //*************************************************************************************
    // Reset
    //*************************************************************************************

    /**
     *  Reset last error.
     */

    extern void MRH_ERR_LocalStreamReset(void);

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get library error.
     *
     *  \return The current library error.
     */

     extern MRH_Local_Stream_Error_Type MRH_ERR_GetLocalStreamError(void);

     /**
      *  Get library error string.
      *
      *  \return The current library error string.
      */

     extern const char* MRH_ERR_GetLocalStreamErrorString(void);

#ifdef __cplusplus
}
#endif

#endif /* MRH_LocalStreamError_h */
