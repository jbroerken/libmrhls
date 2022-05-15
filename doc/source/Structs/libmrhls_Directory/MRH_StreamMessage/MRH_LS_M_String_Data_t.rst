MRH_LS_M_String_Data_t
======================
The MRH_LS_M_String_Data_t struct represents a string message.

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
    * - p_String
      - char*
      - The message string.


Remarks
-------
* The data buffer size is static.
* The data buffer should be initialized with terminators.