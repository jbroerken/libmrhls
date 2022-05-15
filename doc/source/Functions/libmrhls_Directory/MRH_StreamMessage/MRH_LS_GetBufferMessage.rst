MRH_LS_GetBufferMessage
=======================
The MRH_LS_GetBufferMessage function is used to check the message 
type of a full message data buffer.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_StreamMessage.h>


Syntax
------
.. code-block:: c

    MRH_LS_Message MRH_LS_GetBufferMessage(const MRH_Uint8* p_Buffer);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Buffer
      - The message buffer to check.


Return Value
------------
The :doc:`../../../Enumerations/libmrhls_Directory/MRH_StreamMessage/MRH_LS_Message` on success, 
MRH_LS_M_UNK on failure.

Remarks
-------
* The message type might actually be MRH_LS_M_UNK, which is still 
  considered a invalid message.

Code Examples
-------------
None.