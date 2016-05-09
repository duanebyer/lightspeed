#ifndef __LIGHTSPEED_VERTEX_H_
#define __LIGHTSPEED_VERTEX_H_

#include <internal/opengl.h>

namespace lightspeed {

/**
 * \brief Stores the data associated with a single vertex.
 */
struct Vertex final {
  
  GLfloat x;
  GLfloat y;
  GLfloat z;
  
  GLuint textureIndex;
  
  GLfloat u;
  GLfloat v;
  
};

}

#endif

