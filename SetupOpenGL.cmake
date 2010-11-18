INCLUDE(${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
IF (NOT OPENGL_FOUND) 
  MESSAGE ("WARNING: Could not find OpenGL - depending targets will be disabled.")
  SET(OE_MISSING_LIBS "${OE_MISSING_LIBS}, OpenGL")
ENDIF (NOT OPENGL_FOUND)

INCLUDE(${OE_CURRENT_EXTENSION_DIR}/FindGLEW.cmake)
IF (GLEW_FOUND) 
  INCLUDE_DIRECTORIES(${GLEW_INCLUDE_DIR})
ELSE (GLEW_FOUND)
  MESSAGE ("WARNING: Could not find OpenGL extentions (GLEW) - depending targets will be disabled.")
  SET(OE_MISSING_LIBS "${OE_MISSING_LIBS}, GLEW")
ENDIF (GLEW_FOUND)
