MRH_LS_MessageToBuffer
======================
The MRH_LS_MessageToBuffer function is used to write a message 
data buffer with values from a matching message struct.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_StreamMessage.h>


Syntax
------
.. code-block:: c

    int MRH_LS_MessageToBuffer(MRH_Uint8* p_Buffer, 
                               MRH_Uint32* p_Size, 
                               MRH_LS_Message e_Message, 
                               const void* p_Data);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Buffer
      - The message buffer containing the message values.
        The buffer has to be of size 
        :doc:`../../../Macros/libmrhls_Directory/MRH_StreamMessage/MRH_STREAM_MESSAGE_BUFFER_SIZE`.
    * - p_Size
      - The buffer size of the written buffer.
    * - e_Message
      - The :doc:`../../../Enumerations/libmrhls_Directory/MRH_StreamMessage/MRH_LS_Message` 
        of the given message struct to write.
    * - p_Data
      - The message data struct to read values from.
      

Return Value
------------
0 on success, -1 on failure.

Remarks
-------
* A failed result might not overwrite the buffer or 
  buffer size arguments given.

Code Examples
-------------
.. code-block:: c

    // Message data
    MRH_LS_M_Version_Data c_Version;
    MRH_Uint8 p_Buffer[MRH_STREAM_MESSAGE_BUFFER_SIZE] = { '\0' };
    MRH_Uint32 u32_Size;
    
    // Set message version number
    c_Version = MRH_STREAM_MESSAGE_VERSION;
    
    // Convert message to buffer
    if (MRH_LS_MessageToBuffer(p_Buffer, &u32_Size, MRH_LS_M_VERSION, &c_Version) < 0)
    {
        // Error ...
    }

