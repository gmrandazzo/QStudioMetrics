PROJECT(libchart)
cmake_minimum_required(VERSION 3.22)
set(AUTHOR "Giuseppe Marco Randazzo")
set(VERSION "1.3.0")

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
#    OpenGL
#    Charts
#    DataVisualization)

if(WIN32)
  set(_components Core Gui Concurrent Widgets WebKitWidgets Network PrintSupport Charts)
  add_definitions(-DWEBKIT)
else()
  set(_components Core Gui Concurrent Widgets PrintSupport WebEngineCore WebEngineWidgets Charts)
endif()

foreach(_component ${_components})
  find_package(Qt6 COMPONENTS ${_component} REQUIRED )
  list(APPEND QT6_LIBRARIES ${Qt6${_component}_LIBRARIES})
  if(cmake_build_type_tolower STREQUAL "release")
    set_target_properties(${Qt6${_component}_LIBRARIES} PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")
  endif()
endforeach()

add_definitions(${QT6_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

if(WIN32)
  message(STATUS "OS: Windows")
  # add -static if you whant to compile with gcc lib static
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pipe -DWIN32 -mwindows -pedantic -Wall")
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

set(CHART_SRC
    plotinfo.cpp
    datapoint.cpp
    datacurve.cpp
    databar.cpp
    qplotly.cpp
    chartqt.cpp
    chart.cpp)

set(CHART_H
    plotinfo.h
    datapoint.h
    datacurve.h
    databar.h
    qplotly.h
    chartqt.h
    chart.h)


set(QPLOTLY_TEST_SRC testqplotly.cpp)
set(CHART_TEST_SRC testchart.cpp)
set(CHARTQT_TEST_SRC testchartqt.cpp)

set(icon_RCC chart.qrc)

# start building interface from moc
QT6_WRAP_CPP(CHART_MOC_H ${CHART_H})
QT6_ADD_RESOURCES(icon_RCC_SRCS ${icon_RCC})

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${QT_USE_FILE})

add_library(libchart SHARED ${CHART_SRC} ${CHART_H} ${icon_RCC_SRCS} ${CHART_MOC_H})
#add_library(libchart SHARED ${CHART_SRC} ${CHART_H})

#add_library(libchart STATIC ${CHART_SRC} ${CHART_H})
set_target_properties(libchart PROPERTIES OUTPUT_NAME "chart")

if(WIN32)
  target_link_libraries(libchart  PRIVATE ${QT6_LIBRARIES} Qt6::WinMain "-mwindows")
else()
  target_link_libraries(libchart PRIVATE ${QT6_LIBRARIES})
endif()

add_executable(testqplotly ${QPLOTLY_TEST_SRC})
target_link_libraries(testqplotly PRIVATE libchart ${QT6_LIBRARIES})

add_executable(testchart ${CHART_TEST_SRC})
target_link_libraries(testchart PRIVATE libchart ${QT6_LIBRARIES})

#add_executable(testchartqt ${CHARTQT_TEST_SRC})
#target_link_libraries(testchartqt PRIVATE libchart ${QT6_LIBRARIES})

# install(TARGETS libchart LIBRARY DESTINATION lib COMPONENT Libraries)
