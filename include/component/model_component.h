#ifndef __LIGHTSPEED_MODEL_COMPONENT_H_
#define __LIGHTSPEED_MODEL_COMPONENT_H_

#include <initializer_list>

#include "vertex.h"
#include "texture.h"

namespace lightspeed {

struct ModelComponent final {
  
  ModelComponent() :
      vertices(),
      textures() {
  }
  
  ModelComponent(
      std::initializer_list<Vertex> vertices,
      std::initializer_list<Texture*> textures) :
      vertices(vertices),
      textures(textures) {
  }
  
  std::vector<Vertex> vertices;
  std::vector<Texture*> textures;
  
};

}

#endif

