# Changelog

## [2.3.0] - 2022-12-20

New QStudioMetrics release that support multiblock analysis.

### Added
- Add consensus PCA multiblock analysis

### Changed
- Integration of the new libscientific >= 1.4.0
- Fixed initializations 

## [2.2.0] - 2022-11-04

New QStudioMetrics release!
Speed and stability improved!

### Changed
- Integration of the new libscientific >= 1.3.0
- Fixed some bugs in the interface
- Fixed some bugs on PLS-DA plots
- Fixed YScrambling

## [2.1.1] - 2022-10-09

New QStudioMetrics release for Windows and OSX x86_64

### Changed
- Fixed Crash on windows machines.
- Fixed malfunctioning ROC and Precision-Recall AUC vs LV plot for PLSDA

## [2.1.0] - 2022-09-23

New QStudioMetrics release for Windows and OSX x86_64

### Added
- Added LDA support for multi-class analysis. (WARNING: method to be furthermore tested)

### Changed
- Removed plotly support and 3D plot support in general.
- Introduction of QChart plot while maintaining the old-fast plot library.
- Solved several bugs in the interface (special thanks to @gernot-hochmuth) and improved the usability in general
- Upgraded libscientific to the latest version 1.2.7
    

## [2.0.3] - 2021-05-01

### Added
- Added support to modify with custom text/size the "axis name, title on 2D plots"

### Changed
- Fixed 'QByteArray& QByteArray::append(const QString&)' is deprecated from dircompressor.cpp
- Fixed column header assign to variable name by [@gernot-hochmuth](https://github.com/gmrandazzo/QStudioMetrics/pull/2)
- Fixed Data Merge "Advanced pretreatment" [issue](https://github.com/gmrandazzo/QStudioMetrics/issues/3)
- Fixed warning: format 'size_t' from %lu to %llu in qstudiom-pls.cpp
- Fixed deprecated functions into chart.cpp

## [2.0.2] - 2019-03-14
### Added
- Added a new Chart library (v1.2.1) wich plot in 2D using Qt and 3D using plotly.
- Added more symbols to 2D and 3D plot

### Changed
- Fixed minor bugs related to the selection by matrix value


## [2.0.1] - 2019-04-14

### Added
- Added a new Chart library (v1.2.1) wich plot in 2D using Qt and 3D using plotly.
- Added more symbols to 2D and 3D plot

### Changed
- Fixed minor bugs related to the selection by matrix value

## [2.0.0] - 2019-02-28

### Changed
- First official binary release for windows 64bit users
- First official binary release for OSX Mojave


## [1.8.0] - 2016-09-27
### Added
- Source code release


## [0.0.1] - 2011
### Added
- Starting the QStudioMetrics development at University of Perugia
