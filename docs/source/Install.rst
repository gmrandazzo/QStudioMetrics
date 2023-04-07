Install
=======

The installation works for Linux/macOS and Windows.

Requirements
------------

* A development environment (On windows msys/msys2 or visual studio)
* A c/c++/fortran compiler
* Cmake
* python3 (if you whant to use the library in python)


Installation from source
------------------------

First you need to install libscientific. Please follow the instruction `here https://libscientific.readthedocs.io/en/latest/`
Then follow the instructions below

.. code-block::
   git clone https://github.com/gmrandazzo/QStudioMetrics.git
   cd QStudioMetrics
   mkdir build
   cd build
   cmake -DCMAKE_BUILD_TYPE=Release -DLIBSCIENTIFIC_ROOT_DIR=<PATH LIBSCIENTIFIC> ..
   make -jX "
   cd src
   ./QStudioMetrics

Binary distributions
--------------------

Binary distributions available at https://github.com/gmrandazzo/QStudioMetrics/releases


