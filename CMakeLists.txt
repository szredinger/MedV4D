CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(VERSION 2.8)
PROJECT( M4DViewer )

if(POLICY CMP0012)
	cmake_policy(SET CMP0012 NEW)
endif(POLICY CMP0012)
if(POLICY CMP0011)
	cmake_policy(SET CMP0011 NEW)
endif(POLICY CMP0011)

IF(NOT MEDV4D_ROOT_DIRECTORY)
	SET(MEDV4D_ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/MedV4D" )
ENDIF(NOT MEDV4D_ROOT_DIRECTORY)

IF(NOT MEDV4D_CMAKE_MODULES_DIR )
	SET(MEDV4D_CMAKE_MODULES_DIR ${MEDV4D_ROOT_DIRECTORY}/../cmake_project/Modules)
	SET(CMAKE_MODULE_PATH "${MEDV4D_CMAKE_MODULES_DIR}" "${CMAKE_ROOT}/Modules" )
ENDIF(NOT MEDV4D_CMAKE_MODULES_DIR )


SET( MEDV4D_ROOT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/MedV4D )
SET( MEDV4D_LIB_OUTPUT_DIRECTORY ${MEDV4D_ROOT_DIRECTORY}/lib )
SET( MEDV4D_EXECUTABLES_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/executables )


INCLUDE( "${MEDV4D_CMAKE_MODULES_DIR}/SetupOutputPrefixAndPostfix.cmake" )

ADD_SUBDIRECTORY( MedV4D )
ADD_SUBDIRECTORY( Applications/Viewer )
#ADD_SUBDIRECTORY( Applications/tools )
ADD_DEPENDENCIES( Viewer MedV4D )
#ADD_DEPENDENCIES( CudaFilters MedV4D )
