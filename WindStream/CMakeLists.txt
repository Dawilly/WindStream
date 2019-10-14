# CMakeList.txt : CMake project for WindStream, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
include_directories(AudioHandler)

set(AAPI AudioHandler)
set(SOURCES WindStream.c WindStream.h 
	${AAPI}/audiohandler.c ${AAPI}/audiohandler.h)

add_executable (WindStream ${SOURCES})

target_link_libraries(WindStream OpenAL)

# TODO: Add tests and install targets if needed.
