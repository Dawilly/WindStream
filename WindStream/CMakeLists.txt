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

set(SOURCES 
	Test.c
	WindStream.c 
	WindStream.h 
	helpers.c
	helpers.h
	${BASE}/definitions.h

	${APIH}/apihandler.h
	${APIH}/apihandler.c
	${APIH}/openal_apihandler.h
	${APIH}/openal_apihandler.c 
	
	${THREADH}/threadhandler.h
	${THREADH}/win32_threadhandler.c 
	${THREADH}/win32_threadhandler.h
	
	${FILEH}/filehandler.h 
	${FILEH}/wave_filehandler.h 
	${FILEH}/wave_filehandler.c)

add_executable (WindStream ${SOURCES})

# TODO: Add tests and install targets if needed.
