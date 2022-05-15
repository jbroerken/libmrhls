MRH_CreateStreamObject
======================
The MRH_CreateStreamObject function allocates and initializes a 
new local stream struct.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include "libmrhls/MRH_LocalStream.c"


Syntax
------
.. code-block:: c

    MRH_LocalStream* MRH_CreateStreamObject(const char* p_FilePath, 
                                            int i_IsServer);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_FilePath
      - The full socket file path.
    * - i_IsServer
      - If the local stream struct represents a 
        server or client.


Return Value
------------
The new :doc:`../../../../Structs/libmrhls_Directory/MRH_LocalStream/MRH_LocalStream_t` 
on success, NULL on failure.

Remarks
-------
None.

Code Examples
-------------
None.