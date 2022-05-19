MRH_LS_GetWriteMessageSet
=========================
The MRH_LS_GetWriteMessageSet function is used to check if a 
message to write is currently set.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_LocalStream.h>


Syntax
------
.. code-block:: c

    int MRH_LS_GetWriteMessageSet(MRH_LocalStream* p_Stream);


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
0 if set, -1 if not.

Remarks
-------
None.

Code Examples
-------------
.. code-block:: c

    if (MRH_LS_GetWriteMessageSet(p_Stream) < 0)
    {
        // No message set
    }
    else
    {
        // Message set, keep writing
    }
