# QStudioMetrics

![Page views](https://visitor-badge.glitch.me/badge?page_id=gmrandazzo.QStudioMetrics)
![GitHub all releases](https://img.shields.io/github/downloads/gmrandazzo/QStudioMetrics/total)
[![CodeQL](https://github.com/gmrandazzo/QStudioMetrics/actions/workflows/codeql.yml/badge.svg)](https://github.com/gmrandazzo/QStudioMetrics/actions/workflows/codeql.yml)

**QStudioMetrics is a software to develop data mining and multivariate analysis studies.**

Discoveries often arise from exploratory data analysis and come from a hypothesis-free manner
alongside hypothesis-driven approaches. QStudioMetrics is a toolbox that allows a conversation between you,
the data, and the hypotheses. It will enable you to explore and query the data and integrate your personal experience for new discoveries.

![ScreenShot](https://github.com/gmrandazzo/QStudioMetrics/blob/master/vars/qsmgui.png)

QStudioMetrics is written in C++ using the Qt6 framework and run under Linux, Windows and OSX.

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

QStudioMetrics is distributed under GNU AFFERO GENERAL PUBLIC LICENSE VERSION 3.
For more details please read the file "LICENSE".

## Install

## Dependencies

- C++ compiler (supporting C++20)
- CMake >= 3.22
- Qt6 (Core, Core5Compat, Gui, Widgets, Sql, PrintSupport, Concurrent, Charts)
- libscientific (automatically downloaded if not found)

## Compile from source

### macOS (Homebrew)

```bash
brew install cmake qt qt5compat
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt)
make -j
```

### Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev libqt6charts6-dev libqt6core5compat6-dev
mkdir build
cd build
cmake ..
make -j
```

### Linux (Fedora)

```bash
sudo dnf install gcc-c++ cmake qt6-qtbase-devel qt6-qtcharts-devel qt6-qt5compat-devel
mkdir build
cd build
cmake ..
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
