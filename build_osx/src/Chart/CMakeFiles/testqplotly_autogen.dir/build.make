# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.19.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.19.1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/marco/Seafile/Software/QStudioMetrics

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/marco/Seafile/Software/QStudioMetrics/build_osx

# Utility rule file for testqplotly_autogen.

# Include the progress variables for this target.
include src/Chart/CMakeFiles/testqplotly_autogen.dir/progress.make

src/Chart/CMakeFiles/testqplotly_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/marco/Seafile/Software/QStudioMetrics/build_osx/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target testqplotly"
	cd /Users/marco/Seafile/Software/QStudioMetrics/build_osx/src/Chart && /usr/local/Cellar/cmake/3.19.1/bin/cmake -E cmake_autogen /Users/marco/Seafile/Software/QStudioMetrics/build_osx/src/Chart/CMakeFiles/testqplotly_autogen.dir/AutogenInfo.json Debug

testqplotly_autogen: src/Chart/CMakeFiles/testqplotly_autogen
testqplotly_autogen: src/Chart/CMakeFiles/testqplotly_autogen.dir/build.make

.PHONY : testqplotly_autogen

# Rule to build all files generated by this target.
src/Chart/CMakeFiles/testqplotly_autogen.dir/build: testqplotly_autogen

.PHONY : src/Chart/CMakeFiles/testqplotly_autogen.dir/build

src/Chart/CMakeFiles/testqplotly_autogen.dir/clean:
	cd /Users/marco/Seafile/Software/QStudioMetrics/build_osx/src/Chart && $(CMAKE_COMMAND) -P CMakeFiles/testqplotly_autogen.dir/cmake_clean.cmake
.PHONY : src/Chart/CMakeFiles/testqplotly_autogen.dir/clean

src/Chart/CMakeFiles/testqplotly_autogen.dir/depend:
	cd /Users/marco/Seafile/Software/QStudioMetrics/build_osx && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/marco/Seafile/Software/QStudioMetrics /Users/marco/Seafile/Software/QStudioMetrics/src/Chart /Users/marco/Seafile/Software/QStudioMetrics/build_osx /Users/marco/Seafile/Software/QStudioMetrics/build_osx/src/Chart /Users/marco/Seafile/Software/QStudioMetrics/build_osx/src/Chart/CMakeFiles/testqplotly_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/Chart/CMakeFiles/testqplotly_autogen.dir/depend

