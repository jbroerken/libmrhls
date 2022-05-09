MRH_Local_Stream_Error_Type
===========================
The MRH_Local_Stream_Error_Type enumeration lists the errors which 
can occur during library usage.

Header
------
The enumeration definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/Error/MRH_LocalStreamError.h>


Definition
----------
The defined enumeration values are as follows:

.. code-block:: c

    typedef enum
    {
        // None
        MRH_LOCAL_STREAM_ERROR_NONE = -1,
        
        // Unknown
        MRH_LOCAL_STREAM_ERROR_UNK = 0,
        
        // General
        MRH_LOCAL_STREAM_ERROR_GENERAL_RESULT_FAIL = 1,
        MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM = 2,
        MRH_LOCAL_STREAM_ERROR_GENERAL_MALLOC = 3,
        
        // Socket
        MRH_LOCAL_STREAM_ERROR_SOCKET_CREATE = 4,
        MRH_LOCAL_STREAM_ERROR_SOCKET_CONNECT = 5,
        MRH_LOCAL_STREAM_ERROR_SOCKET_READ = 6,
        MRH_LOCAL_STREAM_ERROR_SOCKET_WRITE = 7,
        MRH_LOCAL_STREAM_ERROR_SOCKET_NO_DATA,
        MRH_LOCAL_STREAM_ERROR_SOCKET_POLL,
        
        // Bounds
        MRH_LOCAL_STREAM_ERROR_TYPE_MAX = MRH_LOCAL_STREAM_ERROR_SOCKET_POLL,

        MRH_LOCAL_STREAM_ERROR_TYPE_COUNT = MRH_LOCAL_STREAM_ERROR_TYPE_MAX + 1

    }MRH_Local_Stream_Error_Type;


Values
------
The meaning behind the error enumeration values is as follows:

.. list-table::
    :header-rows: 1

    * - Enumeration
      - Description
    * - MRH_LOCAL_STREAM_ERROR_NONE
      - No error occured.
    * - MRH_LOCAL_STREAM_ERROR_UNK
      - An unknown error occured.
    * - MRH_LOCAL_STREAM_ERROR_GENERAL_RESULT_FAIL
      - The error shows represents a failed result.
    * - MRH_LOCAL_STREAM_ERROR_GENERAL_INVALID_PARAM
      - A parameter passed to a function is not usable.
    * - MRH_LOCAL_STREAM_ERROR_GENERAL_MALLOC
      - Memory allocation failed.
    * - MRH_LOCAL_STREAM_ERROR_SOCKET_CREATE
      - Failed to create socket file.
    * - MRH_LOCAL_STREAM_ERROR_SOCKET_CONNECT
      - Socket connection failed.
    * - MRH_LOCAL_STREAM_ERROR_SOCKET_READ
      - Failed to read socket.
    * - MRH_LOCAL_STREAM_ERROR_SOCKET_WRITE
      - Failed to write socket.
    * - MRH_LOCAL_STREAM_ERROR_SOCKET_NO_DATA
      - No data available at socket.
    * - MRH_LOCAL_STREAM_ERROR_SOCKET_POLL
      - Failed to poll socket.
    * - MRH_LOCAL_STREAM_ERROR_TYPE_MAX
      - The highest error value in this enumeration.
    * - MRH_LOCAL_STREAM_ERROR_TYPE_COUNT
      - The amount of error values in this enumeration.