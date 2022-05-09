MRH_CurrentStreamMessage_t
==========================
The MRH_CurrentStreamMessage_t struct represents a message to send 
or receive.

Header
------
The struct definition can be found in the following file:

.. code-block:: c

    #include "libmrhev/MRH_LocalStreamInternal.h"


Variables
---------
.. list-table::
    :header-rows: 1

    * - Name
      - Type
      - Description
    * - p_Buffer
      - MRH_Uint8*
      - The current message bytes.
    * - u32_Handled
      - MRH_Uint32
      - The amount of bytes read / written.


Remarks
-------
None.