MRH_ERR_SetLocalStreamError
===========================
The MRH_ERR_SetLocalStreamError function is used to set the current 
library error value.

Header
------
The function definition can be found in the following file:

.. code-block:: c

    #include "libmrhls/Error/MRH_LocalStreamErrorInternal.h"


Syntax
------
.. code-block:: c

    void MRH_ERR_SetLocalStreamError(MRH_Local_Stream_Error_Type e_Error);


Parameters
----------
.. list-table::
    :header-rows: 1

    * - Parameter
      - Description
    * - e_Error
      - The new :doc:`../../../../../Enumerations/libmrhls_Directory/Error/MRH_LocalStreamError/MRH_Local_Stream_Error_Type`.


Return Value
------------
None.

Remarks
-------
None.

Code Examples
-------------
None.