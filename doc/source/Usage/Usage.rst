*****
Usage
*****
libmrhls is used to connect a two components, which represents the 
roles of a server and client, for communication with messages. The 
connection is done with a stream object, which manages the connection 
and messages sent. 

A connection starts once the server creates a socket for connection 
which the client connects to. Communication only happens as long as 
both components are connected.

Once connected both the client and server send and receive messages 
exchanged between each other. The messages are either pre-defined in 
structure or using a custom format specified by both components.
Messages are both sent and received one at a time.

The communication continues until either the server closes the connection 
or the client disconnects.

.. note::

    All functions will set library errors on failure. Make sure to check 
    these errors in case of a failure.
    

Only the main library header has to be included to use the event writing and 
creation functions as well as all event data structs known to the library.

.. code-block:: c

    #include <libmrhls.h>


Wanting to only work with the message struct info is possible by including the 
MRH_StreamMessage header.

.. code-block:: c

    #include <libmrhls/MRH_StreamMessage.h>


Creating a Stream
-----------------
Both the server and client start by creating a stream object. The object is 
created with the :doc:`../Functions/libmrhls_Directory/MRH_LocalStream/MRH_LS_Open` 
function. The function also defines if a component acts as a server or as a 
client.

The created loca stream object is used for all stream interaction and needs 
to be managed for the communication to be usable.

**Server:**

.. code-block:: c

    MRH_LocalStream* p_Stream;
    p_Stream = MRH_LS_Open("/path/to/socket", 1);
    
    if ((p_Stream == NULL)
    {
        // Error ...
    }


**Client:**

.. code-block:: c

    MRH_LocalStream* p_Stream;
    p_Stream = MRH_LS_Open("/path/to/socket", -1);
    
    if ((p_Stream == NULL)
    {
        // Error ...
    }


Connecting
----------
Connecting a local stream is handled the same for both server and 
client. This is done with the :doc:`../Functions/libmrhls_Directory/MRH_LocalStream/MRH_LS_Connect` 
function.

The at the time of the object creation specified socket path is used 
to connect to a socket or check for a incoming connection on the socket.

.. code-block:: c

    if (MRH_LS_Connect(p_Stream) < 0)
    {
        // Error ...
    }


.. note::

    The connection can be checked for validity with the 
    :doc:`../Functions/libmrhls_Directory/MRH_LocalStream/MRH_LS_GetConnected` 
    function.
    

.. note::

    It is recommended to first send the message version in use 
    before sending other message data.


Sending Messages
----------------
Messages will first need to be converted to a byte buffer by using the 
:doc:`../Functions/libmrhls_Directory/MRH_StreamMessage/MRH_LS_MessageToBuffer` 
function. This will function will fill a provided buffer with the message 
data given by handing a message type and struct to the function.

.. code-block:: c

    // Message data
    MRH_LS_M_Version_Data c_Version;
    MRH_Uint8 p_Buffer[MRH_STREAM_MESSAGE_BUFFER_SIZE] = { '\0' };
    MRH_Uint32 u32_Size;
    
    // Set message version number
    c_Version = MRH_STREAM_MESSAGE_VERSION;
    
    // Convert message to buffer
    if (MRH_LS_MessageToBuffer(p_Buffer, &u32_Size, MRH_LS_M_VERSION, &c_Version) < 0)
    {
        // Error ...
    }
    

The current message to write is set by calling the :doc:`../Functions/libmrhls_Directory/MRH_LocalStream/MRH_LS_Write` 
function. The function takes a byte buffer and it's size to send messages.

.. note::

    The currently set message cannot be replaced until it is fully sent.
    

.. code-block:: c

    // Send buffer for message
    if (MRH_LS_Write(p_Stream, p_Buffer, u32_Size) < 0)
    {
        // Error ...
    }
    

MRH_LS_Write will attempt to fully write the current message to sent. The status of the 
message write can be checked with the :doc:`../Functions/libmrhls_Directory/MRH_LocalStream/MRH_LS_GetWriteMessageSet` 
function which returns positive as long as a send message was not fully written.

The current message can be continued to be written with MRH_LS_Write, but the function 
:doc:`../Functions/libmrhls_Directory/MRH_LocalStream/MRH_LS_WriteContinue` can also 
be used to continue writing.

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


Receiving Messages
------------------
Messages are received by reading from the local stream when using the 
:doc:`../Functions/libmrhls_Directory/MRH_LocalStream/MRH_LS_Read` function. 
The function takes a buffer with a static size, the location of the read size 
result and the read timeout as parameters.

.. note::

    The read function will block until either data was received or 
    the specified timeout was exceeded.
    

.. warning::

    The buffer and size will neither be cleaned nor be set if the read 
    operation was incomplete.


.. code-block:: c

    MRH_Uint8 p_Buffer[MRH_STREAM_MESSAGE_BUFFER_SIZE] = { '\0' };
    MRH_Uint32 u32_Size;
    
    while (1)
    {
        int i_Result = MRH_LS_Read(p_Stream, 100, p_Buffer, &u32_Size);
        
        if (i_Result < 0)
        {
            // Error ...
        }
        else if (i_Result == 1)
        {
            continue;
        }
        
        // Read success ...
    }
    

The fully received message buffer can now be converted to a message struct 
by reading the message type first with the 
:doc:`../Functions/libmrhls_Directory/MRH_StreamMessage/MRH_LS_GetBufferMessage`  
function and then passing a matching message struct to the :doc:`../Functions/libmrhls_Directory/MRH_StreamMessage/MRH_LS_BufferToMessage` 
function.

.. code-block:: c

    MRH_LS_M_Version_Data c_Version;
    
    switch (MRH_LS_GetBufferMessage(p_Buffer))
    {
        case MRH_LS_M_VERSION:
            if (MRH_LS_BufferToMessage(&c_Version, p_Buffer, u32_Size) < 0)
            {
                // Error ...
            }
            else
            {
                // Use message struct ...
            }
            break;
    
        default:
            break;
    }


Disconnecting
-------------
Disconnecting a local stream is handled with the 
:doc:`../Functions/libmrhls_Directory/MRH_LocalStream/MRH_LS_Disconnect` 
function. Disconnecting is the same for both client and server.

.. code-block:: c

    MRH_LS_Disconnect(p_Stream);


.. warning::

    The current write and receive message set will not be cleared!


Closing Streams
---------------
The stream is closed by calling the same function for both server and 
client: :doc:`../Functions/libmrhls_Directory/MRH_LocalStream/MRH_LS_Close`. 

The server will also remove the socket if the close function is called.

.. code-block:: c

    p_Stream = MRH_LS_Close(p_Stream);


.. note::

    Closing a stream will also disconnect before closing.
  
