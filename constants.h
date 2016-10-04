#ifdef __APPLE__
#include <OpenGL/gl.h>
#endif
#ifdef __linux__
#include <GL/gl.h>
#endif
const GLuint WIDTH = 1900, HEIGHT = 1000;
const GLuint POSITION_WIDTH = 200;
const GLuint POSITION_HEIGHT = 200;
const GLuint POSITION_DEPTH = 200;
const GLuint HALF_POSITION_WIDTH = POSITION_WIDTH/2;
const GLint DEPTH_OFFSET = -1;
const GLuint TOTAL_POSITIONS = POSITION_HEIGHT* POSITION_WIDTH*POSITION_DEPTH;
const GLuint CUBE_DIMENSION = 3;
