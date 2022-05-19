MRH_LS_Connect
==============
The MRH_LS_Connect function is used to connect a existing 
local stream.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_LocalStream.h>


Syntax
------
.. code-block:: c

    int MRH_LS_Connect(MRH_LocalStream* p_Stream);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Stream
      - The :doc:`../../../Structs/libmrhls_Directory/MRH_LocalStream/MRH_LocalStream_t` 
        to connect.


Return Value
------------
0 on success, -1 on failure.

Remarks
-------
* This function blocks until a connection was established.
* The function will accept a connection on the created socket 
  in server mode.
* The function will try to connect to the stream socket file 
  in client mode.

Code Examples
-------------
.. code-block:: c

    if (MRH_LS_Connect(p_Stream) < 0)
    {
        // Error ...
    }

