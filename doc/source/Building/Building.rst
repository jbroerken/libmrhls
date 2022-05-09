********
Building
********
The project files are already prepared and include everything except the 
required dependencies. This document lists both dependencies as well as the 
build process to create the library from source.

Supported Platforms
-------------------
The supported platforms for libmrhls are UNIX-likes, primarily 
Debian-based GNU/Linux Distributions like the Rapsberry Pi OS.

Dependencies
------------
This library has the following dependencies:

* mrhshared: https://github.com/jbroerken/mrhshared/

Build Tools
-----------
This release includes a CMake script (CMakeLists.txt) for a simplified build 
process. The minimal required version for CMake is 3.1.
Also needed is the GNU C Compiler. Full C99 support is required.

Changing Pre-defined Settings
-----------------------------
This library has no pre-defined settings to change in the CMakeLists.txt.

Build Process
-------------
The build process should be relatively straightforward:

1. Aqquire dependencies.
2. Move into the project "build" folder.
3. Compile Makefiles with the included CMakeLists.txt.
4. Run "make" to compile the library.
5. Install the compiled library and its header files.


Shell Commands
--------------
The following shell commands will create makefiles with the 
provided CMakeLists.txt, compile the project with the created 
makefiles and install the library and headers:

.. code-block::

    cd <Project Root Folder>/build
    cmake ..
    make
    sudo make install
