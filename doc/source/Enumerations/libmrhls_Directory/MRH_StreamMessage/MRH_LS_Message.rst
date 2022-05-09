MRH_LS_Message
==============
The MRH_LS_Message enumeration lists all provided stream 
message types.

Header
------
The enumeration definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_StreamMessage.h>


Definition
----------
The defined enumeration values are as follows:

.. code-block:: c

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


Values
------
The meaning behind the enumeration values is as follows:

.. list-table::
    :header-rows: 1

    * - Enumeration
      - Description
    * - MRH_LS_M_UNK
      - Unknown message type.
    * - MRH_LS_M_VERSION
      - Message containing the local stream message version.
    * - MRH_LS_M_CUSTOM
      - Message with custom data.
    * - MRH_LS_M_STRING
      - Text string message.
    * - MRH_LS_M_AUDIO
      - Audio buffer message.
    * - MRH_LS_M_AUDIO_PLAYBACK_FINISHED
      - Audio playback finished message.
    * - MRH_LS_M_AUDIO_START_RECORDING
      - Message for audio source to start recording.
    * - MRH_LS_M_AUDIO_STOP_RECORDING
      - Message for audio source to stop recording.
    * - MRH_LS_M_LOCATION
      - Message for location data.
    * - MRH_LS_MESSAGE_MAX
      - The highest value in this enumeration.
    * - MRH_LS_MESSAGE_COUNT
      - The amount of values in this enumeration.