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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mikael/projets/SETR

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mikael/projets/SETR/build

# Include any dependencies generated for this target.
include CMakeFiles/SETR.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SETR.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SETR.dir/flags.make

CMakeFiles/SETR.dir/SETR.cpp.o: CMakeFiles/SETR.dir/flags.make
CMakeFiles/SETR.dir/SETR.cpp.o: ../SETR.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mikael/projets/SETR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SETR.dir/SETR.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SETR.dir/SETR.cpp.o -c /home/mikael/projets/SETR/SETR.cpp

CMakeFiles/SETR.dir/SETR.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SETR.dir/SETR.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mikael/projets/SETR/SETR.cpp > CMakeFiles/SETR.dir/SETR.cpp.i

CMakeFiles/SETR.dir/SETR.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SETR.dir/SETR.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mikael/projets/SETR/SETR.cpp -o CMakeFiles/SETR.dir/SETR.cpp.s

# Object files for target SETR
SETR_OBJECTS = \
"CMakeFiles/SETR.dir/SETR.cpp.o"

# External object files for target SETR
SETR_EXTERNAL_OBJECTS =

libSETR.a: CMakeFiles/SETR.dir/SETR.cpp.o
libSETR.a: CMakeFiles/SETR.dir/build.make
libSETR.a: CMakeFiles/SETR.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mikael/projets/SETR/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libSETR.a"
	$(CMAKE_COMMAND) -P CMakeFiles/SETR.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SETR.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SETR.dir/build: libSETR.a

.PHONY : CMakeFiles/SETR.dir/build

CMakeFiles/SETR.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SETR.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SETR.dir/clean

CMakeFiles/SETR.dir/depend:
	cd /home/mikael/projets/SETR/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mikael/projets/SETR /home/mikael/projets/SETR /home/mikael/projets/SETR/build /home/mikael/projets/SETR/build /home/mikael/projets/SETR/build/CMakeFiles/SETR.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SETR.dir/depend

