MRH_ERR_GetLocalStreamError
===========================
The MRH_ERR_GetLocalStreamError function is used to retrieve the current library 
error value.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/Error/MRH_LocalStreamError.h>


Syntax
------
.. code-block:: c

    MRH_Local_Stream_Error_Type MRH_ERR_GetLocalStreamError(void);


Parameters
----------
None.

Return Value
------------
The current :doc:`../../../../Enumerations/libmrhls_Directory/Error/MRH_LocalStreamError/MRH_Local_Stream_Error_Type`.

Remarks
-------
* The error value will not be reset after retrieving.

Code Examples
-------------
None.