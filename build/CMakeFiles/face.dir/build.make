# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/antoine/dev/c++Appli/rest/API_swapFace

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/antoine/dev/c++Appli/rest/API_swapFace/build

# Include any dependencies generated for this target.
include CMakeFiles/face.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/face.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/face.dir/flags.make

CMakeFiles/face.dir/src/FaceSwapper.cpp.o: CMakeFiles/face.dir/flags.make
CMakeFiles/face.dir/src/FaceSwapper.cpp.o: ../src/FaceSwapper.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/antoine/dev/c++Appli/rest/API_swapFace/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/face.dir/src/FaceSwapper.cpp.o"
	/bin/g++-8  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/face.dir/src/FaceSwapper.cpp.o -c /home/antoine/dev/c++Appli/rest/API_swapFace/src/FaceSwapper.cpp

CMakeFiles/face.dir/src/FaceSwapper.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/face.dir/src/FaceSwapper.cpp.i"
	/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/antoine/dev/c++Appli/rest/API_swapFace/src/FaceSwapper.cpp > CMakeFiles/face.dir/src/FaceSwapper.cpp.i

CMakeFiles/face.dir/src/FaceSwapper.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/face.dir/src/FaceSwapper.cpp.s"
	/bin/g++-8 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/antoine/dev/c++Appli/rest/API_swapFace/src/FaceSwapper.cpp -o CMakeFiles/face.dir/src/FaceSwapper.cpp.s

# Object files for target face
face_OBJECTS = \
"CMakeFiles/face.dir/src/FaceSwapper.cpp.o"

# External object files for target face
face_EXTERNAL_OBJECTS =

face: CMakeFiles/face.dir/src/FaceSwapper.cpp.o
face: CMakeFiles/face.dir/build.make
face: /usr/local/lib/libopencv_stitching.so.4.3.0
face: /usr/local/lib/libopencv_alphamat.so.4.3.0
face: /usr/local/lib/libopencv_aruco.so.4.3.0
face: /usr/local/lib/libopencv_bgsegm.so.4.3.0
face: /usr/local/lib/libopencv_bioinspired.so.4.3.0
face: /usr/local/lib/libopencv_ccalib.so.4.3.0
face: /usr/local/lib/libopencv_dnn_objdetect.so.4.3.0
face: /usr/local/lib/libopencv_dnn_superres.so.4.3.0
face: /usr/local/lib/libopencv_dpm.so.4.3.0
face: /usr/local/lib/libopencv_face.so.4.3.0
face: /usr/local/lib/libopencv_freetype.so.4.3.0
face: /usr/local/lib/libopencv_fuzzy.so.4.3.0
face: /usr/local/lib/libopencv_hdf.so.4.3.0
face: /usr/local/lib/libopencv_hfs.so.4.3.0
face: /usr/local/lib/libopencv_img_hash.so.4.3.0
face: /usr/local/lib/libopencv_intensity_transform.so.4.3.0
face: /usr/local/lib/libopencv_line_descriptor.so.4.3.0
face: /usr/local/lib/libopencv_quality.so.4.3.0
face: /usr/local/lib/libopencv_rapid.so.4.3.0
face: /usr/local/lib/libopencv_reg.so.4.3.0
face: /usr/local/lib/libopencv_rgbd.so.4.3.0
face: /usr/local/lib/libopencv_saliency.so.4.3.0
face: /usr/local/lib/libopencv_sfm.so.4.3.0
face: /usr/local/lib/libopencv_stereo.so.4.3.0
face: /usr/local/lib/libopencv_structured_light.so.4.3.0
face: /usr/local/lib/libopencv_superres.so.4.3.0
face: /usr/local/lib/libopencv_surface_matching.so.4.3.0
face: /usr/local/lib/libopencv_tracking.so.4.3.0
face: /usr/local/lib/libopencv_videostab.so.4.3.0
face: /usr/local/lib/libopencv_viz.so.4.3.0
face: /usr/local/lib/libopencv_xfeatures2d.so.4.3.0
face: /usr/local/lib/libopencv_xobjdetect.so.4.3.0
face: /usr/local/lib/libopencv_xphoto.so.4.3.0
face: /usr/local/lib/libopencv_highgui.so.4.3.0
face: /usr/local/lib/libopencv_shape.so.4.3.0
face: /usr/local/lib/libopencv_datasets.so.4.3.0
face: /usr/local/lib/libopencv_plot.so.4.3.0
face: /usr/local/lib/libopencv_text.so.4.3.0
face: /usr/local/lib/libopencv_dnn.so.4.3.0
face: /usr/local/lib/libopencv_ml.so.4.3.0
face: /usr/local/lib/libopencv_phase_unwrapping.so.4.3.0
face: /usr/local/lib/libopencv_optflow.so.4.3.0
face: /usr/local/lib/libopencv_ximgproc.so.4.3.0
face: /usr/local/lib/libopencv_video.so.4.3.0
face: /usr/local/lib/libopencv_videoio.so.4.3.0
face: /usr/local/lib/libopencv_imgcodecs.so.4.3.0
face: /usr/local/lib/libopencv_objdetect.so.4.3.0
face: /usr/local/lib/libopencv_calib3d.so.4.3.0
face: /usr/local/lib/libopencv_features2d.so.4.3.0
face: /usr/local/lib/libopencv_flann.so.4.3.0
face: /usr/local/lib/libopencv_photo.so.4.3.0
face: /usr/local/lib/libopencv_imgproc.so.4.3.0
face: /usr/local/lib/libopencv_core.so.4.3.0
face: CMakeFiles/face.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/antoine/dev/c++Appli/rest/API_swapFace/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable face"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/face.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/face.dir/build: face

.PHONY : CMakeFiles/face.dir/build

CMakeFiles/face.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/face.dir/cmake_clean.cmake
.PHONY : CMakeFiles/face.dir/clean

CMakeFiles/face.dir/depend:
	cd /home/antoine/dev/c++Appli/rest/API_swapFace/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antoine/dev/c++Appli/rest/API_swapFace /home/antoine/dev/c++Appli/rest/API_swapFace /home/antoine/dev/c++Appli/rest/API_swapFace/build /home/antoine/dev/c++Appli/rest/API_swapFace/build /home/antoine/dev/c++Appli/rest/API_swapFace/build/CMakeFiles/face.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/face.dir/depend

