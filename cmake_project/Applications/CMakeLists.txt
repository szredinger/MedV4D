CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(MEDV4D_APPLICATIONS)

IF(NOT MEDV4D_CMAKE_SOURCE_DIR)
	SET(MEDV4D_CMAKE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/.." )
	SET(MEDV4D_CMAKE_MODULES_DIR ${MEDV4D_CMAKE_SOURCE_DIR}/Modules)
	SET(CMAKE_MODULE_PATH "${MEDV4D_CMAKE_MODULES_DIR}" "${CMAKE_ROOT}/Modules" )
ENDIF(NOT MEDV4D_CMAKE_SOURCE_DIR)

ADD_SUBDIRECTORY(m4dPilot)
ADD_SUBDIRECTORY(OrganSegmentation)
ADD_SUBDIRECTORY(ExampleApplications)


