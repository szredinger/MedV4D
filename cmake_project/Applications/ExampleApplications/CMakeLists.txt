
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT( ExampleApplications )

IF(NOT MEDV4D_CMAKE_SOURCE_DIR)
	SET(MEDV4D_CMAKE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/../.." )
	SET(MEDV4D_CMAKE_MODULES_DIR ${MEDV4D_CMAKE_SOURCE_DIR}/Modules)
	SET(CMAKE_MODULE_PATH "${MEDV4D_CMAKE_MODULES_DIR}" "${CMAKE_ROOT}/Modules" )
ENDIF(NOT MEDV4D_CMAKE_SOURCE_DIR)

INCLUDE( "${MEDV4D_CMAKE_MODULES_DIR}/Functions.cmake" )
INCLUDE( "${MEDV4D_CMAKE_MODULES_DIR}/FindPackages.cmake" )
INCLUDE( "${MEDV4D_CMAKE_MODULES_DIR}/StandardCompileOptions.cmake" )

#Mandatory -- prevents adding targets for Medv4DLibs more than once 
IF(NOT MEDV4D_LIBS_CMAKE_PROCESSED)
	ADD_SUBDIRECTORY( "${MEDV4D_CMAKE_SOURCE_DIR}/Medv4DLibs" "${MEDV4D_CMAKE_SOURCE_DIR}/../tmp" )
	#Must propagate to parent scope
	SET( MEDV4D_LIBS_CMAKE_PROCESSED TRUE PARENT_SCOPE)
ENDIF(NOT MEDV4D_LIBS_CMAKE_PROCESSED)

SET(EXECUTABLE_OUTPUT_PATH ${MEDV4D_CMAKE_SOURCE_DIR}/../executables)
TARGET_MEDV4D_PROGRAM( BoneSegmentation ${MEDV4D_CMAKE_SOURCE_DIR}/../Applications/BoneSegmentation )
TARGET_MEDV4D_PROGRAM( SimpleMIP ${MEDV4D_CMAKE_SOURCE_DIR}/../Applications/SimpleMIP )

