MRH_LS_Close
============
The MRH_LS_Close function is used to close a existing local stream.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_LocalStream.h>


Syntax
------
.. code-block:: c

    MRH_LocalStream* MRH_LS_Close(MRH_LocalStream* p_Stream);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - p_Stream
      - The :doc:`../../../Structs/libmrhls_Directory/MRH_LocalStream/MRH_LocalStream_t` 
        to close.


Return Value
------------
Always NULL.

Remarks
-------
* This function will also disconnect the stream before closing.
* Closing a stream in server mode will unlink the connection socket.

Code Examples
-------------
None.