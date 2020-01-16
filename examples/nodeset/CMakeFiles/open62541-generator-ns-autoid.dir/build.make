# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/grigory/Abschlussprojekt/open62541-fork

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/grigory/Abschlussprojekt/open62541-fork

# Utility rule file for open62541-generator-ns-autoid.

# Include the progress variables for this target.
include examples/nodeset/CMakeFiles/open62541-generator-ns-autoid.dir/progress.make

examples/nodeset/CMakeFiles/open62541-generator-ns-autoid: src_generated/open62541/namespace_autoid_generated.c
examples/nodeset/CMakeFiles/open62541-generator-ns-autoid: src_generated/open62541/namespace_autoid_generated.h


src_generated/open62541/namespace_autoid_generated.c: tools/nodeset_compiler/nodeset_compiler.py
src_generated/open62541/namespace_autoid_generated.c: tools/nodeset_compiler/nodes.py
src_generated/open62541/namespace_autoid_generated.c: tools/nodeset_compiler/nodeset.py
src_generated/open62541/namespace_autoid_generated.c: tools/nodeset_compiler/datatypes.py
src_generated/open62541/namespace_autoid_generated.c: tools/nodeset_compiler/backend_open62541.py
src_generated/open62541/namespace_autoid_generated.c: tools/nodeset_compiler/backend_open62541_nodes.py
src_generated/open62541/namespace_autoid_generated.c: tools/nodeset_compiler/backend_open62541_datatypes.py
src_generated/open62541/namespace_autoid_generated.c: deps/ua-nodeset/AutoID/Opc.Ua.AutoID.NodeSet2.xml
src_generated/open62541/namespace_autoid_generated.c: deps/ua-nodeset/Schema/Opc.Ua.NodeSet2.xml
src_generated/open62541/namespace_autoid_generated.c: deps/ua-nodeset/DI/Opc.Ua.Di.NodeSet2.xml
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/grigory/Abschlussprojekt/open62541-fork/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ../../src_generated/open62541/namespace_autoid_generated.c, ../../src_generated/open62541/namespace_autoid_generated.h"
	cd /home/grigory/Abschlussprojekt/open62541-fork/examples/nodeset && /usr/bin/python /home/grigory/Abschlussprojekt/open62541-fork/tools/nodeset_compiler/nodeset_compiler.py --internal-headers --types-array=UA_TYPES --types-array=UA_TYPES_DI --types-array=UA_TYPES_AUTOID --existing=/home/grigory/Abschlussprojekt/open62541-fork/deps/ua-nodeset/Schema/Opc.Ua.NodeSet2.xml --existing=/home/grigory/Abschlussprojekt/open62541-fork/deps/ua-nodeset/DI/Opc.Ua.Di.NodeSet2.xml --xml=/home/grigory/Abschlussprojekt/open62541-fork/deps/ua-nodeset/AutoID/Opc.Ua.AutoID.NodeSet2.xml /home/grigory/Abschlussprojekt/open62541-fork/src_generated/open62541/namespace_autoid_generated

src_generated/open62541/namespace_autoid_generated.h: src_generated/open62541/namespace_autoid_generated.c
	@$(CMAKE_COMMAND) -E touch_nocreate src_generated/open62541/namespace_autoid_generated.h

open62541-generator-ns-autoid: examples/nodeset/CMakeFiles/open62541-generator-ns-autoid
open62541-generator-ns-autoid: src_generated/open62541/namespace_autoid_generated.c
open62541-generator-ns-autoid: src_generated/open62541/namespace_autoid_generated.h
open62541-generator-ns-autoid: examples/nodeset/CMakeFiles/open62541-generator-ns-autoid.dir/build.make

.PHONY : open62541-generator-ns-autoid

# Rule to build all files generated by this target.
examples/nodeset/CMakeFiles/open62541-generator-ns-autoid.dir/build: open62541-generator-ns-autoid

.PHONY : examples/nodeset/CMakeFiles/open62541-generator-ns-autoid.dir/build

examples/nodeset/CMakeFiles/open62541-generator-ns-autoid.dir/clean:
	cd /home/grigory/Abschlussprojekt/open62541-fork/examples/nodeset && $(CMAKE_COMMAND) -P CMakeFiles/open62541-generator-ns-autoid.dir/cmake_clean.cmake
.PHONY : examples/nodeset/CMakeFiles/open62541-generator-ns-autoid.dir/clean

examples/nodeset/CMakeFiles/open62541-generator-ns-autoid.dir/depend:
	cd /home/grigory/Abschlussprojekt/open62541-fork && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/grigory/Abschlussprojekt/open62541-fork /home/grigory/Abschlussprojekt/open62541-fork/examples/nodeset /home/grigory/Abschlussprojekt/open62541-fork /home/grigory/Abschlussprojekt/open62541-fork/examples/nodeset /home/grigory/Abschlussprojekt/open62541-fork/examples/nodeset/CMakeFiles/open62541-generator-ns-autoid.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/nodeset/CMakeFiles/open62541-generator-ns-autoid.dir/depend

