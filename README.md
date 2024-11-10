# QStudioMetrics

![Page views](https://visitor-badge.glitch.me/badge?page_id=gmrandazzo.QStudioMetrics&left_color=green&right_color=red)
![GitHub all releases](https://img.shields.io/github/downloads/gmrandazzo/QStudioMetrics/total)
[![CodeQL](https://github.com/gmrandazzo/QStudioMetrics/actions/workflows/codeql.yml/badge.svg)](https://github.com/gmrandazzo/QStudioMetrics/actions/workflows/codeql.yml)

**QStudioMetrics is a software to develop data mining and multivariate analysis studies.**

Discoveries often arise from exploratory data analysis and come from a hypothesis-free manner
alongside hypothesis-driven approaches. QStudioMetrics is a toolbox that allows a conversation between you,
the data, and the hypotheses. It will enable you to explore and query the data and integrate your personal experience for new discoveries.

![ScreenShot](https://github.com/gmrandazzo/QStudioMetrics/blob/master/vars/qsmgui.png)

QStudioMetrics is written in C++ using the Qt5 framework and run under Linux, Windows and OSX.

QStudioMetrics compute:

- Principal Component Analysis (PCA)
- Consensus Principal Component Analysis (CPCA)
- Partial Least Squares (PLS)
- Multiple Linear Regression (MLR)
- Linear Discriminant Analysis (LDA)
- Clustering analysis: Hierarchical and K-Means
- Model validation: leave-one-out, bootstrap k-fold cross validation, y-scrambling

All the algorithms are part of libscientific (See https://github.com/gmrandazzo/libscientific)

**Author: Giuseppe Marco Randazzo <br/>
Mantainer: Giuseppe Marco Randazzo, gmrandazzo@gmail.com <br/>**

## License

QStudioMetrics is distributed under LGPLv3 license.
For more details please read the file "LICENSE" or go to "http://www.gnu.org/licenses/lgpl-3.0.html"

## Install

## Dependencies

- c++ compiler
- cmake >= 3.22
- Qt6
- libscientific >= 1.4.x

## Compile from source

OSX using homebrew

```
brew install libscientific
brew install qt
brew install cmake g++
cmake ~/Nextcloud/Software/QStudioMetrics/ -Wno-dev -DCMAKE_PREFIX_PATH=$(brew --prefix qt) -DLIBSCIENTIFIC_ROOT_DIR=$(brew --prefix libscientific)
make -j
```

## Install binary release

Binary releases for Windows and OSX at <https://github.com/gmrandazzo/QStudioMetrics/releases>.

## Documentation

https://qstudiometrics.readthedocs.io/en/latest/

## Supported platforms

- Linux
- Windows
- Mac intel/silicon

## Known bugs

- Hierarchical clustering: the problem belongs to the original libscientific library
