set(QSTUDIOMETRICS_SRC
    dircompressor.cpp
    PCA/pcamodel.cpp
    CPCA/cpcamodel.cpp
    PLS/plsmodel.cpp
    EPLS/eplsmodel.cpp
    MLR/mlrmodel.cpp
    LDA/ldamodel.cpp
    VariableSelectionModel.cpp
    qstudiometricsdataoperations.cpp
    qsmdata.cpp
    DATAIO.cpp
    Dialogs/DialogPlots.cpp
    Dialogs/ClassPlotDialog.cpp
    PCA/PCAPlot.cpp
    CPCA/CPCAPlot.cpp
    PLS/PLSPlot.cpp
    EPLS/EPLSPlot.cpp
    MLR/MLRPlot.cpp
    LDA/LDAPlot.cpp
    Dialogs/ImportLabelListDialog.cpp
    Dialogs/addLabelDialog.cpp
    ListView.cpp
    Dialogs/LabelDialog.cpp
    Dialogs/VariableSelectorDialog.cpp
    Dialogs/ObjectSelectorDialog.cpp
    Dialogs/SelectionStyleColor.cpp
    Dialogs/SelectByDialog.cpp
    Dialogs/SelectionStyleDialog.cpp
    Dialogs/LinePlotStyleDialog.cpp
    Plotlib/BarPlot.cpp
    Dialogs/FindCorrelationWidget.cpp
    Plotlib/ScatterPlot.cpp
    Plotlib/SimpleLine2DPlot.cpp
    Dialogs/ModelDialog.cpp
    Dialogs/ModelDialogWizard.cpp
    Dialogs/ClassModelDialog.cpp
    Dialogs/ValidatorDialog.cpp
    Dialogs/ClassDialog.cpp
    Dialogs/DoPredictionDialog.cpp
    Dialogs/DoClusterAnalysisDialog.cpp
    Dialogs/ValidationClusterPlot.cpp
    Dialogs/VariablePlotDialog.cpp
    Plotlib/VariablePlot.cpp
    Dialogs/ProjectManager.cpp
    Dialogs/addfileDialog.cpp
    Dialogs/ImportFileDialog.cpp
    Dialogs/SaveDialog.cpp
    Dialogs/SearchOnTableDialog.cpp
    Dialogs/HighlightTableVarDialog.cpp
    Dialogs/GenericProgressDialog.cpp
    Dialogs/PlotSettingsDialog.cpp
    Table.cpp
    Dialogs/ExportTableDialog.cpp
    Dialogs/ExtractDataDialog.cpp
    Dialogs/MergeDataDialog.cpp
    Dialogs/AdvancedPretreatmentDialog.cpp
    ModelInfo.cpp
    MDIChild.cpp
    Dialogs/AboutDialog.cpp
    MainWindow.cpp
    run.cpp
    main.cpp)

set(QSTUDIOMETRICS_H
    qstudiometricsdataoperations.h
    qsmdata.h
    PCA/pcamodel.h
    CPCA/cpcamodel.h
    PLS/plsmodel.h
    EPLS/eplsmodel.h
    VariableSelectionModel.h
    DATAIO.h
    Dialogs/DialogPlots.h
    Dialogs/ClassPlotDialog.h
    PCA/PCAPlot.h
    PLS/PLSPlot.h
    EPLS/EPLSPlot.h
    MLR/MLRPlot.h
    LDA/LDAPlot.h
    Dialogs/ImportLabelListDialog.h
    Dialogs/addLabelDialog.h
    ListView.h
    Dialogs/LabelDialog.h
    Dialogs/VariableSelectorDialog.h
    Dialogs/ObjectSelectorDialog.h
    Dialogs/SelectionStyleColor.h
    Dialogs/SelectByDialog.h
    Dialogs/SelectionStyleDialog.h
    Dialogs/LinePlotStyleDialog.h
    Plotlib/BarPlot.h
    Dialogs/FindCorrelationWidget.h
    Plotlib/ScatterPlot.h
    Plotlib/SimpleLine2DPlot.h
    Dialogs/ModelDialog.h
    Dialogs/ModelDialogWizard.h
    Dialogs/ClassModelDialog.h
    Dialogs/ValidatorDialog.h
    Dialogs/ClassDialog.h
    Dialogs/DoPredictionDialog.h
    Dialogs/DoClusterAnalysisDialog.h
    Dialogs/ValidationClusterPlot.h
    Dialogs/VariablePlotDialog.h
    Plotlib/VariablePlot.h
    Dialogs/ProjectManager.h
    Dialogs/addfileDialog.h
    Dialogs/ImportFileDialog.h
    Dialogs/SaveDialog.h
    Dialogs/SearchOnTableDialog.h
    Dialogs/HighlightTableVarDialog.h
    Dialogs/GenericProgressDialog.h
    Dialogs/PlotSettingsDialog.h
    Table.h
    Dialogs/ExportTableDialog.h
    Dialogs/ExtractDataDialog.h
    Dialogs/MergeDataDialog.h
    Dialogs/AdvancedPretreatmentDialog.h
    ModelInfo.h
    MDIChild.h
    Dialogs/AboutDialog.h
#     Dialogs/ProgressBarDialog.h
    MainWindow.h
    run.h)

set(QSTUDIOMETRICS_UIS
    Dialogs/ImportLabelListDialog.ui
    Dialogs/addLabelDialog.ui
    ListView.ui
    Dialogs/LabelDialog.ui
    Dialogs/VariableSelectorDialog.ui
    Dialogs/ObjectSelectorDialog.ui
    Dialogs/SelectByDialog.ui
    Dialogs/SelectionStyleColor.ui
    Dialogs/SelectionStyleDialog.ui
    Dialogs/LinePlotStyleDialog.ui
    Plotlib/BarPlot.ui
    Dialogs/FindCorrelationWidget.ui
    Plotlib/ScatterPlot.ui
    Plotlib/SimpleLine2DPlot.ui
    Dialogs/DialogPlots.ui
    Dialogs/ClassPlotDialog.ui
    Dialogs/ModelDialog.ui
    Dialogs/ModelDialogWizard.ui
    Dialogs/ClassModelDialog.ui
    Dialogs/ValidatorDialog.ui
    Dialogs/ClassDialog.ui
    Dialogs/DoPredictionDialog.ui
    Dialogs/DoClusterAnalysisDialog.ui
    Dialogs/ValidationClusterPlot.ui
    Dialogs/VariablePlotDialog.ui
    Dialogs/ProjectManager.ui
    Dialogs/addfileDialog.ui
    Dialogs/ImportFileDialog.ui
    Dialogs/SaveDialog.ui
    Dialogs/SearchOnTableDialog.ui
    Dialogs/HighlightTableVarDialog.ui
    Dialogs/GenericProgressDialog.ui
    Dialogs/PlotSettingsDialog.ui
    Table.ui
    Dialogs/ExportTableDialog.ui
    Dialogs/ExtractDataDialog.ui
    Dialogs/MergeDataDialog.ui
    Dialogs/AdvancedPretreatmentDialog.ui
    ModelInfo.ui
    Dialogs/AboutDialog.ui
#     Dialogs/ProgressBarDialog.ui
    MainWindow.ui)

#add_subdirectory(Chart)

set(QSTUDIOMETRICS_CMDPCA_SRC DATAIO.cpp qstudiom-pca.cpp)
set(QSTUDIOMETRICS_CMDPLS_SRC DATAIO.cpp qstudiom-pls.cpp)
set(QSTUDIOMETRICS_CMDMLR_SRC DATAIO.cpp qstudiom-mlr.cpp)
set(QSTUDIOMETRICS_CMDUPCA_SRC DATAIO.cpp qstudiom-upca.cpp)
set(QSTUDIOMETRICS_CMDUPLS_SRC DATAIO.cpp qstudiom-upls.cpp)
set(QSTUDIOMETRICS_CMDBINTEST_SRC DATAIO.cpp qstudiom-bintest.cpp)
set(QSTUDIOMETRICS_CMDDOE_SRC DATAIO.cpp qstudiom-doe.cpp)
set(QSTUDIOMETRICS_CMDMDC_SRC DATAIO.cpp qstudiom-mdc.cpp)
set(QSTUDIOMETRICS_CMDKMEANS_SRC DATAIO.cpp qstudiom-kmeans.cpp)
set(QSTUDIOMETRICS_CMDHGM_SRC DATAIO.cpp qstudiom-hypgridmap.cpp)


set(icon_RCC icons/icons.qrc)

# start building interface from moc
#set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
QT6_WRAP_CPP(QSTUDIOMETRICS_MOC_H ${QSTUDIOMETRICS_H})
QT6_WRAP_UI(QSTUDIOMETRICS_UIS_H ${QSTUDIOMETRICS_UIS})
# generate rules for building source files from the resources
QT6_ADD_RESOURCES(icon_RCC_SRCS ${icon_RCC})


source_group("Resources" FILES
  ${QSTUDIOMETRICS_UIS}
)

source_group("Generated" FILES
  ${QSTUDIOMETRICS_UIS_H}
  ${QSTUDIOMETRICS_MOC_H}
)

if(WIN32)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE)
  set(APP_ICON_RC ${CMAKE_CURRENT_SOURCE_DIR}/win32_icon.rc)
  set(WIN32_ICON_O ${CMAKE_CURRENT_BINARY_DIR}/_app_icon.o)
  ADD_CUSTOM_COMMAND(OUTPUT ${WIN32_ICON_O} COMMAND windres.exe ARGS -I${CMAKE_CURRENT_SOURCE_DIR} -o${WIN32_ICON_O} -i${APP_ICON_RC} DEPENDS ${APP_ICON_RC})
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})


if(WIN32)
  if(cmake_build_type_tolower STREQUAL "release")
    add_executable(QStudioMetrics WIN32 ${QSTUDIOMETRICS_SRC} ${QSTUDIOMETRICS_MOC_H} ${QSTUDIOMETRICS_UIS_H} ${icon_RCC_SRCS} ${WIN32_ICON_O})
  else()
    add_executable(QStudioMetrics ${QSTUDIOMETRICS_SRC} ${QSTUDIOMETRICS_MOC_H} ${QSTUDIOMETRICS_UIS_H} ${icon_RCC_SRCS} ${WIN32_ICON_O})
  endif()
else(WIN32)
add_executable(QStudioMetrics ${QSTUDIOMETRICS_SRC} ${QSTUDIOMETRICS_MOC_H} ${QSTUDIOMETRICS_UIS_H} ${icon_RCC_SRCS})
endif()

target_link_libraries(QStudioMetrics PRIVATE ${LIBSCIENTIFIC_LIBRARY} libchart ${QT6_LIBRARIES})

add_executable(qsm-pca ${QSTUDIOMETRICS_CMDPCA_SRC})
target_link_libraries(qsm-pca PRIVATE ${LIBSCIENTIFIC_LIBRARY})

add_executable(qsm-pls ${QSTUDIOMETRICS_CMDPLS_SRC})
target_link_libraries(qsm-pls PRIVATE ${LIBSCIENTIFIC_LIBRARY})

add_executable(qsm-mlr ${QSTUDIOMETRICS_CMDMLR_SRC})
target_link_libraries(qsm-mlr PRIVATE ${LIBSCIENTIFIC_LIBRARY})

add_executable(qsm-mdc ${QSTUDIOMETRICS_CMDMDC_SRC})
target_link_libraries(qsm-mdc PRIVATE ${LIBSCIENTIFIC_LIBRARY})

add_executable(qsm-kmeans ${QSTUDIOMETRICS_CMDKMEANS_SRC})
target_link_libraries(qsm-kmeans PRIVATE ${LIBSCIENTIFIC_LIBRARY})

add_executable(qsm-hgm ${QSTUDIOMETRICS_CMDHGM_SRC})
target_link_libraries(qsm-hgm PRIVATE ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-objselector qsmcompselector.cpp DATAIO.cpp)
#target_link_libraries(qsm-objselector PRIVATE ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-mlr ${QSTUDIOMETRICS_CMDMLR_SRC})
#target_link_libraries(qsm-mlr PRIVATE ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-lda ${QSTUDIOMETRICS_CMDLDA_SRC})
#target_link_libraries(qsm-lda PRIVATE ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-upca ${QSTUDIOMETRICS_CMDUPCA_SRC})
#target_link_libraries(qsm-upca PRIVATE ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-upls ${QSTUDIOMETRICS_CMDUPLS_SRC})
#target_link_libraries(qsm-upls PRIVATE ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-bintest ${QSTUDIOMETRICS_CMDBINTEST_SRC})
#target_link_libraries(qsm-bintest PRIVATE ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-doe ${QSTUDIOMETRICS_CMDDOE_SRC})
#target_link_libraries(qsm-doe PRIVATE ${LIBSCIENTIFIC_LIBRARY})


#install(TARGETS QStudioMetrics RUNTIME DESTINATION bin ${QSM_INSTALL_DESTINATIONS_ARGS})
install(TARGETS QStudioMetrics DESTINATION bin COMPONENT ApplicationData)

if(cmake_build_type_tolower STREQUAL "debug")
  add_executable(testimportdvector testimportdvector.cpp DATAIO.cpp)
  add_executable(testimportmatrix testimportmatrix.cpp DATAIO.cpp)
  add_executable(testimportarray testimportarray.cpp DATAIO.cpp)
  add_executable(testimportpcamodel testimportpcamodel.cpp DATAIO.cpp)
  add_executable(qsmprojectcompressor qsmprojectcompressor.cpp dircompressor.cpp)

  target_link_libraries(testimportdvector ${LIBSCIENTIFIC_LIBRARY})
  target_link_libraries(testimportmatrix ${LIBSCIENTIFIC_LIBRARY})
  target_link_libraries(testimportarray ${LIBSCIENTIFIC_LIBRARY})
  target_link_libraries(testimportpcamodel ${LIBSCIENTIFIC_LIBRARY})
  target_link_libraries(qsmprojectcompressor ${QT6_LIBRARIES})
endif()
