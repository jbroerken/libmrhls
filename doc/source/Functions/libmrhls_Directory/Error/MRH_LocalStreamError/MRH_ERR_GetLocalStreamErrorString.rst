MRH_ERR_GetLocalStreamErrorString
=================================
The MRH_ERR_GetLocalStreamErrorString function is used to retrieve the description 
string for the current library error value.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/Error/MRH_LocalStreamError.h>


Syntax
------
.. code-block:: c

    const char* MRH_ERR_GetLocalStreamErrorString(void);


Parameters
----------
None.

Return Value
------------
The current library error string.

Remarks
-------
* The string returned depends on the currently set 
  :doc:`../../../../Enumerations/libmrhls_Directory/Error/MRH_LocalStreamError/MRH_Local_Stream_Error_Type`.

Code Examples
-------------
None.