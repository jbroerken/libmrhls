#########################################################################
#
#  CMAKE
#
#########################################################################

###
#  Minimum Version
#  ---------------
#  The CMake version required.
###
cmake_minimum_required(VERSION 3.1)

###
#  CMake Configuration
#  -------------------
#  Configuration settings for CMake.
#
#  NOTE:
#  These settings have to be applied before the project() setting!
###
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_STANDARD 99)

###
#  Project Info
#  ------------
#  General simple information about our project.
###
project(libmrhls VERSION 1.0.0
                 DESCRIPTION "MRH Platform local stream library"
                 LANGUAGES C)
set(LIB_SO_VERSION 1)

#########################################################################
#
#  PATHS
#
#########################################################################

###
#  Install Paths
#  -------------
#  GNU system paths for installation.
###
include(GNUInstallDirs)

###
#  Build Paths
#  -----------
#  The paths for the cmake build.
###
set(BUILD_DIR_PATH "${CMAKE_SOURCE_DIR}/build/")
file(MAKE_DIRECTORY ${BUILD_DIR_PATH})

###
#  Source Paths
#  ------------
#  The paths to the source files to use.
###
set(SRC_DIR_PATH "${CMAKE_SOURCE_DIR}/src/")
set(INCLUDE_DIR_PATH "${CMAKE_SOURCE_DIR}/include/")

set(SRC_LIST_BASE "${SRC_DIR_PATH}/libmrhls/Error/MRH_LocalStreamError.c"
                  "${SRC_DIR_PATH}/libmrhls/Error/MRH_LocalStreamErrorInternal.h"
                  "${SRC_DIR_PATH}/libmrhls/MRH_LocalStreamRevision.c"
                  "${SRC_DIR_PATH}/libmrhls/MRH_LocalStream.c"
                  "${SRC_DIR_PATH}/libmrhls/MRH_LocalStreamInternal.h"
                  "${INCLUDE_DIR_PATH}/libmrhls/libmrhls/Error/MRH_LocalStreamError.h"
                  "${INCLUDE_DIR_PATH}/libmrhls/libmrhls/MRH_StreamMessage.h"
                  "${INCLUDE_DIR_PATH}/libmrhls/libmrhls/MRH_LocalStream.h"
                  "${INCLUDE_DIR_PATH}/libmrhls/libmrhls/MRH_LocalStreamRevision.h"
                  "${INCLUDE_DIR_PATH}/libmrhls/libmrhls.h")

###
#  Public API Path
#  ---------------
#  The path to the public api include folder.
###
set(PUBLIC_API_DIR_PATH "${INCLUDE_DIR_PATH}/libmrhls")
					 
#########################################################################
#
#  TARGET
#
#########################################################################

###
#  Target
#  ------
#  The target(s) to build.
###
add_library(libmrhls_Shared SHARED ${SRC_LIST_BASE})
set_target_properties(libmrhls_Shared
                      PROPERTIES
                      PREFIX "lib"
                      OUTPUT_NAME "mrhls"
                      SUFFIX ".so"
                      VERSION ${PROJECT_VERSION}
                      SOVERSION ${LIB_SO_VERSION})

###
#  Install
#  -------
#  Library installation.
###
install(TARGETS libmrhls_Shared
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY "${PUBLIC_API_DIR_PATH}/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
        PATTERN "*.h")