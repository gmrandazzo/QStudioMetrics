set(QSTUDIOMETRICS_SRC
    dircompressor.cpp
    pcamodel.cpp
    plsmodel.cpp
    upcamodel.cpp
    uplsmodel.cpp
    mlrmodel.cpp
    ldamodel.cpp
    VariableSelectionModel.cpp
    data.cpp
    DATAIO.cpp
    PlotDialog.cpp
    ClassPlotDialog.cpp
    PCAPlot.cpp
    PLSPlot.cpp
    UPCAPlot.cpp
    UPLSPlot.cpp
    MLRPlot.cpp
    LDAPlot.cpp
    ImportLabelListDialog.cpp
    addLabelDialog.cpp
    ListView.cpp
    LabelDialog.cpp
    VariableSelectorDialog.cpp
    ObjectSelectorDialog.cpp
    SelectionStyleColor.cpp
    SelectByDialog.cpp
    SelectionStyleDialog.cpp
    LinePlotStyleDialog.cpp
    BarPlot.cpp
    FindCorrelationWidget.cpp
    ScatterPlot3D.cpp
    SimpleScatterPlot3D.cpp
    ScatterPlot2D.cpp
    SimpleLine2DPlot.cpp
    ModelDialog.cpp
    ClassModelDialog.cpp
    ValidatorDialog.cpp
    ClassDialog.cpp
    DoPredictionDialog.cpp
    DoClusterAnalysisDialog.cpp
    ValidationClusterPlot.cpp
    VariablePlotDialog.cpp
    VariablePlot.cpp
    VariableSelectionDialog.cpp
    ProjectManager.cpp
    addfileDialog.cpp
    ImportFromFileDialog.cpp
    ImportFileDialog.cpp
    SaveDialog.cpp
    SearchOnTableDialog.cpp
    HighlightTableVarDialog.cpp
    GenericProgressDialog.cpp
    Table.cpp
    ExportTableDialog.cpp
    ExtractDataDialog.cpp
    MergeDataDialog.cpp
    AdvancedPretreatmentDialog.cpp
    ModelInfo.cpp
    MDIChild.cpp
    AboutDialog.cpp
#     ProgressBarDialog.cpp
    MainWindow.cpp
    run.cpp
    main.cpp)

set(QSTUDIOMETRICS_H
    data.h
    pcamodel.h
    plsmodel.h
    upcamodel.h
    uplsmodel.h
    VariableSelectionModel.h
    DATAIO.h
    PlotDialog.h
    ClassPlotDialog.h
    PCAPlot.h
    PLSPlot.h
    UPCAPlot.h
    UPLSPlot.h
    MLRPlot.h
    LDAPlot.h
    ImportLabelListDialog.h
    addLabelDialog.h
    ListView.h
    LabelDialog.h
    VariableSelectorDialog.h
    ObjectSelectorDialog.h
    SelectionStyleColor.h
    SelectByDialog.h
    SelectionStyleDialog.h
    LinePlotStyleDialog.h
    BarPlot.h
    FindCorrelationWidget.h
    ScatterPlot3D.h
    SimpleScatterPlot3D.h
    ScatterPlot2D.h
    SimpleLine2DPlot.h
    ModelDialog.h
    ClassModelDialog.h
    ValidatorDialog.h
    ClassDialog.h
    DoPredictionDialog.h
    DoClusterAnalysisDialog.h
    ValidationClusterPlot.h
    VariablePlotDialog.h
    VariablePlot.h
    VariableSelectionDialog.h
    ProjectManager.h
    addfileDialog.h
    ImportFromFileDialog.h
    ImportFileDialog.h
    SaveDialog.h
    SearchOnTableDialog.h
    HighlightTableVarDialog.h
    GenericProgressDialog.h
    Table.h
    ExportTableDialog.h
    ExtractDataDialog.h
    MergeDataDialog.h
    AdvancedPretreatmentDialog.h
    ModelInfo.h
    MDIChild.h
    AboutDialog.h
#     ProgressBarDialog.h
    MainWindow.h
    run.h)

set(QSTUDIOMETRICS_UIS
    ImportLabelListDialog.ui
    addLabelDialog.ui
    ListView.ui
    LabelDialog.ui
    VariableSelectorDialog.ui
    ObjectSelectorDialog.ui
    SelectByDialog.ui
    SelectionStyleColor.ui
    SelectionStyleDialog.ui
    LinePlotStyleDialog.ui
    BarPlot.ui
    FindCorrelationWidget.ui
    ScatterPlot2D.ui
    ScatterPlot3D.ui
    SimpleScatterPlot3D.ui
    SimpleLine2DPlot.ui
    PlotDialog.ui
    ClassPlotDialog.ui
    ModelDialog.ui
    ClassModelDialog.ui
    ValidatorDialog.ui
    ClassDialog.ui
    DoPredictionDialog.ui
    DoClusterAnalysisDialog.ui
    ValidationClusterPlot.ui
    VariablePlotDialog.ui
    VariableSelectionDialog.ui
    ProjectManager.ui
    addfileDialog.ui
    ImportFromFileDialog.ui
    ImportFileDialog.ui
    SaveDialog.ui
    SearchOnTableDialog.ui
    HighlightTableVarDialog.ui
    GenericProgressDialog.ui
    Table.ui
    ExportTableDialog.ui
    ExtractDataDialog.ui
    MergeDataDialog.ui
    AdvancedPretreatmentDialog.ui
    ModelInfo.ui
    AboutDialog.ui
#     ProgressBarDialog.ui
    MainWindow.ui)

add_subdirectory(Chart)

set(QSTUDIOMETRICS_CMDPCA_SRC DATAIO.cpp qstudiom-pca.cpp)
set(QSTUDIOMETRICS_CMDPLS_SRC DATAIO.cpp qstudiom-pls.cpp)
set(QSTUDIOMETRICS_CMDUPCA_SRC DATAIO.cpp qstudiom-upca.cpp)
set(QSTUDIOMETRICS_CMDUPLS_SRC DATAIO.cpp qstudiom-upls.cpp)
set(QSTUDIOMETRICS_CMDBINTEST_SRC DATAIO.cpp qstudiom-bintest.cpp)
set(QSTUDIOMETRICS_CMDDOE_SRC DATAIO.cpp qstudiom-doe.cpp)
set(QSTUDIOMETRICS_CMDMDC_SRC DATAIO.cpp qstudiom-mdc.cpp)

set(icon_RCC icons/icons.qrc)

# start building interface from moc
#set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
QT5_WRAP_CPP(QSTUDIOMETRICS_MOC_H ${QSTUDIOMETRICS_H})
QT5_WRAP_UI(QSTUDIOMETRICS_UIS_H ${QSTUDIOMETRICS_UIS})
# generate rules for building source files from the resources
QT5_ADD_RESOURCES(icon_RCC_SRCS ${icon_RCC})


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
  if(APP_ICON_RC)
    set(WIN32_ICON_O ${CMAKE_CURRENT_BINARY_DIR}/_app_icon.o)
    ADD_CUSTOM_COMMAND( OUTPUT ${WIN32_ICON_O} COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -o${WIN32_ICON_O} -i${APP_ICON_RC})
  endif()
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(QStudioMetrics ${QSTUDIOMETRICS_SRC} ${QSTUDIOMETRICS_MOC_H} ${QSTUDIOMETRICS_UIS_H} ${icon_RCC_SRCS})
target_link_libraries(QStudioMetrics ${LIBSCIENTIFIC_LIBRARY} libchart ${QT5_LIBRARIES} ${VTK_LIBRARIES})

add_executable(qsm-pca ${QSTUDIOMETRICS_CMDPCA_SRC})
target_link_libraries(qsm-pca ${LIBSCIENTIFIC_LIBRARY})

add_executable(qsm-pls ${QSTUDIOMETRICS_CMDPLS_SRC})
target_link_libraries(qsm-pls ${LIBSCIENTIFIC_LIBRARY})

add_executable(qsm-mdc ${QSTUDIOMETRICS_CMDMDC_SRC})
target_link_libraries(qsm-mdc ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-objselector qsmcompselector.cpp DATAIO.cpp)
#target_link_libraries(qsm-objselector ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-mlr ${QSTUDIOMETRICS_CMDMLR_SRC})
#target_link_libraries(qsm-mlr ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-lda ${QSTUDIOMETRICS_CMDLDA_SRC})
#target_link_libraries(qsm-lda ${LIBSCIENTIFIC_LIBRARY})s

#add_executable(qsm-upca ${QSTUDIOMETRICS_CMDUPCA_SRC})
#target_link_libraries(qsm-upca ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-upls ${QSTUDIOMETRICS_CMDUPLS_SRC})
#target_link_libraries(qsm-upls ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-bintest ${QSTUDIOMETRICS_CMDBINTEST_SRC})
#target_link_libraries(qsm-bintest ${LIBSCIENTIFIC_LIBRARY})

#add_executable(qsm-doe ${QSTUDIOMETRICS_CMDDOE_SRC})
#target_link_libraries(qsm-doe ${LIBSCIENTIFIC_LIBRARY})


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
  target_link_libraries(qsmprojectcompressor ${QT5_LIBRARIES})
endif()
