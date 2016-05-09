#ifndef __LIGHTSPEED_PLAYER_COMPONENT_H_
#define __LIGHTSPEED_PLAYER_COMPONENT_H_

namespace lightspeed {

struct PlayerComponent final {
  
  PlayerComponent(
      int forwardKey,
      int backwardKey,
      int leftKey,
      int rightKey,
      double sensitivity,
      double acceleration) :
      sensitivity(sensitivity),
      acceleration(acceleration),
      yaw(0.0),
      pitch(0.0),
      forwardKey(forwardKey),
      backwardKey(backwardKey),
      leftKey(leftKey),
      rightKey(rightKey),
      forwardKeyPressed(false),
      backwardKeyPressed(false),
      leftKeyPressed(false),
      rightKeyPressed(false) {
  }
  
  double sensitivity;
  double acceleration;
  
  double yaw;
  double pitch;
  
  int forwardKey;
  int backwardKey;
  int leftKey;
  int rightKey;
  
  bool forwardKeyPressed;
  bool backwardKeyPressed;
  bool leftKeyPressed;
  bool rightKeyPressed;
  
};

}

#endif

