MRH_STREAM_MESSAGE_CUSTOM_BUFFER_SIZE
=====================================
The MRH_STREAM_MESSAGE_CUSTOM_BUFFER_SIZE macro defines the maximum 
size of the custom stream message data buffer useable in bytes.

Header
------
The macro definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_LocalStreamRevision.h>


Definition
----------
The defined size in the header is as follows:

.. code-block:: c

    #define MRH_STREAM_MESSAGE_CUSTOM_BUFFER_SIZE (MRH_STREAM_MESSAGE_BUFFER_SIZE - sizeof(MRH_Uint32))