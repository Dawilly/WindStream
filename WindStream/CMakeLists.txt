# CMakeList.txt : CMake project for WindStream, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable
include_directories(APIHandler)
include_directories(FileHandler)
include_directories(ThreadHandler)

set(BASE .)
set(FILEH FileHandler)
set(APIH APIHandler)
set(THREADH ThreadHandler)

set(SOURCES mainTest.c WindStream.c WindStream.h 
	${BASE}/definitions.h
	${APIH}/apihandler.h
	${APIH}/openal_apihandler.c 
	
	${THREADH}/threadhandler.h
	${THREADH}/win32_threadhandler.c 
	${THREADH}/win32_threadhandler.h
	
	${FILEH}/filehandler.h 
	${FILEH}/wav_filestructure.h
	${FILEH}/wav_filehandler.h 
	${FILEH}/wav_filehandler.c)

add_executable (WindStream ${SOURCES})

target_link_libraries(WindStream OpenAL)

# TODO: Add tests and install targets if needed.
