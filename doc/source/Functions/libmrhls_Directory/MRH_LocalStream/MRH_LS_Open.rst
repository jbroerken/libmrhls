MRH_LS_Open
===========
The MRH_LS_Open function is used to open a new local stream.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_LocalStream.h>


Syntax
------
.. code-block:: c

    MRH_LocalStream* MRH_LS_Open(const char* p_FilePath, 
                                 int i_Create);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_FilePath
      - The full socket file path.
    * - i_Create
      - If the socket should be created (server mode).


Return Value
------------
The new :doc:`../../../Structs/libmrhls_Directory/MRH_LocalStream/MRH_LocalStream_t` 
on success, NULL on failure.

Remarks
-------
None.

Code Examples
-------------
.. code-block:: c

    MRH_LocalStream* p_Stream;
    int i_CreateSocket = -1;
    
    p_Stream = MRH_LS_Open("/path/to/socket", i_CreateSocket);
    
    if ((p_Stream == NULL)
    {
        // Error ...
    }

