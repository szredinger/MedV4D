#
# nešlo vygenerovať
#
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

ADD_DEFINITIONS("-std=c++0x")

PROJECT ( Hmira )

IF(NOT MEDV4D_ROOT_DIRECTORY)
        SET(MEDV4D_ROOT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../MedV4D" )
ENDIF(NOT MEDV4D_ROOT_DIRECTORY)

IF(NOT MEDV4D_CMAKE_MODULES_DIR )
        SET(MEDV4D_CMAKE_MODULES_DIR ${MEDV4D_ROOT_DIRECTORY}/../cmake_project/Modules)
        SET(CMAKE_MODULE_PATH "${MEDV4D_CMAKE_MODULES_DIR}" "${CMAKE_ROOT}/Modules" )
ENDIF(NOT MEDV4D_CMAKE_MODULES_DIR )

FIND_PACKAGE( MedV4D REQUIRED )

SET( HMIRA_INCLUDE_DIRECTORY "include" )

FIND_LIBRARY( OPENMESH_CORE_DEBUG_LIB OpenMeshCored ) #PATHS "usr/local/lib/OpenMesh")
FIND_LIBRARY( OPENMESH_CORE_RELEASE_LIB OpenMeshCore )
SET( OPENMESH_CORE_LIB "debug" ${OPENMESH_CORE_DEBUG_LIB} "debug" ${OPENMESH_CORE_DEBUG_LIB} )

FIND_PATH( OPENMESH_INCLUDE_DIR "OpenMesh/Core/Geometry/VectorT.hh" )

INCLUDE_DIRECTORIES(${MEDV4D_INCLUDE_DIR} ${MEDV4D_INCLUDE_DIRS_3RD_PARTY} ${HMIRA_INCLUDE_DIRECTORY} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${OPENMESH_INCLUDE_DIR} )

INCLUDE( "${MEDV4D_CMAKE_MODULES_DIR}/StandardCompileOptions.cmake" NO_POLICY_SCOPE )

ADD_EXECUTABLE( main "main.cpp" ${HMIRA_HEADERS} ${EXTENSION_HEADERS} )

MESSAGE( "${MEDV4D_EXECUTABLE_SOURCES}" )
MESSAGE( "${MEDV4D_EXECUTABLE_HEADERS}" )
MESSAGE( "${MEDV4D_LIBRARIES}" )


TARGET_LINK_LIBRARIES( main 
	${OPENMESH_CORE_LIB} 
	#${MEDV4D_LIBRARIES} 
	#${MEDV4D_LIBRARIES_3RD_PARTY} 
	)

SUBDIRS( ${HMIRA_INCLUDE_DIRECTORY} )
