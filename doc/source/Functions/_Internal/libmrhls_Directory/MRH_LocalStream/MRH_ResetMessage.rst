MRH_ResetMessage
================
The MRH_ResetMessage function is used to reset a current read or 
write message of the local stream.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include "libmrhls/MRH_LocalStream.c"


Syntax
------
.. code-block:: c

    void MRH_ResetMessage(MRH_CurrentStreamMessage* p_Message);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Message
      - The :doc:`../../../../Structs/_Internal/libmrhls_Directory/MRH_LocalStreamInternal/MRH_CurrentStreamMessage_t` 
        to reset.


Return Value
------------
None.

Remarks
-------
* Only the message size will be reset.

Code Examples
-------------
None.