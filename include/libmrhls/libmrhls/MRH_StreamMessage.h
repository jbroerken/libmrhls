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
#define MRH_STREAM_MESSAGE_BUFFER_SIZE 2044 // Exclude total size
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
        
        MRH_LS_M_UNK = 0,
        
        /**
         *  Version
         */
        
        MRH_LS_M_VERSION = 1,
        
        /**
         *  Custom
         */
        
        MRH_LS_M_CUSTOM = 2,
        
        /**
         *  String
         */
        
        MRH_LS_M_STRING = 3,
        
        /**
         *  Audio
         */
        
        MRH_LS_M_AUDIO = 4,
        MRH_LS_M_AUDIO_PLAYBACK_FINISHED = 5,
        MRH_LS_M_AUDIO_START_RECORDING = 6,
        MRH_LS_M_AUDIO_STOP_RECORDING = 7,
        
        /**
         *  Location
         */
        
        MRH_LS_M_LOCATION,
        
        /**
         *  Bounds
         */
        
        MRH_LS_MESSAGE_MAX = MRH_LS_M_LOCATION,
        
        MRH_LS_MESSAGE_COUNT = MRH_LS_MESSAGE_MAX + 1
        
    }MRH_LS_Message;
    
    //*************************************************************************************
    // Version
    //*************************************************************************************
    
    typedef struct MRH_LS_M_Version_Data_t
    {
        MRH_Uint32 u32_Version;
        
    }MRH_LS_M_Version_Data;
    
    //*************************************************************************************
    // Custom
    //*************************************************************************************
    
    typedef struct MRH_LS_M_Custom_Data_t
    {
        MRH_Uint32 u32_Size;
        MRH_Uint8 p_Buffer[MRH_STREAM_MESSAGE_BUFFER_SIZE - sizeof(MRH_Uint32)];
        
    }MRH_LS_M_Custom_Data;
    
    //*************************************************************************************
    // String
    //*************************************************************************************
    
    typedef struct MRH_LS_M_String_Data_t
    {
        char p_String[MRH_STREAM_MESSAGE_BUFFER_SIZE]; // UTF-8
        
    }MRH_LS_M_String_Data;
    
    //*************************************************************************************
    // Audio
    //*************************************************************************************
    
    typedef struct MRH_LS_M_Audio_Data_t
    {
        MRH_Uint32 u32_KHz;
        MRH_Uint32 u32_Samples;
        MRH_Sint16 p_Samples[MRH_STREAM_MESSAGE_BUFFER_SIZE - (sizeof(MRH_Uint32) * 2)]; // Mono
        
    }MRH_LS_M_Audio_Data;
    
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
        
    }MRH_LS_M_Location_Data;
    
    //*************************************************************************************
    // Buffer
    //*************************************************************************************
    
    /**
     *  Fill a buffer with a given message struct.
     *  
     *  \param p_Buffer The buffer to fill. The buffer has to be of size 
     *                  STREAM_MESSAGE_BUFFER_SIZE.
     *  \param p_Size The buffer size of the written buffer.
     *  \param e_Message The message type.
     *  \param p_Data The message data.
     *  
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_LS_MessageToBuffer(MRH_Uint8* p_Buffer, MRH_Uint32* p_Size, MRH_LS_Message e_Message, const void* p_Data);
    
    /**
     *  Get the message type of a buffer.
     *  
     *  \param p_Buffer The buffer to read. Has to be at least of size MRH_Uint32.
     *  
     *  \return The buffer message type.
     */

    extern MRH_LS_Message MRH_LS_GetBufferMessage(const MRH_Uint8* p_Buffer);
    
    /**
     *  Fill a message struct with a given buffer.
     *  
     *  \param p_Data The message data.    
     *  \param p_Buffer The buffer to read. The buffer has to be of size 
     *                  STREAM_MESSAGE_BUFFER_SIZE.
     *  \param u32_Size The buffer size of the given buffer.
     *  
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_LS_BufferToMessage(void* p_Data, const MRH_Uint8* p_Buffer, MRH_Uint32 u32_Size);
    
#ifdef __cplusplus
}
#endif


#endif /* MRH_StreamMessage_h */
