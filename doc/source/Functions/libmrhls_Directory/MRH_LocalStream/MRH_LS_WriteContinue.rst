MRH_LS_WriteContinue
====================
The MRH_LS_WriteContinue function is used to write 
the current set message to a existing and connected 
local stream.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_LocalStream.h>


Syntax
------
.. code-block:: c

    int MRH_LS_WriteContinue(MRH_LocalStream* p_Stream);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Stream
      - The :doc:`../../../Structs/libmrhls_Directory/MRH_LocalStream/MRH_LocalStream_t` 
        to write to.


Return Value
------------
1 if unfinished, 0 on fully written, -1 on failure.

Remarks
-------
* Trying to write without a set message will cause nothing to 
  be written.

Code Examples
-------------
.. code-block:: c

    // Not fully written?
    while (MRH_LS_GetWriteMessageSet(p_Stream) == 0)
    {
        // Continue sending
        if (MRH_LS_WriteContinue(p_Stream) < 0)
        {
            // Error ...
        }
    }

