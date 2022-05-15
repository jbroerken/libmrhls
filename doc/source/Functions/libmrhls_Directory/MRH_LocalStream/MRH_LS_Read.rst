MRH_LS_Read
===========
The MRH_LS_Read function is used to read from a existing 
and connected local stream.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_LocalStream.h>


Syntax
------
.. code-block:: c

    int MRH_LS_Read(MRH_LocalStream* p_Stream, 
                    int i_TimeoutMS, 
                    MRH_Uint8* p_Buffer, 
                    MRH_Uint32* p_Size);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Stream
      - The :doc:`../../../Structs/libmrhls_Directory/MRH_LocalStream/MRH_LocalStream_t` 
        to read from.
    * - i_TimeoutMS
      - The timeout to wait for data in milliseconds.
    * - p_Buffer
      - The buffer to read into. The buffer has to be 
        of size :doc:`../../../Macros/libmrhls_Directory/MRH_StreamMessage/MRH_STREAM_MESSAGE_BUFFER_SIZE`.
    * - p_Size
      - The full message read size in bytes.


Return Value
------------
1 if unfinished read, 0 on fully read, -1 on failure.

Remarks
-------
* The buffer and buffer size will only be written if the 
  received message was fully read.

Code Examples
-------------
None.