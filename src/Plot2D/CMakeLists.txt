set(PLOT2D_SRC
    SimpleLine2DPlot.cpp
    ScatterPlot2D.cpp)

set(PLOT2D_H
    ScatterPlot2D.h
    SimpleLine2DPlot.h)

set(PLOT2D_UIS
    ScatterPlot2D.ui
    SimpleLine2DPlot.ui)

# start building interface from moc
#set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
QT5_WRAP_CPP(PLOT2D_MOC_H ${PLOT2D_H})
QT5_WRAP_UI(PLOT2D_UIS_H ${PLOT2D_UIS})
# generate rules for building source files from the resources
QT5_ADD_RESOURCES(icon_RCC_SRCS ${icon_RCC})


source_group("Resources" FILES ${PLOT2D_UIS})

source_group("Generated" FILES
  ${PLOT2D_UIS_H}
  ${PLOT2D_MOC_H})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_library(libqsmplot2D SHARED ${PLOT2D_SRC} ${PLOT2D_H})
