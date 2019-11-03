# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("WindStream")

# Include sub-projects.
add_subdirectory ("WindStream")
add_subdirectory ("OpenAL")
add_subdirectory ("Ogg")
# add_subdirectory ("Flac")
# add_subdirectory ("Vorbis")
