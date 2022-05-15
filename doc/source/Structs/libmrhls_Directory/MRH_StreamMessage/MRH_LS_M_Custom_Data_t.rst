MRH_LS_M_Custom_Data_t
======================
The MRH_LS_M_Custom_Data_t struct represents a custom message.

Header
------
The struct definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_StreamMessage.h>


Variables
---------
.. list-table::
    :header-rows: 1

    * - Name
      - Type
      - Description
    * - u32_Size
      - MRH_Uint32
      - The size of the custom data buffer in bytes.
    * - p_Buffer
      - MRH_Uint8*
      - The custom message data.


Remarks
-------
* The data buffer size is static.
* Custom data in the buffer after the given size might be 
  uninitialized.