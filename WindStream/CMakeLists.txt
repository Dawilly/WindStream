# CMakeList.txt : CMake project for WindStream, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
include_directories(FileHandler)
include_directories(APIHandler)

set(FILEH FileHandler)
set(APIH APIHandler)
set(SOURCES mainTest.c WindStream.c WindStream.h 
	${FILEH}/filehandler.c ${FILEH}/filehandler.h
	${APIH}/openal_apihandler.c ${APIH}/apihandler.h)

add_executable (WindStream ${SOURCES})

target_link_libraries(WindStream OpenAL)

# TODO: Add tests and install targets if needed.
