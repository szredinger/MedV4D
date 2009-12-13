# Windows only

FIND_PATH(CHAI3D_INCLUDE_DIR chai3d.h $ENV{CHAI3D_ROOT}/src)
FIND_PATH(CHAI3D_GLUT_INCLUDE_DIR gl/glut.h $ENV{CHAI3D_ROOT}/external/OpenGL/msvc)
FIND_PATH(CHAI3D_LIBRARY NAMES chai3d-debug.lib PATHS $ENV{CHAI3D_ROOT}/lib/msvc9) 
FIND_PATH(CHAI3D_GLUT_LIBRARY NAMES glut32.lib PATHS $ENV{CHAI3D_ROOT}/external/OpenGL/msvc)

IF (CHAI3D_INCLUDE_DIR AND CHAI3D_LIBRARY AND CHAI3D_GLUT_INCLUDE_DIR AND CHAI3D_GLUT_LIBRARY)
   SET(CHAI3D_FOUND TRUE)
ENDIF (CHAI3D_INCLUDE_DIR AND CHAI3D_LIBRARY AND CHAI3D_GLUT_INCLUDE_DIR AND CHAI3D_GLUT_LIBRARY)

IF (CHAI3D_FOUND)
   IF (NOT CHAI3D_FIND_QUIETLY)
      MESSAGE(STATUS "Found Chai3D - ${CHAI3D_LIBRARY}")
   ENDIF (NOT CHAI3D_FIND_QUIETLY)
   
   INCLUDE_DIRECTORIES( ${CHAI3D_INCLUDE_DIR} ${CHAI3D_GLUT_INCLUDE_DIR} )
   LINK_DIRECTORIES( ${CHAI3D_LIBRARY} ${CHAI3D_GLUT_LIBRARY} )
   TARGET_LINK_LIBRARIES( ${OUTPUT_NAME} ${CHAI3D_LIBRARY}/chai3d-debug.lib )
ELSE (CHAI3D_FOUND)
   IF (CHAI3D_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Chai3D - try to set environment variable CHAI3D_ROOT to chai3d dir.")
   ENDIF (CHAI3D_FIND_REQUIRED)
ENDIF (CHAI3D_FOUND)
