MRH_LS_M_Audio_Data_t
=====================
The MRH_LS_M_Audio_Data_t struct represents a audio buffer message.

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
    * - u32_KHz
      - MRH_Uint32
      - The Khz for the whole audio buffer.
    * - u32_Samples
      - MRH_Uint32
      - The amount of samples found in the buffer.
    * - p_Samples
      - MRH_Sint16*
      - The PCM16 audio samples.


Remarks
-------
* The audio buffer size is static.
* The used audio size is the sample count * sizeof(MRH_Sint16).