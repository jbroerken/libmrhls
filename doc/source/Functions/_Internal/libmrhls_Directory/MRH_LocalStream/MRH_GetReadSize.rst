MRH_GetReadSize
===============
The MRH_GetReadSize function is used to get the amount of bytes 
remaining to be read for a received message.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include "libmrhls/MRH_LocalStream.c"


Syntax
------
.. code-block:: c

    MRH_Uint32 MRH_GetReadSize(MRH_CurrentStreamMessage* p_Message);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Message
      - The :doc:`../../../../Structs/_Internal/libmrhls_Directory/MRH_LocalStreamInternal/MRH_CurrentStreamMessage_t` 
        to check.


Return Value
------------
The remaining message read size in bytes.

Remarks
-------
* The read size will return the byte size of the total message 
  size if that value is not fully read.

Code Examples
-------------
None.