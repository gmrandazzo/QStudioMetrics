option(ENABLE_PLUGINS "Enable plugin support" OFF)
set(PLUGINS_PATH "" CACHE PATH "Path to the QStudioMetrics-Plugins directory")

set(QSTUDIOMETRICS_SRC
    PCA/pcamodel.cpp
    CPCA/cpcamodel.cpp
    PLS/plsmodel.cpp
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
    VariableSelectionModel.h
    DATAIO.h
    Dialogs/DialogPlots.h
    Dialogs/ClassPlotDialog.h
    PCA/PCAPlot.h
    PLS/PLSPlot.h
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

if(ENABLE_PLUGINS)
    add_definitions(-DENABLE_PLUGINS)
    list(APPEND QSTUDIOMETRICS_SRC PluginManager.cpp)
    if(PLUGINS_PATH)
        include_directories(${PLUGINS_PATH})
        list(APPEND QSTUDIOMETRICS_H PluginManager.h)
    else()
        list(APPEND QSTUDIOMETRICS_H PluginManager.h)
    endif()
endif()

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

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/src)


if(WIN32)
  if(cmake_build_type_tolower STREQUAL "release")
    add_executable(QStudioMetrics WIN32 ${QSTUDIOMETRICS_SRC} ${QSTUDIOMETRICS_MOC_H} ${QSTUDIOMETRICS_UIS_H} ${icon_RCC_SRCS} ${WIN32_ICON_O})
  else()
    add_executable(QStudioMetrics ${QSTUDIOMETRICS_SRC} ${QSTUDIOMETRICS_MOC_H} ${QSTUDIOMETRICS_UIS_H} ${icon_RCC_SRCS} ${WIN32_ICON_O})
  endif()
elseif(APPLE)
  add_executable(QStudioMetrics MACOSX_BUNDLE ${QSTUDIOMETRICS_SRC} ${QSTUDIOMETRICS_MOC_H} ${QSTUDIOMETRICS_UIS_H} ${icon_RCC_SRCS})
else()
  add_executable(QStudioMetrics ${QSTUDIOMETRICS_SRC} ${QSTUDIOMETRICS_MOC_H} ${QSTUDIOMETRICS_UIS_H} ${icon_RCC_SRCS})
endif()

target_link_libraries(QStudioMetrics PRIVATE ${LIBSCIENTIFIC_LIBRARY} libchart ${QT6_LIBS})
set_target_properties(QStudioMetrics PROPERTIES ENABLE_EXPORTS ON)

# --- Plugin Discovery Mechanism ---
if(ENABLE_PLUGINS)
    set(DISCOVERY_BASE "")
    if(PLUGINS_PATH)
        set(DISCOVERY_BASE "${PLUGINS_PATH}/plugins")
    else()
        set(DISCOVERY_BASE "${CMAKE_CURRENT_SOURCE_DIR}/plugins")
    endif()

    if(EXISTS ${DISCOVERY_BASE})
        file(GLOB PLUGINS_DIRS RELATIVE ${DISCOVERY_BASE} ${DISCOVERY_BASE}/*)
        set(ALL_PLUGIN_TARGETS "")
        foreach(PLUGIN_DIR ${PLUGINS_DIRS})
            if(IS_DIRECTORY ${DISCOVERY_BASE}/${PLUGIN_DIR} AND EXISTS ${DISCOVERY_BASE}/${PLUGIN_DIR}/CMakeLists.txt)
                message(STATUS "Discovered plugin: ${PLUGIN_DIR}")
                # Use absolute paths and specific binary dir for external plugins
                add_subdirectory(${DISCOVERY_BASE}/${PLUGIN_DIR} ${CMAKE_CURRENT_BINARY_DIR}/plugins/${PLUGIN_DIR})
                file(READ ${DISCOVERY_BASE}/${PLUGIN_DIR}/CMakeLists.txt PLUGIN_CMAKE_CONTENT)
                string(REGEX MATCH "add_library\\s*\\(\\s*([a-zA-Z0-9_]+)" MATCH_VAR "${PLUGIN_CMAKE_CONTENT}")
                if(CMAKE_MATCH_1)
                    list(APPEND ALL_PLUGIN_TARGETS ${CMAKE_MATCH_1})
                endif()
            endif()
        endforeach()
        if(APPLE AND ALL_PLUGIN_TARGETS)
            add_custom_target(DeployPlugins ALL
                COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/src/QStudioMetrics.app/Contents/PlugIns/
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/bin/plugins/ ${CMAKE_BINARY_DIR}/src/QStudioMetrics.app/Contents/PlugIns/
                COMMENT "Deploying all plugins for macOS..."
            )
            add_dependencies(DeployPlugins ${ALL_PLUGIN_TARGETS} QStudioMetrics)
        endif()
    endif()
endif()
# ----------------------------------

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
  add_executable(testqsmdataops test_qsm_data_ops.cpp qstudiometricsdataoperations.cpp)
  target_link_libraries(testqsmdataops PRIVATE ${LIBSCIENTIFIC_LIBRARY} ${QT6_LIBS})
  add_test(NAME TestQSMDataOps COMMAND testqsmdataops)

  qt6_wrap_cpp(TEST_DATA_MOC qsmdata.h Dialogs/GenericProgressDialog.h)
  add_executable(testqsmdata 
      test_qsm_data.cpp 
      qsmdata.cpp 
      DATAIO.cpp 
      qstudiometricsdataoperations.cpp 
      PCA/pcamodel.cpp 
      PLS/plsmodel.cpp 
      MLR/mlrmodel.cpp 
      LDA/ldamodel.cpp 
      CPCA/cpcamodel.cpp 
      Dialogs/GenericProgressDialog.cpp 
      ${TEST_DATA_MOC}
      ${QSTUDIOMETRICS_UIS_H}
  )
  set_target_properties(testqsmdata PROPERTIES AUTOMOC OFF)
  target_link_libraries(testqsmdata PRIVATE ${LIBSCIENTIFIC_LIBRARY} ${QT6_LIBS} Qt6::Sql Qt6::Widgets)
  add_test(NAME TestQSMData COMMAND testqsmdata)

  add_executable(testrun 
      test_run.cpp 
      run.cpp 
      qsmdata.cpp 
      DATAIO.cpp 
      qstudiometricsdataoperations.cpp 
      PCA/pcamodel.cpp 
      PLS/plsmodel.cpp 
      MLR/mlrmodel.cpp 
      LDA/ldamodel.cpp 
      CPCA/cpcamodel.cpp 
      Dialogs/GenericProgressDialog.cpp 
      ${TEST_DATA_MOC}
      ${QSTUDIOMETRICS_UIS_H}
  )
  set_target_properties(testrun PROPERTIES AUTOMOC OFF)
  target_link_libraries(testrun PRIVATE ${LIBSCIENTIFIC_LIBRARY} ${QT6_LIBS} Qt6::Sql Qt6::Widgets)
  add_test(NAME TestRun COMMAND testrun)

  add_executable(testdataioutils test_dataio_utils.cpp DATAIO.cpp)

  target_link_libraries(testimportdvector ${LIBSCIENTIFIC_LIBRARY})
  target_link_libraries(testimportmatrix ${LIBSCIENTIFIC_LIBRARY})
  target_link_libraries(testimportarray ${LIBSCIENTIFIC_LIBRARY})
  target_link_libraries(testimportpcamodel ${LIBSCIENTIFIC_LIBRARY})
  target_link_libraries(testdataioutils PRIVATE ${LIBSCIENTIFIC_LIBRARY} ${QT6_LIBS})

  add_test(NAME TestImportDVector COMMAND testimportdvector)
  add_test(NAME TestImportMatrix COMMAND testimportmatrix)
  add_test(NAME TestImportArray COMMAND testimportarray)
  add_test(NAME TestImportPCAModel COMMAND testimportpcamodel)
  add_test(NAME TestDataIOUtils COMMAND testdataioutils)

  qt6_wrap_cpp(TEST_QSM_MOC Dialogs/GenericProgressDialog.h)
  add_executable(testqsmprojectio 
      test_qsm_project_io.cpp 
      qsmdata.cpp 
      DATAIO.cpp 
      qstudiometricsdataoperations.cpp
      PCA/pcamodel.cpp
      PLS/plsmodel.cpp
      MLR/mlrmodel.cpp
      LDA/ldamodel.cpp
      CPCA/cpcamodel.cpp
      Dialogs/GenericProgressDialog.cpp
      ${TEST_QSM_MOC}
      ${QSTUDIOMETRICS_UIS_H}
  )
  target_link_libraries(testqsmprojectio PRIVATE ${LIBSCIENTIFIC_LIBRARY} ${QT6_LIBS} Qt6::Sql Qt6::Widgets)
  add_test(NAME TestQSMProjectIO COMMAND testqsmprojectio)
endif()

if(LIBSCIENTIFIC_DEPENDENCY)
  set(_qsm_targets QStudioMetrics qsm-pca qsm-pls qsm-mlr qsm-mdc qsm-kmeans qsm-hgm)
  if(cmake_build_type_tolower STREQUAL "debug")
    list(APPEND _qsm_targets testimportdvector testimportmatrix testimportarray testimportpcamodel testqsmdataops testqsmdata testrun testdataioutils testqsmprojectio)
  endif()
  foreach(_target ${_qsm_targets})
    if(TARGET ${_target})
      add_dependencies(${_target} ${LIBSCIENTIFIC_DEPENDENCY})
    endif()
  endforeach()
endif()

if(APPLE)
    find_package(Qt6 COMPONENTS Core REQUIRED)
    get_target_property(_qmake_path Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_path}" DIRECTORY)
    
    add_custom_command(TARGET QStudioMetrics POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/src/QStudioMetrics.app/Contents/Frameworks/
        COMMAND ${CMAKE_COMMAND} -E copy ${LIBSCIENTIFIC_LIBRARY} ${CMAKE_BINARY_DIR}/src/QStudioMetrics.app/Contents/Frameworks/
        COMMENT "Deploying libraries for macOS..."
    )
    add_custom_target(RunMacDeployQt ALL
        COMMAND ${_qt_bin_dir}/macdeployqt ${CMAKE_BINARY_DIR}/src/QStudioMetrics.app -always-overwrite
        COMMENT "Deploying Qt for macOS (including plugins)..."
    )
    add_dependencies(RunMacDeployQt QStudioMetrics)
    if(TARGET DeployPlugins)
        add_dependencies(RunMacDeployQt DeployPlugins)
    endif()
endif()
