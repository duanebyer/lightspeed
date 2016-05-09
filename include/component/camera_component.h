#ifndef __LIGHTSPEED_CAMERA_COMPONENT_H_
#define __LIGHTSPEED_CAMERA_COMPONENT_H_

#include <cmath>

namespace lightspeed {

struct CameraComponent final {
  
  CameraComponent() :
      fov(M_PI / 4.0),
      aspectRatio(1.0),
      clipNear(0.1),
      clipFar(100.0) {
  }
  
  CameraComponent(
      double fov,
      double aspectRatio,
      double clipNear,
      double clipFar) :
      fov(fov),
      aspectRatio(aspectRatio),
      clipNear(clipNear),
      clipFar(clipFar) {
  }
  
  double fov;
  double aspectRatio;
  double clipNear;
  double clipFar;
  
};

}

#endif

