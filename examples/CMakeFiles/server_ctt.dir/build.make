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
CMAKE_COMMAND = /home/grigory/Programs/CLion/clion-2019.2.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/grigory/Programs/CLion/clion-2019.2.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/grigory/Abschlussprojekt/open62541-fork

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/grigory/Abschlussprojekt/open62541-fork

# Include any dependencies generated for this target.
include examples/CMakeFiles/server_ctt.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/server_ctt.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/server_ctt.dir/flags.make

examples/CMakeFiles/server_ctt.dir/server_ctt.c.o: examples/CMakeFiles/server_ctt.dir/flags.make
examples/CMakeFiles/server_ctt.dir/server_ctt.c.o: examples/server_ctt.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/grigory/Abschlussprojekt/open62541-fork/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/CMakeFiles/server_ctt.dir/server_ctt.c.o"
	cd /home/grigory/Abschlussprojekt/open62541-fork/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server_ctt.dir/server_ctt.c.o   -c /home/grigory/Abschlussprojekt/open62541-fork/examples/server_ctt.c

examples/CMakeFiles/server_ctt.dir/server_ctt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server_ctt.dir/server_ctt.c.i"
	cd /home/grigory/Abschlussprojekt/open62541-fork/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/grigory/Abschlussprojekt/open62541-fork/examples/server_ctt.c > CMakeFiles/server_ctt.dir/server_ctt.c.i

examples/CMakeFiles/server_ctt.dir/server_ctt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server_ctt.dir/server_ctt.c.s"
	cd /home/grigory/Abschlussprojekt/open62541-fork/examples && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/grigory/Abschlussprojekt/open62541-fork/examples/server_ctt.c -o CMakeFiles/server_ctt.dir/server_ctt.c.s

# Object files for target server_ctt
server_ctt_OBJECTS = \
"CMakeFiles/server_ctt.dir/server_ctt.c.o"

# External object files for target server_ctt
server_ctt_EXTERNAL_OBJECTS =

bin/examples/server_ctt: examples/CMakeFiles/server_ctt.dir/server_ctt.c.o
bin/examples/server_ctt: examples/CMakeFiles/server_ctt.dir/build.make
bin/examples/server_ctt: bin/libopen62541.a
bin/examples/server_ctt: examples/CMakeFiles/server_ctt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/grigory/Abschlussprojekt/open62541-fork/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ../bin/examples/server_ctt"
	cd /home/grigory/Abschlussprojekt/open62541-fork/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server_ctt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/server_ctt.dir/build: bin/examples/server_ctt

.PHONY : examples/CMakeFiles/server_ctt.dir/build

examples/CMakeFiles/server_ctt.dir/clean:
	cd /home/grigory/Abschlussprojekt/open62541-fork/examples && $(CMAKE_COMMAND) -P CMakeFiles/server_ctt.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/server_ctt.dir/clean

examples/CMakeFiles/server_ctt.dir/depend:
	cd /home/grigory/Abschlussprojekt/open62541-fork && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/grigory/Abschlussprojekt/open62541-fork /home/grigory/Abschlussprojekt/open62541-fork/examples /home/grigory/Abschlussprojekt/open62541-fork /home/grigory/Abschlussprojekt/open62541-fork/examples /home/grigory/Abschlussprojekt/open62541-fork/examples/CMakeFiles/server_ctt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/server_ctt.dir/depend
