# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /usr/pkg/bin/cmake

# The command to remove a file.
RM = /usr/pkg/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/paisletimo/COMP308/comp308sheepSim/work

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/paisletimo/COMP308/comp308sheepSim/build

# Include any dependencies generated for this target.
include src/CMakeFiles/a3.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/a3.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/a3.dir/flags.make

src/CMakeFiles/a3.dir/main.cpp.o: src/CMakeFiles/a3.dir/flags.make
src/CMakeFiles/a3.dir/main.cpp.o: /home/paisletimo/COMP308/comp308sheepSim/work/src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paisletimo/COMP308/comp308sheepSim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/a3.dir/main.cpp.o"
	cd /home/paisletimo/COMP308/comp308sheepSim/build/src && /bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/a3.dir/main.cpp.o -c /home/paisletimo/COMP308/comp308sheepSim/work/src/main.cpp

src/CMakeFiles/a3.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a3.dir/main.cpp.i"
	cd /home/paisletimo/COMP308/comp308sheepSim/build/src && /bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paisletimo/COMP308/comp308sheepSim/work/src/main.cpp > CMakeFiles/a3.dir/main.cpp.i

src/CMakeFiles/a3.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a3.dir/main.cpp.s"
	cd /home/paisletimo/COMP308/comp308sheepSim/build/src && /bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paisletimo/COMP308/comp308sheepSim/work/src/main.cpp -o CMakeFiles/a3.dir/main.cpp.s

src/CMakeFiles/a3.dir/main.cpp.o.requires:

.PHONY : src/CMakeFiles/a3.dir/main.cpp.o.requires

src/CMakeFiles/a3.dir/main.cpp.o.provides: src/CMakeFiles/a3.dir/main.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/a3.dir/build.make src/CMakeFiles/a3.dir/main.cpp.o.provides.build
.PHONY : src/CMakeFiles/a3.dir/main.cpp.o.provides

src/CMakeFiles/a3.dir/main.cpp.o.provides.build: src/CMakeFiles/a3.dir/main.cpp.o


src/CMakeFiles/a3.dir/terrain.cpp.o: src/CMakeFiles/a3.dir/flags.make
src/CMakeFiles/a3.dir/terrain.cpp.o: /home/paisletimo/COMP308/comp308sheepSim/work/src/terrain.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paisletimo/COMP308/comp308sheepSim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/a3.dir/terrain.cpp.o"
	cd /home/paisletimo/COMP308/comp308sheepSim/build/src && /bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/a3.dir/terrain.cpp.o -c /home/paisletimo/COMP308/comp308sheepSim/work/src/terrain.cpp

src/CMakeFiles/a3.dir/terrain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a3.dir/terrain.cpp.i"
	cd /home/paisletimo/COMP308/comp308sheepSim/build/src && /bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paisletimo/COMP308/comp308sheepSim/work/src/terrain.cpp > CMakeFiles/a3.dir/terrain.cpp.i

src/CMakeFiles/a3.dir/terrain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a3.dir/terrain.cpp.s"
	cd /home/paisletimo/COMP308/comp308sheepSim/build/src && /bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paisletimo/COMP308/comp308sheepSim/work/src/terrain.cpp -o CMakeFiles/a3.dir/terrain.cpp.s

src/CMakeFiles/a3.dir/terrain.cpp.o.requires:

.PHONY : src/CMakeFiles/a3.dir/terrain.cpp.o.requires

src/CMakeFiles/a3.dir/terrain.cpp.o.provides: src/CMakeFiles/a3.dir/terrain.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/a3.dir/build.make src/CMakeFiles/a3.dir/terrain.cpp.o.provides.build
.PHONY : src/CMakeFiles/a3.dir/terrain.cpp.o.provides

src/CMakeFiles/a3.dir/terrain.cpp.o.provides.build: src/CMakeFiles/a3.dir/terrain.cpp.o


src/CMakeFiles/a3.dir/noise.cpp.o: src/CMakeFiles/a3.dir/flags.make
src/CMakeFiles/a3.dir/noise.cpp.o: /home/paisletimo/COMP308/comp308sheepSim/work/src/noise.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/paisletimo/COMP308/comp308sheepSim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/a3.dir/noise.cpp.o"
	cd /home/paisletimo/COMP308/comp308sheepSim/build/src && /bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/a3.dir/noise.cpp.o -c /home/paisletimo/COMP308/comp308sheepSim/work/src/noise.cpp

src/CMakeFiles/a3.dir/noise.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a3.dir/noise.cpp.i"
	cd /home/paisletimo/COMP308/comp308sheepSim/build/src && /bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/paisletimo/COMP308/comp308sheepSim/work/src/noise.cpp > CMakeFiles/a3.dir/noise.cpp.i

src/CMakeFiles/a3.dir/noise.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a3.dir/noise.cpp.s"
	cd /home/paisletimo/COMP308/comp308sheepSim/build/src && /bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/paisletimo/COMP308/comp308sheepSim/work/src/noise.cpp -o CMakeFiles/a3.dir/noise.cpp.s

src/CMakeFiles/a3.dir/noise.cpp.o.requires:

.PHONY : src/CMakeFiles/a3.dir/noise.cpp.o.requires

src/CMakeFiles/a3.dir/noise.cpp.o.provides: src/CMakeFiles/a3.dir/noise.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/a3.dir/build.make src/CMakeFiles/a3.dir/noise.cpp.o.provides.build
.PHONY : src/CMakeFiles/a3.dir/noise.cpp.o.provides

src/CMakeFiles/a3.dir/noise.cpp.o.provides.build: src/CMakeFiles/a3.dir/noise.cpp.o


# Object files for target a3
a3_OBJECTS = \
"CMakeFiles/a3.dir/main.cpp.o" \
"CMakeFiles/a3.dir/terrain.cpp.o" \
"CMakeFiles/a3.dir/noise.cpp.o"

# External object files for target a3
a3_EXTERNAL_OBJECTS =

bin/a3: src/CMakeFiles/a3.dir/main.cpp.o
bin/a3: src/CMakeFiles/a3.dir/terrain.cpp.o
bin/a3: src/CMakeFiles/a3.dir/noise.cpp.o
bin/a3: src/CMakeFiles/a3.dir/build.make
bin/a3: bin/libglew.a
bin/a3: bin/libglfw3.a
bin/a3: /lib64/libX11.so
bin/a3: /lib64/libXrandr.so
bin/a3: /lib64/libXinerama.so
bin/a3: /lib64/libXi.so
bin/a3: /lib64/libXxf86vm.so
bin/a3: /lib64/librt.so
bin/a3: /lib64/libm.so
bin/a3: /lib64/libXcursor.so
bin/a3: /lib64/libGL.so
bin/a3: bin/libstb.a
bin/a3: /lib64/libGLU.so
bin/a3: /lib64/libGL.so
bin/a3: src/CMakeFiles/a3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/paisletimo/COMP308/comp308sheepSim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../bin/a3"
	cd /home/paisletimo/COMP308/comp308sheepSim/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/a3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/a3.dir/build: bin/a3

.PHONY : src/CMakeFiles/a3.dir/build

src/CMakeFiles/a3.dir/requires: src/CMakeFiles/a3.dir/main.cpp.o.requires
src/CMakeFiles/a3.dir/requires: src/CMakeFiles/a3.dir/terrain.cpp.o.requires
src/CMakeFiles/a3.dir/requires: src/CMakeFiles/a3.dir/noise.cpp.o.requires

.PHONY : src/CMakeFiles/a3.dir/requires

src/CMakeFiles/a3.dir/clean:
	cd /home/paisletimo/COMP308/comp308sheepSim/build/src && $(CMAKE_COMMAND) -P CMakeFiles/a3.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/a3.dir/clean

src/CMakeFiles/a3.dir/depend:
	cd /home/paisletimo/COMP308/comp308sheepSim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/paisletimo/COMP308/comp308sheepSim/work /home/paisletimo/COMP308/comp308sheepSim/work/src /home/paisletimo/COMP308/comp308sheepSim/build /home/paisletimo/COMP308/comp308sheepSim/build/src /home/paisletimo/COMP308/comp308sheepSim/build/src/CMakeFiles/a3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/a3.dir/depend
