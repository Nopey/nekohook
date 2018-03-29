
#
#
# Welcome to the cmake lists file, this controls how nekohook will build for your system
#
# TODO, make wasy way to do "touch ../src/CMakeLists.txt" to make things easy
#
#

cmake_minimum_required (VERSION 2.6)
project (libnekohook)

## Defaults
# User options can be set with cmake -D"NAME OF OPTION HERE"="VARIABLE", e.g. "cmake -DCGAME=csgo"
set(CGAME tf2 CACHE STRING "Game to build with")
set(CGFX valve_surface CACHE STRING "Graphics module to use")
set(CINP xlib CACHE STRING "Input module to use")
set(CUSERNAME UNREGISTERED CACHE STRING "Register a username")
set(CUSERID 0 CACHE STRING "Register a id")
set(CForceClang false CACHE STRING "Should cmake force compilation with clang?")
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE RELEASE)
endif(NOT CMAKE_BUILD_TYPE)

# Function to remove source files from a directory, ripped from aimtux fuzion <3
FUNCTION(EXCLUDE_FILES_FROM_DIR_IN_LIST _InFileList _excludeDirName)
  foreach(ITR ${_InFileList})
    if("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)") # Check if the item matches the directory name in _excludeDirName
      if(CMAKE_BUILD_TYPE MATCHES DEBUG)
        message(STATUS "Remove Item from List:${ITR}")
      endif()
      list(REMOVE_ITEM _InFileList ${ITR}) # Remove the item from the list
    endif()
  endforeach()
  set(SOURCE_FILES ${_InFileList} PARENT_SCOPE) # Return the SOURCE_FILES variable to the calling parent
ENDFUNCTION()

# Add cathooks source files except the modules, we add those selectivly
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.c*")
EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCE_FILES}" "${CMAKE_SOURCE_DIR}/src/modules/")
#these source files will be compiled & linked first, therefor their globals init'd first.
set(EARLY_SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/util/catvars.cpp" "${CMAKE_SOURCE_DIR}/src/util/logging.cpp" "${CMAKE_SOURCE_DIR}/src/framework/console.cpp")

# Flags we use for every build type
set(CMAKE_CXX_STANDARD 17)
set(CCOMMON_FLAGS "")

## Game modules
message(STATUS "Game module: ${CGAME}")
# Team Fortress game module
if(CGAME STREQUAL "tf2")
	add_definitions("-DCATHOOK_TF2=1")
  set(CBASE "SOURCE_2013_SDK")
# Fistful of Frags game module
elseif(CGAME STREQUAL "fof")
	add_definitions("-DCATHOOK_FOF=1")
	set(CBASE "SOURCE_2013_SDK")
elseif(CGAME STREQUAL "csgo")
	add_definitions("-DCATHOOK_CSGO=1")
  file(GLOB_RECURSE TMP_SOURCES "${CMAKE_SOURCE_DIR}/src/modules/csgo/*.c*")
	list(APPEND SOURCE_FILES ${TMP_SOURCES})
endif()

## Graphics modules
message(STATUS "Graphics module: ${CGFX}")
# Valve surface
if(CGFX STREQUAL "valve_surface")
	add_definitions("-DCATHOOK_GFX_SURFACE=1")
elseif(CGFX STREQUAL "gl_draw")
  add_definitions("-DCATHOOK_GFX_GL=1")
  list(APPEND SOURCE_FILES "src/modules/gl_draw/gl_draw.cpp")
  add_subdirectory(src/modules/gl_draw/libglez/)
  include_directories(SYSTEM src/modules/gl_draw/libglez/include)
  #TODO: I don't remember why rdynamic is needed
  add_compile_options(-rdynamic)
  list(APPEND LINK_LIBRARIES glez)
  #TODO: Don't use xlib on windows!
  set(USE_XLIB true)
endif()

## Input modules
message(STATUS "Input module: ${CINP}")
# Valve surface
if(CINP STREQUAL "xlib")
	add_definitions("-DCATHOOK_INP_XLIB=1")
  list(APPEND SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/modules/xlib/input.cpp")
  set(USE_XLIB true)
elseif(CINP STREQUAL "sdl")
  # TODO: Find TF2 SDL2
  list(APPEND LINK_LIBRARIES $ENV{HOME}/.steam/steam/steamapps/common/Team\ Fortress\ 2/bin/libSDL2-2.0.so.0)
  FIND_PATH(SDL2_INCLUDE_DIR SDL.h
  HINTS
  $ENV{SDL2DIR}
  PATH_SUFFIXES include/SDL2 include
  PATHS ~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt)
  message("SDL Include Dir:" ${SDL2_INCLUDE_DIR})
  include_directories(SYSTEM ${SDL2_INCLUDE_DIR})
  add_definitions("-DCATHOOK_INP_SDL=1")
  list(APPEND SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/modules/sdl/sdl.cpp")
  set(USE_SDL true)
endif()

## SDK/Bases if needed
# The source sdk that everyone loves
if(CBASE STREQUAL "SOURCE_2013_SDK")
	set(CCOMMON_FLAGS "${CCOMMON_FLAGS} -m32")
  add_definitions(-DCATHOOK_SOURCE_2013=1)
  file(GLOB_RECURSE TMP_SOURCES "${CMAKE_SOURCE_DIR}/src/modules/source2013/*.c*")
	list(APPEND SOURCE_FILES ${TMP_SOURCES}) # add our source sdk files
endif()

# If anything uses xlib, we need the main file
if(USE_XLIB)
  list(APPEND SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/modules/xlib/xlib.cpp")
endif()

# If anything uses sdl, we need the main file
if(USE_SDL)
  list(APPEND SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/modules/sdl/sdl.cpp")
endif()

# Default compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "${CCOMMON_FLAGS} -g3 -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "${CCOMMON_FLAGS} -s -mtune=generic -O3")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Flags: ${CMAKE_CXX_FLAGS_RELEASE}")

#Some files need to init first
foreach(item ${EARLY_SOURCE_FILES})
 list(REMOVE_ITEM SOURCE_FILES ${item})
endforeach()

#include_directories(${INCLUDE_DIRS})
add_library(nekohook SHARED ${EARLY_SOURCE_FILES} ${SOURCE_FILES})

target_link_libraries(nekohook c stdc++ ${LINK_LIBRARIES})
