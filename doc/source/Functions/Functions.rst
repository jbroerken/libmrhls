*********
Functions
*********
The list of functions shown here includes information about their use case as 
well as the required parameters and return values. Code examples are given if 
deemed neccessary.

Internal functions are used by the library itself. These functions are not 
meant to be used directly for application development.

Function errors can be retrieved by using the get error functions after the 
called function failed.

Library Functions
-----------------
.. toctree::
  :maxdepth: 1

  libmrhls_Directory/Error/MRH_LocalStreamError/MRH_ERR_LocalStreamReset
  libmrhls_Directory/Error/MRH_LocalStreamError/MRH_ERR_GetLocalStreamError
  libmrhls_Directory/Error/MRH_LocalStreamError/MRH_ERR_GetLocalStreamErrorString
  libmrhls_Directory/MRH_LocalStream/MRH_LS_Open
  libmrhls_Directory/MRH_LocalStream/MRH_LS_Connect
  libmrhls_Directory/MRH_LocalStream/MRH_LS_Read
  libmrhls_Directory/MRH_LocalStream/MRH_LS_Write
  libmrhls_Directory/MRH_LocalStream/MRH_LS_WriteContinue
  libmrhls_Directory/MRH_LocalStream/MRH_LS_Disconnect
  libmrhls_Directory/MRH_LocalStream/MRH_LS_Close
  libmrhls_Directory/MRH_LocalStream/MRH_LS_GetConnected
  libmrhls_Directory/MRH_LocalStream/MRH_LS_GetWriteMessageSet
  libmrhls_Directory/MRH_StreamMessage/MRH_LS_MessageToBuffer
  libmrhls_Directory/MRH_StreamMessage/MRH_LS_GetBufferMessage
  libmrhls_Directory/MRH_StreamMessage/MRH_LS_BufferToMessage
  libmrhls_Directory/MRH_LocalStreamRevision/MRH_GetLocalStreamLibraryVersionMajor
  libmrhls_Directory/MRH_LocalStreamRevision/MRH_GetLocalStreamLibraryVersionMinor
  libmrhls_Directory/MRH_LocalStreamRevision/MRH_GetLocalStreamLibraryVersionPatch

Internal Functions
------------------
.. toctree::
  :maxdepth: 1
  
  _Internal/libmrhls_Directory/Error/MRH_LocalStreamErrorInternal/MRH_ERR_SetLocalStreamError
  _Internal/libmrhls_Directory/MRH_LocalStream/MRH_ResetMessage
  _Internal/libmrhls_Directory/MRH_LocalStream/MRH_GetTotalSize
  _Internal/libmrhls_Directory/MRH_LocalStream/MRH_CreateStreamObject
  _Internal/libmrhls_Directory/MRH_LocalStream/MRH_AcceptStreamConnection
  _Internal/libmrhls_Directory/MRH_LocalStream/MRH_ConnectToStream
  _Internal/libmrhls_Directory/MRH_LocalStream/MRH_GetReadSize