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

#ifndef MRH_StreamMessage_h
#define MRH_StreamMessage_h

// C

// External
#include <MRH_Typedefs.h>

// Project

// Pre-defined
#define MRH_STREAM_MESSAGE_VERSION 1


#ifdef __cplusplus
extern "C"
{
#endif
    
    //*************************************************************************************
    // Message List
    //*************************************************************************************
    
    typedef enum
    {
        /**
         *  Unk
         */
        
        MRH_LSM_UNK = 0,
        
        /**
         *  Version
         */
        
        MRH_LSM_VERSION = 1,
        
        /**
         *  Custom
         */
        
        MRH_LSM_CUSTOM = 2,
        
        /**
         *  String
         */
        
        MRH_LSM_STRING = 3,
        
        /**
         *  Audio
         */
        
        MRH_LSM_AUDIO = 4,
        MRH_LSM_AUDIO_PLAYBACK_FINISHED = 5,
        MRH_LSM_AUDIO_START_RECORDING = 6,
        MRH_LSM_AUDIO_STOP_RECORDING = 7,
        
        /**
         *  Location
         */
        
        MRH_LSM_LOCATION,
        
        /**
         *  Bounds
         */
        
        MRH_STREAM_MESSAGE_MAX = MRH_LSM_LOCATION,
        
        MRH_STREAM_MESSAGE_COUNT = MRH_STREAM_MESSAGE_MAX + 1
        
    }MRH_StreamMessage;
    
    //*************************************************************************************
    // Version
    //*************************************************************************************
    
    typedef struct MRH_LSM_Version_Data_t
    {
        MRH_Uint32 u32_Version;
        
    }MRH_LSM_Version_Data;
    
    //*************************************************************************************
    // Custom
    //*************************************************************************************
    
    typedef struct MRH_LSM_Custom_Data_t
    {
        MRH_Uint32 u32_Size;
        MRH_Uint8* p_Buffer;
        
    }MRH_LSM_Custom_Data;
    
    //*************************************************************************************
    // String
    //*************************************************************************************
    
    typedef struct MRH_LSM_String_Data_t
    {
        MRH_Uint32 u32_Size; // Bytes
        char* p_String; // UTF-8
        
    }MRH_LSM_String_Data;
    
    //*************************************************************************************
    // Audio
    //*************************************************************************************
    
    typedef struct MRH_LSM_Audio_Data_t
    {
        MRH_Uint8 u8_Channels;
        MRH_Uint32 u32_KHz;
        MRH_Uint32 u32_Samples;
        MRH_Sint16* p_Samples; // PCM-16
        
    }MRH_LSM_Audio_Data;
    
    // @NOTE: No data for MRH_LSM_AUDIO_PLAYBACK_FINISHED
    
    // @NOTE: No data for MRH_LSM_AUDIO_START_RECORDING
    
    // @NOTE: No data for MRH_LSM_AUDIO_STOP_RECORDING
    
    //*************************************************************************************
    // Location
    //*************************************************************************************
    
    typedef struct MRH_LSM_Location_Data_t
    {
        MRH_Sfloat64 f64_Latitude;
        MRH_Sfloat64 f64_Longtitude;
        MRH_Sfloat64 f64_Elevation;
        MRH_Sfloat64 f64_Facing;
        
    }MRH_LSM_Location_Data;
    
#ifdef __cplusplus
}
#endif


#endif /* MRH_StreamMessage_h */
