PROJECT(libqplotly)
cmake_minimum_required(VERSION 2.8)
set(AUTHOR "Giuseppe Marco Randazzo")
set(VERSION "1.0.0")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MACOSX_RPATH 1)


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

set(_components Core Gui Widgets PrintSupport WebEngine WebEngineWidgets)
foreach(_component ${_components})
  find_package(Qt5${_component} REQUIRED )
  list(APPEND QT5_LIBRARIES ${Qt5${_component}_LIBRARIES})
  if(cmake_build_type_tolower STREQUAL "release")
    set_target_properties(${Qt5${_component}_LIBRARIES} PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
  endif()
endforeach()


add_definitions(${QT5_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

if(WIN32)
  message(STATUS "OS: Windows")
  # add -static if you whant to compile with gcc lib static
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pipe -mwindows -pedantic -Wall")
  # add to the linker flags the static library -static-libgcc -static-libstdc++
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s -Wl,-O1")
elseif(APPLE)
  message(STATUS "Build Release: APPLE")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wno-long-long -DDEBUG")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wno-long-long")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
else(WIN32)
  message(STATUS "OS: Unix")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pedantic -Wall -Werror -g")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-O1")
endif(WIN32)

# set(CMAKE_INSTALL_PREFIX "/usr/local/")

set(QPLOTLY_SRC
    datapoint.cpp
    datacurve.cpp
    databar.cpp
    qplotly.cpp)

set(QPLOTLY_H
    datapoint.h
    datacurve.h
    databar.h
    qplotly.h)

set(QPLOTLY_TEST_SRC testqplotly.cpp)

# start building interface from moc
QT5_WRAP_CPP(QPLOTLY_MOC_H ${QPLOTLY_H})

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${QT_USE_FILE})

add_library(libqplotly SHARED ${QPLOTLY_SRC} ${QPLOTLY_H} ${QPLOTLY_MOC_H})
#add_library(libqplotly STATIC ${QPLOTLY_SRC} ${QPLOTLY_H})
set_target_properties(libqplotly PROPERTIES OUTPUT_NAME "qplotly")

if(WIN32)
  target_link_libraries(libqplotly  ${QT5_LIBRARIES} Qt5::WinMain "-mwindows")
else()
  target_link_libraries(libqplotly ${QT5_LIBRARIES})


  add_executable(testqplotly ${QPLOTLY_TEST_SRC})
  target_link_libraries(testqplotly libqplotly ${QT5_LIBRARIES})
endif()

# install(TARGETS libqplotly LIBRARY DESTINATION lib COMPONENT Libraries)
