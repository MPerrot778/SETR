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
CMAKE_SOURCE_DIR = /home/mikael/projets/SETR/src/daemonFuse

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mikael/projets/SETR/src/daemonFuse/build

# Include any dependencies generated for this target.
include CMakeFiles/tp2client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tp2client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tp2client.dir/flags.make

CMakeFiles/tp2client.dir/setrfs.c.o: CMakeFiles/tp2client.dir/flags.make
CMakeFiles/tp2client.dir/setrfs.c.o: ../setrfs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mikael/projets/SETR/src/daemonFuse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/tp2client.dir/setrfs.c.o"
	/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/bin/arm-raspbian-linux-gnueabihf-gcc --sysroot=/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/arm-raspbian-linux-gnueabihf/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tp2client.dir/setrfs.c.o -c /home/mikael/projets/SETR/src/daemonFuse/setrfs.c

CMakeFiles/tp2client.dir/setrfs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tp2client.dir/setrfs.c.i"
	/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/bin/arm-raspbian-linux-gnueabihf-gcc --sysroot=/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/arm-raspbian-linux-gnueabihf/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mikael/projets/SETR/src/daemonFuse/setrfs.c > CMakeFiles/tp2client.dir/setrfs.c.i

CMakeFiles/tp2client.dir/setrfs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tp2client.dir/setrfs.c.s"
	/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/bin/arm-raspbian-linux-gnueabihf-gcc --sysroot=/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/arm-raspbian-linux-gnueabihf/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mikael/projets/SETR/src/daemonFuse/setrfs.c -o CMakeFiles/tp2client.dir/setrfs.c.s

CMakeFiles/tp2client.dir/fstools.c.o: CMakeFiles/tp2client.dir/flags.make
CMakeFiles/tp2client.dir/fstools.c.o: ../fstools.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mikael/projets/SETR/src/daemonFuse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/tp2client.dir/fstools.c.o"
	/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/bin/arm-raspbian-linux-gnueabihf-gcc --sysroot=/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/arm-raspbian-linux-gnueabihf/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tp2client.dir/fstools.c.o -c /home/mikael/projets/SETR/src/daemonFuse/fstools.c

CMakeFiles/tp2client.dir/fstools.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tp2client.dir/fstools.c.i"
	/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/bin/arm-raspbian-linux-gnueabihf-gcc --sysroot=/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/arm-raspbian-linux-gnueabihf/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mikael/projets/SETR/src/daemonFuse/fstools.c > CMakeFiles/tp2client.dir/fstools.c.i

CMakeFiles/tp2client.dir/fstools.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tp2client.dir/fstools.c.s"
	/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/bin/arm-raspbian-linux-gnueabihf-gcc --sysroot=/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/arm-raspbian-linux-gnueabihf/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mikael/projets/SETR/src/daemonFuse/fstools.c -o CMakeFiles/tp2client.dir/fstools.c.s

CMakeFiles/tp2client.dir/communications.c.o: CMakeFiles/tp2client.dir/flags.make
CMakeFiles/tp2client.dir/communications.c.o: ../communications.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mikael/projets/SETR/src/daemonFuse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/tp2client.dir/communications.c.o"
	/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/bin/arm-raspbian-linux-gnueabihf-gcc --sysroot=/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/arm-raspbian-linux-gnueabihf/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tp2client.dir/communications.c.o -c /home/mikael/projets/SETR/src/daemonFuse/communications.c

CMakeFiles/tp2client.dir/communications.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tp2client.dir/communications.c.i"
	/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/bin/arm-raspbian-linux-gnueabihf-gcc --sysroot=/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/arm-raspbian-linux-gnueabihf/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/mikael/projets/SETR/src/daemonFuse/communications.c > CMakeFiles/tp2client.dir/communications.c.i

CMakeFiles/tp2client.dir/communications.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tp2client.dir/communications.c.s"
	/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/bin/arm-raspbian-linux-gnueabihf-gcc --sysroot=/home/mikael/arm-cross-comp-env/arm-raspbian-linux-gnueabihf/arm-raspbian-linux-gnueabihf/sysroot $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/mikael/projets/SETR/src/daemonFuse/communications.c -o CMakeFiles/tp2client.dir/communications.c.s

# Object files for target tp2client
tp2client_OBJECTS = \
"CMakeFiles/tp2client.dir/setrfs.c.o" \
"CMakeFiles/tp2client.dir/fstools.c.o" \
"CMakeFiles/tp2client.dir/communications.c.o"

# External object files for target tp2client
tp2client_EXTERNAL_OBJECTS =

tp2client: CMakeFiles/tp2client.dir/setrfs.c.o
tp2client: CMakeFiles/tp2client.dir/fstools.c.o
tp2client: CMakeFiles/tp2client.dir/communications.c.o
tp2client: CMakeFiles/tp2client.dir/build.make
tp2client: CMakeFiles/tp2client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mikael/projets/SETR/src/daemonFuse/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable tp2client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tp2client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tp2client.dir/build: tp2client

.PHONY : CMakeFiles/tp2client.dir/build

CMakeFiles/tp2client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tp2client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tp2client.dir/clean

CMakeFiles/tp2client.dir/depend:
	cd /home/mikael/projets/SETR/src/daemonFuse/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mikael/projets/SETR/src/daemonFuse /home/mikael/projets/SETR/src/daemonFuse /home/mikael/projets/SETR/src/daemonFuse/build /home/mikael/projets/SETR/src/daemonFuse/build /home/mikael/projets/SETR/src/daemonFuse/build/CMakeFiles/tp2client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tp2client.dir/depend

