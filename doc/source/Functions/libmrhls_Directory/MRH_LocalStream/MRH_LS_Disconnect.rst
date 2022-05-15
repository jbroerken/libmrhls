MRH_LS_Disonnect
================
The MRH_LS_Disconnect function is used to disconnect from a existing 
and connected local stream.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_LocalStream.h>


Syntax
------
.. code-block:: c

    void MRH_LS_Disconnect(MRH_LocalStream* p_Stream);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Stream
      - The :doc:`../../../Structs/libmrhls_Directory/MRH_LocalStream/MRH_LocalStream_t` 
        to disconnect.


Return Value
------------
None.

Remarks
-------
* The connection socket will still exist if disconnected in 
  server mode.

Code Examples
-------------
None.