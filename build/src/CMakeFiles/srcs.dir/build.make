# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bing/CPP/Tiny_STL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bing/CPP/Tiny_STL/build

# Include any dependencies generated for this target.
include src/CMakeFiles/srcs.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/srcs.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/srcs.dir/flags.make

src/CMakeFiles/srcs.dir/test.cpp.o: src/CMakeFiles/srcs.dir/flags.make
src/CMakeFiles/srcs.dir/test.cpp.o: ../src/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bing/CPP/Tiny_STL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/srcs.dir/test.cpp.o"
	cd /home/bing/CPP/Tiny_STL/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/srcs.dir/test.cpp.o -c /home/bing/CPP/Tiny_STL/src/test.cpp

src/CMakeFiles/srcs.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/srcs.dir/test.cpp.i"
	cd /home/bing/CPP/Tiny_STL/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bing/CPP/Tiny_STL/src/test.cpp > CMakeFiles/srcs.dir/test.cpp.i

src/CMakeFiles/srcs.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/srcs.dir/test.cpp.s"
	cd /home/bing/CPP/Tiny_STL/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bing/CPP/Tiny_STL/src/test.cpp -o CMakeFiles/srcs.dir/test.cpp.s

src/CMakeFiles/srcs.dir/test.cpp.o.requires:

.PHONY : src/CMakeFiles/srcs.dir/test.cpp.o.requires

src/CMakeFiles/srcs.dir/test.cpp.o.provides: src/CMakeFiles/srcs.dir/test.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/srcs.dir/build.make src/CMakeFiles/srcs.dir/test.cpp.o.provides.build
.PHONY : src/CMakeFiles/srcs.dir/test.cpp.o.provides

src/CMakeFiles/srcs.dir/test.cpp.o.provides.build: src/CMakeFiles/srcs.dir/test.cpp.o


# Object files for target srcs
srcs_OBJECTS = \
"CMakeFiles/srcs.dir/test.cpp.o"

# External object files for target srcs
srcs_EXTERNAL_OBJECTS =

src/libsrcs.a: src/CMakeFiles/srcs.dir/test.cpp.o
src/libsrcs.a: src/CMakeFiles/srcs.dir/build.make
src/libsrcs.a: src/CMakeFiles/srcs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bing/CPP/Tiny_STL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libsrcs.a"
	cd /home/bing/CPP/Tiny_STL/build/src && $(CMAKE_COMMAND) -P CMakeFiles/srcs.dir/cmake_clean_target.cmake
	cd /home/bing/CPP/Tiny_STL/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/srcs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/srcs.dir/build: src/libsrcs.a

.PHONY : src/CMakeFiles/srcs.dir/build

src/CMakeFiles/srcs.dir/requires: src/CMakeFiles/srcs.dir/test.cpp.o.requires

.PHONY : src/CMakeFiles/srcs.dir/requires

src/CMakeFiles/srcs.dir/clean:
	cd /home/bing/CPP/Tiny_STL/build/src && $(CMAKE_COMMAND) -P CMakeFiles/srcs.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/srcs.dir/clean

src/CMakeFiles/srcs.dir/depend:
	cd /home/bing/CPP/Tiny_STL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bing/CPP/Tiny_STL /home/bing/CPP/Tiny_STL/src /home/bing/CPP/Tiny_STL/build /home/bing/CPP/Tiny_STL/build/src /home/bing/CPP/Tiny_STL/build/src/CMakeFiles/srcs.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/srcs.dir/depend
