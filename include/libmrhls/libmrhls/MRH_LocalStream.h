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

#ifndef MRH_LocalStream_h
#define MRH_LocalStream_h

// C

// External

// Project
#include "./MRH_StreamMessage.h"


#ifdef __cplusplus
extern "C"
{
#endif
    
    //*************************************************************************************
    // Object
    //*************************************************************************************
    
    struct MRH_LocalStream_t;
    typedef struct MRH_LocalStream_t MRH_LocalStream;
    
    //*************************************************************************************
    // Open
    //*************************************************************************************
    
    /**
     *  Open a new local stream.
     *  
     *  \param p_FilePath The full path to the socket file.
     *  \param i_Create If the socket file should be created.
     *  
     *  \return The new local stream on success, NULL on failure.
     */
  
    extern MRH_LocalStream* MRH_LS_Open(const char* p_FilePath, int i_Create);
    
    //*************************************************************************************
    // Connect
    //*************************************************************************************
    
    /**
     *  Connect a opened local stream. This function blocks until a connection was 
     *  established.
     *  
     *  \param p_Stream The stream to connect with.  
     *  
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_LS_Connect(MRH_LocalStream* p_Stream);
    
    //*************************************************************************************
    // Read
    //*************************************************************************************
    
    /**
     *  Read from a local stream.
     *  
     *  \param p_Stream The stream to read from.
     *  \param i_TimeoutMS The time in milliseconds to wait for data.
     *  
     *  \return 1 if unfinished, 0 if finished and -1 on failure.
     */
    
    extern int MRH_LS_Read(MRH_LocalStream* p_Stream, int i_TimeoutMS);
    
    //*************************************************************************************
    // Write
    //*************************************************************************************
    
    /**
     *  Write to a local stream.
     *  
     *  \param p_Stream The stream to write to.
     *  
     *  \return 1 if unfinished, 0 if finished and -1 on failure.
     */
    
    extern int MRH_LS_Write(MRH_LocalStream* p_Stream);
    
    //*************************************************************************************
    // Disconnect
    //*************************************************************************************
    
    /**
     *  Disconnect a local stream.
     *  
     *  \param p_Stream The stream to disconnect.  
     */
    
    extern void MRH_LS_Disconnect(MRH_LocalStream* p_Stream);
    
    //*************************************************************************************
    // Close
    //*************************************************************************************
    
    /**
     *  Close a local stream.
     *  
     *  \param p_Stream The stream to close.
     *  
     *  \return Always NULL.
     */
    
    extern MRH_LocalStream* MRH_LS_Close(MRH_LocalStream* p_Stream);
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Check if the local stream is connected.
     *  
     *  \param p_Stream The stream to check.    
     *  
     *  \return 0 if connected, -1 if not.
     */
    
    extern int MRH_LS_GetConnected(MRH_LocalStream* p_Stream);
    
    /**
     *  Get the currently read local stream message.
     *  
     *  \param p_Stream The stream to check.  
     *  
     *  \return The read message on success, MRH_LSM_UNK on failure or nothing read.
     */
    
    extern MRH_StreamMessage MRH_LS_GetLastMessage(MRH_LocalStream* p_Stream);
    
    /**
     *  Get the last read local stream message data.
     *  
     *  \param p_Stream The stream to check.    
     *  \param p_Data The data to write to.
     *  
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_LS_GetLastMessageData(MRH_LocalStream* p_Stream, void* p_Data);
    
    //*************************************************************************************
    // Setters
    //*************************************************************************************
    
    /**
     *  Set the stream message to write on a local stream.
     *  
     *  \param p_Stream The stream to write with.
     *  \param e_Message The stream message to write.
     *  \param p_Data The stream message data to write.
     *  
     *  \return 0 on success, -1 on failure.
     */
    
    extern int MRH_LS_SetMessage(MRH_LocalStream* p_Stream, MRH_StreamMessage e_Message, const void* p_Data);
    
#ifdef __cplusplus
}
#endif


#endif /* MRH_LocalStream_h */
