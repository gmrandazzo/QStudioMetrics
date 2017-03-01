PROJECT(libqchart)
cmake_minimum_required(VERSION 2.8)
set(AUTHOR "Giuseppe Marco Randazzo")
set(VERSION "1.0.5")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_MACOSX_RPATH 1)

find_package(Qt5 COMPONENTS Core Widgets PrintSupport)
set(QT5_LIBRARIES Qt5::Widgets Qt5::PrintSupport Qt5::Core)
add_definitions(${QT5_DEFINITIONS})

if(WIN32)
  message(STATUS "OS: Windows")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pipe -mwindows -pedantic -Wall -static")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libstdc++ -s -Wl,-O1")
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

set(QCHART_SRC
    qchart.cpp
    datapoint.cpp
    datacurve.cpp)

set(QCHART_H
    qchart.h
    datapoint.h
    datacurve.h)

set(QCHARTTEST_SRC testqchart.cpp)

set(icon_RCC qchart.qrc)

# start building interface from moc
QT5_WRAP_CPP(QCHART_MOC_H ${QCHART_H})
QT5_ADD_RESOURCES(icon_RCC_SRCS ${icon_RCC})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(libqchart SHARED ${QCHART_SRC} ${QCHART_H} ${icon_RCC_SRCS})
#add_library(libqchart STATIC ${QCHART_SRC} ${QCHART_H} ${icon_RCC_SRCS})
set_target_properties(libqchart PROPERTIES OUTPUT_NAME "qchart")


if(WIN32)
  target_link_libraries(libqchart Qt5::Widgets "-mwindows")
else()
  target_link_libraries(libqchart ${QT5_LIBRARIES})

  add_executable(test ${QCHARTTEST_SRC})
  target_link_libraries(test libqchart ${QT5_LIBRARIES})
endif()

# install(TARGETS libqchart LIBRARY DESTINATION lib COMPONENT Libraries)
