MRH_GetTotalSize
================
The MRH_GetTotalSize function is used to retrieve the full 
message size in bytes from a current local stream message.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include "libmrhls/MRH_LocalStream.c"


Syntax
------
.. code-block:: c

    MRH_Uint32 MRH_GetTotalSize(MRH_CurrentStreamMessage* p_Message);


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
The total message size.

Remarks
-------
* The message size will be correct if it exceeds the maximum message 
  size.

Code Examples
-------------
None.