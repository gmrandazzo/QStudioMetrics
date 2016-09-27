cmake_minimum_required(VERSION 2.8)
set(Version "1.8.0")

project(QStudioMetrics)

find_package(Qt4 REQUIRED)
#find_package(Qt5Widgets REQUIRED)

include(${QT_USE_FILE})
SET(QT_USE_QTNETWORK TRUE)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE}) # include UseVTK.cmake

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif(NOT CMAKE_BUILD_TYPE)

string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_tolower)

# Disable some warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-c++11-long-long -Wno-extra-semi -Wno-c++11-extensions -Wno-extra-semi")

if(WIN32)
  message(STATUS "Build Release: WIN32")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -DRELEASE -static -DWIN32 -Wno-long-long")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined -static-libgcc -static-libstdc++ -Wl,-O1 -Wl,--as-needed -Wl,--sort-common -s -Wl,-O1")
  SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG)
  add_definitions(${QT_DEFINITIONS})
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
elseif(APPLE) 
  if(cmake_build_type_tolower STREQUAL "debug")
    message(STATUS "Build Debug: APPLE")
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -pedantic -g")
  else()
  message(STATUS "Build Release: APPLE")
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRELEASE -O2 -Wall -Wnewline-eof")
  endif()
  add_definitions(${QT_DEFINITIONS})
else()
  message(STATUS "Build Release: Linux")
  if(cmake_build_type_tolower STREQUAL "debug")
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -pedantic -g -Wno-long-long ")
    add_definitions(${QT_DEFINITIONS} --debug)
  else()
    set(CMAKE_BUILD_TYPE "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DRELEASE -O2 -Wall -Wno-long-long ")
    add_definitions(${QT_DEFINITIONS})
  endif()
endif(WIN32)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

if(NOT SCIENTIFIC_LIB_DIR)
  message(SEND_ERROR "Please specify the libscientific library path with -DSCIENTIFIC_LIB_DIR=...")
endif()

if(NOT SCIENTIFIC_INCLUDE_DIR)
  message(SEND_ERROR "Please specify the libscientific include path with -DSCIENTIFIC_INCLUDE_DIR=...")
endif()

include_directories(
  ${PROJECT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_BINARY_DIR} #this is where ui_SimpleViewUI.h is generated
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${SCIENTIFIC_INCLUDE_DIR}
)

link_directories(${SCIENTIFIC_LIB_DIR})

add_subdirectory (src)
