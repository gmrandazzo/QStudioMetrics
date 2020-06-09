QStudioMetrics
==============

**QStudioMetrics is a software to develop data mining and multivariate analysis studies.**

![ScreenShot](https://github.com/gmrandazzo/QStudioMetrics/blob/master/vars/qsmgui.png)

QStudioMetrics is written in C++ using the Qt5 framework and run under Linux, Windows and OSX.

QStudioMetrics compute:
  - Principal Component Analysis (PCA)
  - Partial Least Squares (PLS)
  - Ensemble Partial Least Squares (EPLS)
  - Multiple Linear Regression (MLR)
  - Linear Discriminant Analysis (LDA)
  - Clustering analysis: Hierarchical and K-Means
  - Model validation: leave-one-out, bootstrap k-fold cross validation, y-scrambling


All the algorithms are part of libscientific (See https://github.com/gmrandazzo/libscientific)

**Author: Giuseppe Marco Randazzo <br/>
Mantainer: Giuseppe Marco Randazzo, gmrandazzo@gmail.com <br/>**

License
-------

QStudioMetrics is distributed under LGPLv3 license.
For more details please read the file "LICENSE" or go to "http://www.gnu.org/licenses/lgpl-3.0.html"

Install
-------

## Dependencies
- c++ compiler
- cmake >= 2.8.9
- Qt5 (with WebKit or WebEngine)
- libscientific > 0.8

## Compile from source

If you have a recent qt version and QWebEngine support use:

```
cmake .. -Wno-dev -DCMAKE_PREFIX_PATH=/usr/local/Cellar/qt/5.11.1/ -DLIBSCIENTIFIC_ROOT_DIR=/usr/local/Cellar/libscientific/HEAD-b4a8a4f/
make
```

For other qt versions with WebKit support pleas use this configuration:

```
cmake .. -Wno-dev -DCMAKE_PREFIX_PATH=/usr/local/Cellar/qt/5.11.1/ -DLIBSCIENTIFIC_ROOT_DIR=/usr/local/Cellar/libscientific/HEAD-b4a8a4f/ -DPLOT_ENGINE=WEBKIT
make
```

## Install binary release

Binary releases for Windows and OSX at <https://github.com/gmrandazzo/QStudioMetrics/releases>.

## Acknowledgments

[Plotly Javascript](https://plotly.com/javascript/) - QStudioMetrics use plot.js as plot engine.
 


