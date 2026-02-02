# QStudioMetrics

![GitHub all releases](https://img.shields.io/github/downloads/gmrandazzo/QStudioMetrics/total)
[![CodeQL](https://github.com/gmrandazzo/QStudioMetrics/actions/workflows/codeql.yml/badge.svg)](https://github.com/gmrandazzo/QStudioMetrics/actions/workflows/codeql.yml)
[![License: AGPL v3](https://img.shields.io/badge/License-AGPL_v3-blue.svg)](https://opensource.org/licenses/AGPL-3.0)

**QStudioMetrics** is a high-performance C++ toolbox for exploratory data analysis, chemometrics, and multivariate statistics. It provides a bridge between complex algorithms and intuitive discovery, allowing researchers to interact with their data without writing a single line of code.

![ScreenShot](https://github.com/gmrandazzo/QStudioMetrics/blob/master/vars/qsmgui.png)

---

## 🚀 Key Features

- **Interactive Exploratory Analysis**: Real-time plotting and manipulation of PCA and CPCA models.
- **Advanced Regression**: Comprehensive support for PLS and MLR with automated validation.
- **Robust Classification**: Linear Discriminant Analysis (LDA) with high-accuracy results.
- **Validation Suite**: Built-in K-Fold cross-validation, Leave-One-Out, and Y-Scrambling to ensure statistical significance.
- **High Performance**: Native C++20 engine powered by `libscientific` for handling large datasets efficiently.
- **Cross-Platform**: Seamlessly runs on Linux, Windows, and macOS (Intel/Silicon).

---

## 🍇 Quick Start: The Wine Challenge

Discover how to separate wine cultivars in 30 seconds:

1.  **Import**: Launch QStudioMetrics and go to `File > Import Data`. Select `examples/wine.csv`.
2.  **Model**: Navigate to `Models > PCA`. Select all chemical variables (Alcohol, Malic acid, etc.) and click `Compute`.
3.  **Discover**: Open the `Scores Plot`. You will see three distinct clusters representing different wine cultivars based on their chemical profile!

---

## 🛠 Installation & Build

### Dependencies

- **C++ Compiler**: Supporting C++20 (GCC 10+, Clang 10+, MSVC 2019+)
- **CMake**: >= 3.22
- **Qt6**: Core, Core5Compat, Gui, Widgets, Sql, PrintSupport, Concurrent, Charts
- **libscientific**: (Automatically downloaded during build if not found)
- **Pandoc & LaTeX**: (Optional) Required only for building the PDF/HTML documentation

### Compile from source

#### macOS (Homebrew)
```bash
brew install cmake qt qt5compat
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$(brew --prefix qt)
make -j
```

#### Linux (Debian/Ubuntu)
```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev libqt6charts6-dev libqt6core5compat6-dev
mkdir build && cd build
cmake ..
make -j
```

#### Linux (Fedora)
```bash
sudo dnf install gcc-c++ cmake qt6-qtbase-devel qt6-qtcharts-devel qt6-qt5compat-devel
mkdir build && cd build
cmake ..
make -j
```

---

## 📚 Documentation

Detailed documentation, including statistical interpretations and plot guides, is available in the `docs` folder.

### Install Documentation Tools

#### macOS
```bash
brew install pandoc
# For PDF support:
brew install --cask mactex-no-gui
```

#### Linux (Debian/Ubuntu)
```bash
sudo apt install pandoc texlive-latex-base texlive-fonts-recommended texlive-extra-utils texlive-latex-extra
```

#### Linux (Fedora)
```bash
sudo dnf install pandoc texlive-scheme-basic
```

### Build Commands
```bash
cd docs
make html   # Generates build/index.html
make pdf    # Generates build/MANUAL.pdf
make all    # Generates both
```

---

## 🎓 How to Cite

If you use QStudioMetrics in your research, please cite it to support the project:

> Randazzo, G. M. QStudioMetrics: A C++ Toolbox for Multivariate Analysis. GitHub Repository. https://github.com/gmrandazzo/QStudioMetrics

---

## 📜 License

QStudioMetrics is distributed under the **GNU Affero General Public License Version 3**.
See the [LICENSE](LICENSE) file for the full text.

---

**Author**: Giuseppe Marco Randazzo  
**Contact**: [gmrandazzo@gmail.com](mailto:gmrandazzo@gmail.com)
