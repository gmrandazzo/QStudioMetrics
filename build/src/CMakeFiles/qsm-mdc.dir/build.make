# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.15.5/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.15.5/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/marco/projects/QStudioMetrics

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/marco/projects/QStudioMetrics/build

# Include any dependencies generated for this target.
include src/CMakeFiles/qsm-mdc.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/qsm-mdc.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/qsm-mdc.dir/flags.make

src/CMakeFiles/qsm-mdc.dir/DATAIO.cpp.o: src/CMakeFiles/qsm-mdc.dir/flags.make
src/CMakeFiles/qsm-mdc.dir/DATAIO.cpp.o: ../src/DATAIO.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/marco/projects/QStudioMetrics/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/qsm-mdc.dir/DATAIO.cpp.o"
	cd /Users/marco/projects/QStudioMetrics/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/qsm-mdc.dir/DATAIO.cpp.o -c /Users/marco/projects/QStudioMetrics/src/DATAIO.cpp

src/CMakeFiles/qsm-mdc.dir/DATAIO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/qsm-mdc.dir/DATAIO.cpp.i"
	cd /Users/marco/projects/QStudioMetrics/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/marco/projects/QStudioMetrics/src/DATAIO.cpp > CMakeFiles/qsm-mdc.dir/DATAIO.cpp.i

src/CMakeFiles/qsm-mdc.dir/DATAIO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/qsm-mdc.dir/DATAIO.cpp.s"
	cd /Users/marco/projects/QStudioMetrics/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/marco/projects/QStudioMetrics/src/DATAIO.cpp -o CMakeFiles/qsm-mdc.dir/DATAIO.cpp.s

src/CMakeFiles/qsm-mdc.dir/qstudiom-mdc.cpp.o: src/CMakeFiles/qsm-mdc.dir/flags.make
src/CMakeFiles/qsm-mdc.dir/qstudiom-mdc.cpp.o: ../src/qstudiom-mdc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/marco/projects/QStudioMetrics/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/qsm-mdc.dir/qstudiom-mdc.cpp.o"
	cd /Users/marco/projects/QStudioMetrics/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/qsm-mdc.dir/qstudiom-mdc.cpp.o -c /Users/marco/projects/QStudioMetrics/src/qstudiom-mdc.cpp

src/CMakeFiles/qsm-mdc.dir/qstudiom-mdc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/qsm-mdc.dir/qstudiom-mdc.cpp.i"
	cd /Users/marco/projects/QStudioMetrics/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/marco/projects/QStudioMetrics/src/qstudiom-mdc.cpp > CMakeFiles/qsm-mdc.dir/qstudiom-mdc.cpp.i

src/CMakeFiles/qsm-mdc.dir/qstudiom-mdc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/qsm-mdc.dir/qstudiom-mdc.cpp.s"
	cd /Users/marco/projects/QStudioMetrics/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/marco/projects/QStudioMetrics/src/qstudiom-mdc.cpp -o CMakeFiles/qsm-mdc.dir/qstudiom-mdc.cpp.s

# Object files for target qsm-mdc
qsm__mdc_OBJECTS = \
"CMakeFiles/qsm-mdc.dir/DATAIO.cpp.o" \
"CMakeFiles/qsm-mdc.dir/qstudiom-mdc.cpp.o"

# External object files for target qsm-mdc
qsm__mdc_EXTERNAL_OBJECTS =

src/qsm-mdc: src/CMakeFiles/qsm-mdc.dir/DATAIO.cpp.o
src/qsm-mdc: src/CMakeFiles/qsm-mdc.dir/qstudiom-mdc.cpp.o
src/qsm-mdc: src/CMakeFiles/qsm-mdc.dir/build.make
src/qsm-mdc: /usr/local/lib/libscientific.dylib
src/qsm-mdc: src/CMakeFiles/qsm-mdc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/marco/projects/QStudioMetrics/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable qsm-mdc"
	cd /Users/marco/projects/QStudioMetrics/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/qsm-mdc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/qsm-mdc.dir/build: src/qsm-mdc

.PHONY : src/CMakeFiles/qsm-mdc.dir/build

src/CMakeFiles/qsm-mdc.dir/clean:
	cd /Users/marco/projects/QStudioMetrics/build/src && $(CMAKE_COMMAND) -P CMakeFiles/qsm-mdc.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/qsm-mdc.dir/clean

src/CMakeFiles/qsm-mdc.dir/depend:
	cd /Users/marco/projects/QStudioMetrics/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/marco/projects/QStudioMetrics /Users/marco/projects/QStudioMetrics/src /Users/marco/projects/QStudioMetrics/build /Users/marco/projects/QStudioMetrics/build/src /Users/marco/projects/QStudioMetrics/build/src/CMakeFiles/qsm-mdc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/qsm-mdc.dir/depend
