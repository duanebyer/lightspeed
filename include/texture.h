#ifndef __LIGHTSPEED_TEXTURE_H_
#define __LIGHTSPEED_TEXTURE_H_

#include <string>

namespace lightspeed {

/**
 * \brief Stores the raw RGB data of a texture.
 * 
 * This class cannot be copied in any way. It should be passed by reference or
 * by pointer.
 */
class Texture final {
  
public:
  
  /**
   * \brief Loads the Texture from a file.
   */
  Texture(std::string fileName);
  Texture(Texture const&) = delete;
  void operator=(Texture const&) = delete;
  
private:
  
};

}

#endif

