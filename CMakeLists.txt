project(QStudioMetrics)
cmake_minimum_required(VERSION 3.22)
set(VERSION "2.3.0")
set(AUTHOR "Giuseppe Marco Randazzo")
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: DEBUG or RELEASE" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_tolower)

#set(_components
#    Core
#    Gui
#    DBus
#    Designer
#    Declarative
#    Script
#    ScriptTools
#    Network
#    Test
#    Xml
#    Svg
#    Sql
#    Widgets
#    PrintSupport
#    Concurrent
#    UiTools
#    WebKit
#    WebKitWidgets
#    OpenGL)


set(_components Core Core5Compat Gui Widgets Sql PrintSupport Concurrent Charts)
foreach(_component ${_components})
  find_package(Qt6 COMPONENTS ${_component} REQUIRED)
  list(APPEND QT6_LIBRARIES ${Qt6${_component}_LIBRARIES})
  if(cmake_build_type_tolower STREQUAL "release")
    set_target_properties(${Qt6${_component}_LIBRARIES} PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
  endif()
endforeach()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")
find_package(libscientific)

if(NOT ${LIBSCIENTIFIC_FOUND})
  message(STATUS "Cloning libscientific")
    include(ExternalProject)
    set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
    ExternalProject_Add(libscientific_git
    GIT_REPOSITORY https://github.com/gmrandazzo/libscientific
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION})
    include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
    link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
    if(WIN32)
        set(LIBSCIENTIFIC_LIBRARY ${EXTERNAL_INSTALL_LOCATION}/lib/libscientific.dll)
    elseif(APPLE)
        set(LIBSCIENTIFIC_LIBRARY ${EXTERNAL_INSTALL_LOCATION}/lib/libscientific.dylib)
    else()
        set(LIBSCIENTIFIC_LIBRARY ${EXTERNAL_INSTALL_LOCATION}/lib/libscientific.so)
    endif(WIN32)
else()
  include_directories(${LIBSCIENTIFIC_INCLUDE_DIRS})
endif()

# Disable some warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wno-extra-semi")

# IF RELEASE
if(cmake_build_type_tolower STREQUAL "release")
  set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
endif()


string(TOLOWER "${PLOT_ENGINE}" plot_engine_tolower)
if(plot_engine_tolower STREQUAL "webkit")
  message(STATUS "Plot engine: WEBKIT")
  add_definitions(-DWEBKIT)
endif()

set(CMAKE_CXX_STANDARD 20)

#include(CheckCXXCompilerFlag)
#CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
#CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

if(WIN32)
  message(STATUS "Build Release: WIN32")
  # add -static if you whant to compile with gcc lib static
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -DRELEASE -DWIN32 -DBUILDEXEC -Wno-long-long")
  # add to the linker flags the static library -static-libgcc -static-libstdc++
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined -Wl,-O1 -Wl,--as-needed -Wl,--sort-common -s -Wl,-O1")
  SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
elseif(APPLE)
  if(cmake_build_type_tolower STREQUAL "debug")
    message(STATUS "Build Debug: APPLE")
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -pedantic -g -DDEBUG")
  else()
  message(STATUS "Build Release: APPLE")
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRELEASE -DOSX -O2 -Wall -Wnewline-eof")
    add_definitions(-DBUILDEXEC)
  endif()
else()
  message(STATUS "Build Release: Linux")
  if(cmake_build_type_tolower STREQUAL "debug")
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -pedantic -g -Wno-long-long ")
  else()
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRELEASE -O2 -Wall -Wno-long-long ")
  endif()
endif(WIN32)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

add_definitions(${QT6_DEFINITIONS})

include_directories(
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(src/Chart)
add_subdirectory (src)
