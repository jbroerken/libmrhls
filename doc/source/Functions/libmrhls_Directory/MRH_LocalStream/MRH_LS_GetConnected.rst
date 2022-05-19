MRH_LS_GetConnected
===================
The MRH_LS_GetConnected function is used to check if a 
local stream is currently connected.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_LocalStream.h>


Syntax
------
.. code-block:: c

    int MRH_LS_GetConnected(MRH_LocalStream* p_Stream);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Stream
      - The :doc:`../../../Structs/libmrhls_Directory/MRH_LocalStream/MRH_LocalStream_t` 
        to check.


Return Value
------------
0 if connected, -1 if not.

Remarks
-------
* The connection is checked by looking for a connection socket for 
  sending messages.

Code Examples
-------------
.. code-block:: c

    if (MRH_LS_GetConnected(p_Stream) < 0)
    {
        // Not connected
    }
    else
    {
        // Connected
    }



