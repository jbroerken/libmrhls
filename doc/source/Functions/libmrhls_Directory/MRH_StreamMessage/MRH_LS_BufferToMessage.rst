MRH_LS_BufferToMessage
======================
The MRH_LS_BufferToMessage function is used to write a message 
struct with a matching message data buffer.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_StreamMessage.h>


Syntax
------
.. code-block:: c

    int MRH_LS_BufferToMessage(void* p_Data, 
                               const MRH_Uint8* p_Buffer, 
                               MRH_Uint32 u32_Size);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Data
      - The message data struct to write.
    * - p_Buffer
      - The message buffer containing the message values.
        The buffer has to be of size 
        :doc:`../../../Macros/libmrhls_Directory/MRH_StreamMessage/MRH_STREAM_MESSAGE_BUFFER_SIZE`.
    * - u32_Size
      - The size of the given message buffer in bytes.


Return Value
------------
0 on success, -1 on failure.

Remarks
-------
* The struct has to match the buffer message type. Use the 
  :doc:`MRH_LS_GetBufferMessage` function to get the message 
  type.

Code Examples
-------------
.. code-block:: c
    
    // Define struct info
    MRH_LS_M_Version_Data c_Version;
    MRH_Uint8 p_Buffer[MRH_STREAM_MESSAGE_BUFFER_SIZE] = { '\0' };
    MRH_Uint32 u32_Size;
    
    // Read message to buffer
    // ...
    
    // Check message type
    // ...
    
    // Convert to struct
    if (MRH_LS_BufferToMessage(&c_Version, p_Buffer, u32_Size) < 0)
    {
        // Error ...
    }
    else
    {
        // Use message struct ...
    }

