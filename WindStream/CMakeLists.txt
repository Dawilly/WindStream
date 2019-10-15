# CMakeList.txt : CMake project for WindStream, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
include_directories(AudioHandler)
include_directories(APIHandler)

set(AUDIOH AudioHandler)
set(APIH APIHandler)
set(SOURCES mainTest.c WindStream.c WindStream.h 
	${AUDIOH}/audiohandler.c ${AUDIOH}/audiohandler.h
	${APIH}/openal_apihandler.c ${APIH}/apihandler.h)

add_executable (WindStream ${SOURCES})

target_link_libraries(WindStream OpenAL)

# TODO: Add tests and install targets if needed.
