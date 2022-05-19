MRH_LS_Write
============
The MRH_LS_Write function is used to set write data and write 
to a existing and connected local stream.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_LocalStream.h>


Syntax
------
.. code-block:: c

    int MRH_LS_Write(MRH_LocalStream* p_Stream, 
                     const MRH_Uint8* p_Buffer, 
                     MRH_Uint32 u32_Size);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Stream
      - The :doc:`../../../Structs/libmrhls_Directory/MRH_LocalStream/MRH_LocalStream_t` 
        to write to.
    * - p_Buffer
      - The buffer to write from.
    * - u32_Size
      - The full message write size in bytes.


Return Value
------------
1 if unfinished, 0 on fully written, -1 on failure.

Remarks
-------
* The function will continue to write the currently set 
  message if unfinished even if a new message is given.

Code Examples
-------------
.. code-block:: c

    // Define message buffer info
    MRH_Uint8 p_Buffer[MRH_STREAM_MESSAGE_BUFFER_SIZE] = { '\0' };
    MRH_Uint32 u32_Size;

    // Convert message struct to buffer
    // ...
    
    // Send buffer for message
    if (MRH_LS_Write(p_Stream, p_Buffer, u32_Size) < 0)
    {
        // Error ...
    }

