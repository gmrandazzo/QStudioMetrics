# LibChart - High-Performance Qt 2D Plotting Library

**LibChart** is a highly optimized C++ plotting library based on Qt 6. It is designed to render massive datasets (1,000,000+ points) in real-time with fluid interactivity (60 FPS), making it ideal for scientific visualization and data analysis tools.

## Key Features

*   **High Performance:** Capable of rendering over 1 million scatter points without UI freezing or lag.
*   **Optimized Rendering:**
    *   **Spatial Decimation:** Uses a grid-based hashing system to prevent overdraw of overlapping points, drastically reducing draw calls.
    *   **Level of Detail (LOD):** Implements pixel-distance filtering for line curves to reduce vertex count while maintaining visual fidelity.
    *   **Marker Caching:** Pre-renders scatter markers into cached `QPixmap`s to avoid expensive per-point rasterization.
*   **Advanced Interactivity:**
    *   **Lasso Selection:** Freeform selection of points using a lasso tool.
    *   **Rubber Band Zooming:** Intuitive zoom functionality using standard rect selection.
    *   **Instant Hover:** Uses spatial indexing (binary search on sorted data) to provide immediate tooltips for the nearest point, even in dense datasets.
*   **Dual Backend Architecture:**
    *   **Native Qt:** Custom `QPainter` based engine for maximum control and speed (`Chart`).
    *   **QtCharts:** Wrapper around standard QtCharts (`ChartQt`).
    *   *(Deprecated)* **Web Integration:** Legacy support for Plotly/Cytoscape via web views.

## User Interaction & Controls

| Action | Mouse/Key Input | Description |
| :--- | :--- | :--- |
| **Lasso Selection** | **Left Click + Drag** | Draw a freeform shape to select points inside the region. |
| **Zoom In** | **Middle Click + Drag** | Draw a rectangle to zoom into that specific area. |
| **Hover Tooltip** | **Mouse Move** | Move cursor over points to see their Name, X, and Y values instantly. |
| **Scroll / Pan** | **Mouse Wheel** | Scroll vertically (or mapped axis) to pan the view. |
| **Cancel / Reset** | **Right Click** | Cancel the current selection or zoom operation. |
| **Zoom Steps** | **`+` / `-` Keys** | Step through the zoom history stack. |
| **Pan Arrows** | **Arrow Keys** | Pan the view left, right, up, or down. |

## Architecture & Optimizations

The library uses a generic `Graphs` interface to abstract the plotting engine. The core implementation (`Chart`) achieves its performance through several techniques:

1.  **Render Loop:** Instead of standard Qt Graphics View Framework (which creates heavy objects for every item), `Chart` is a simple `QWidget` that paints directly to a `QPixmap` buffer.
2.  **Grid Decimation:** When drawing scatter plots, the canvas is logically divided into small grid cells (e.g., 2x2 pixels). If a point has already been drawn in a cell, subsequent points falling into that same cell are skipped. This effectively limits the number of drawn items to the screen resolution, not the dataset size.
3.  **Spatial Indexing:** Points are maintained in a sorted structure allowing `O(log N)` lookups. This is crucial for the hover functionality, checking only a narrow strip of data around the mouse cursor instead of iterating the entire dataset.

## Build Instructions

### Dependencies
*   **Qt 6** (Core, Gui, Widgets, PrintSupport, Charts)
*   **CMake** (3.22 or later)
*   **C++17** compatible compiler

### Compilation

```bash
mkdir build
cd build
cmake ..
make
```

### Running Tests

To verify the performance with a generated 50,000 point dataset:

```bash
./testchart
```

## Usage Example

```cpp
#include "chart.h"

// Create the chart widget
Chart *chart = new Chart(parentWidget);

// Add data points
for (int i = 0; i < 10000; ++i) {
    chart->addPoint(x[i], y[i], "Point " + QString::number(i));
}

// Configure axes
chart->setXaxisName("Time (s)");
chart->setYaxisName("Amplitude");
chart->setPlotTitle("Signal Analysis");

// Display
chart->show();
chart->Refresh();
```

## License

This project is licensed under the **GNU Affero General Public License v3 (AGPL-3.0)**. See the `LICENSE` file for details.