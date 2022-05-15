MRH_LS_M_Location_Data_t
========================
The MRH_LS_M_Location_Data_t struct represents a location message.

Header
------
The struct definition can be found in the following file:

.. code-block:: c

    #include <libmrhls/MRH_StreamMessage.h>


Variables
---------
.. list-table::
    :header-rows: 1

    * - Name
      - Type
      - Description
    * - f64_Latitude
      - MRH_Sfloat64
      - The location latitude.
    * - f64_Longtitude
      - MRH_Sfloat64
      - The location longitude.
    * - f64_Elevation
      - MRH_Sfloat64
      - The location elevation.
    * - f64_Facing
      - MRH_Sfloat64
      - The location facing.


Remarks
-------
* The value interpretation depends on the sender.