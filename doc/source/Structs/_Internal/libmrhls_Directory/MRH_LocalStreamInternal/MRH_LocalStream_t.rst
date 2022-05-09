MRH_LocalStream_t
=================
The MRH_LocalStream_t struct contains the information used by 
the local stream to send and receive messages.

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
    * - i_ConnectionFD
      - int
      - The file descriptor for the connection socket of the server.
    * - i_MessageFD
      - int
      - The message exchange socket file descriptor.
    * - p_FilePath
      - char*
      - The file path for the used connection socket.
    * - i_IsServer
      - int
      - The server identification flag.
    * - c_Send
      - :doc:`MRH_CurrentStreamMessage_t` 
      - The currently sent message.
    * - c_Receive
      - :doc:`MRH_CurrentStreamMessage_t` 
      - The currently received message.


Remarks
-------
* This struct is given to the library user as an opague struct.