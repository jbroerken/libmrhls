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

// External

// Project
#include "./MRH_LocalStreamErrorInternal.h"


//*************************************************************************************
// Error Data
//*************************************************************************************

// Last error
static MRH_Local_Stream_Error_Type e_LastError = MRH_LOCAL_STREAM_ERROR_NONE;

//*************************************************************************************
// Reset
//*************************************************************************************

void MRH_ERR_LocalStreamReset(void)
{
    e_LastError = MRH_LOCAL_STREAM_ERROR_NONE;
}

//*************************************************************************************
// Getters
//*************************************************************************************

MRH_Local_Stream_Error_Type MRH_ERR_GetLocalStreamError(void)
{
    return e_LastError;
}

const char* MRH_ERR_GetLocalStreamErrorString(void)
{
    switch (e_LastError)
    {
        // None
        case MRH_LOCAL_STREAM_ERROR_NONE:
            return "No error";
            
        // Unknown
        case MRH_LOCAL_STREAM_ERROR_UNK:
            return "Unknown error";
            
        // General
        case MRH_LOCAL_STREAM_ERROR_GENERAL_RESULT_FAIL:
            return "Event result fail";
        case MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM:
            return "Invalid parameters";
        case MRH_LOCAL_STREAM_ERROR_GENERAL_MALLOC:
            return "Memory allocation failed";
            
        // Socket
        case MRH_LOCAL_STREAM_ERROR_SOCKET_CREATE:
            return "Failed to create socket";
        case MRH_LOCAL_STREAM_ERROR_SOCKET_CONNECT:
            return "Failed to connect socket";
        case MRH_LOCAL_STREAM_ERROR_SOCKET_READ:
            return "Failed to read socket";
        case MRH_LOCAL_STREAM_ERROR_SOCKET_WRITE:
            return "Failed to write socket";
        case MRH_LOCAL_STREAM_ERROR_SOCKET_NO_DATA:
            return "Socket has no data available";
        case MRH_LOCAL_STREAM_ERROR_SOCKET_POLL:
            return "Failed to poll socket";
            
        // Message
        case MRH_LOCAL_STREAM_ERROR_MESSAGE_NO_DATA:
            return "No data for message";
        case MRH_LOCAL_STREAM_ERROR_MESSAGE_AVAILABLE:
            return "A read message is already available";
            
        default:
            return NULL;
    }
}

//*************************************************************************************
// Setters
//*************************************************************************************

void MRH_ERR_SetLocalStreamError(MRH_Local_Stream_Error_Type e_Error)
{
    e_LastError = e_Error;
}
