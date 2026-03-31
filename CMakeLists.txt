cmake_minimum_required(VERSION 3.22)
project(QStudioMetrics)

set(VERSION "2.5.0")
set(AUTHOR "Giuseppe Marco Randazzo")

# Bundle Metadata
set(MACOSX_BUNDLE_GUI_IDENTIFIER "com.gmrandazzo.qstudiometrics")
set(MACOSX_BUNDLE_BUNDLE_NAME "QStudioMetrics")
set(MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION}")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}")
set(MACOSX_BUNDLE_COPYRIGHT "Copyright (C) 2016-2026 ${AUTHOR}")
set(MACOSX_BUNDLE_ICON_FILE "QStudioMetrics.icns")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(_components Core Core5Compat Gui Widgets Sql PrintSupport Concurrent Charts Network)
foreach(_component ${_components})
  find_package(Qt6 COMPONENTS ${_component} REQUIRED)
  list(APPEND QT6_LIBS "Qt6::${_component}")
endforeach()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")
find_package(libscientific)

if(NOT LIBSCIENTIFIC_FOUND)
    include(ExternalProject)
    set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)
    file(MAKE_DIRECTORY ${EXTERNAL_INSTALL_LOCATION}/include)
    ExternalProject_Add(libscientific_git
        GIT_REPOSITORY https://github.com/gmrandazzo/libscientific
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION})
    set(LIBSCIENTIFIC_DEPENDENCY libscientific_git)
    include_directories(${EXTERNAL_INSTALL_LOCATION}/include)
    link_directories(${EXTERNAL_INSTALL_LOCATION}/lib)
    if(WIN32)
        set(LIBSCIENTIFIC_LIBRARY ${EXTERNAL_INSTALL_LOCATION}/lib/libscientific.dll)
    elseif(APPLE)
        set(LIBSCIENTIFIC_LIBRARY ${EXTERNAL_INSTALL_LOCATION}/lib/libscientific.dylib)
    else()
        set(LIBSCIENTIFIC_LIBRARY ${EXTERNAL_INSTALL_LOCATION}/lib/libscientific.so)
    endif()
else()
  include_directories(${LIBSCIENTIFIC_INCLUDE_DIRS})
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-extra-semi")

include_directories(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})
add_subdirectory(src/Chart)
add_subdirectory(src)

set(CPACK_PACKAGE_NAME "QStudioMetrics")
set(CPACK_PACKAGE_VENDOR "${AUTHOR}")
set(CPACK_PACKAGE_VERSION "${VERSION}")
include(CPack)
