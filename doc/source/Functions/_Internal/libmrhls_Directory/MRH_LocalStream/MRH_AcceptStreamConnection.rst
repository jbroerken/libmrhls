MRH_AcceptStreamConnection
==========================
The MRH_AcceptStreamConnection function is used by a local stream 
server to accept a connection to an existing local stream socket.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include "libmrhls/MRH_LocalStream.c"


Syntax
------
.. code-block:: c

    int MRH_AcceptStreamConnection(MRH_LocalStream* p_Stream);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Stream
      - The :doc:`../../../../Structs/libmrhls_Directory/MRH_LocalStream/MRH_LocalStream_t` 
        to accept connections on.


Return Value
------------
0 on success, -1 on failure.

Remarks
-------
* This function blocks until a connect was established.

Code Examples
-------------
None.