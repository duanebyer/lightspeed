#ifndef __LIGHTSPEED_PLAYER_COMPONENT_H_
#define __LIGHTSPEED_PLAYER_COMPONENT_H_

namespace lightspeed {

struct PlayerComponent final {
  
  PlayerComponent(
      int forwardKey,
      int backwardKey,
      int leftKey,
      int rightKey,
      int upKey,
      int downKey,
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
      upKey(upKey),
      downKey(downKey),
      forwardKeyPressed(false),
      backwardKeyPressed(false),
      leftKeyPressed(false),
      rightKeyPressed(false),
      upKeyPressed(false),
      downKeyPressed(false) {
  }
  
  double sensitivity;
  double acceleration;
  
  double yaw;
  double pitch;
  
  int forwardKey;
  int backwardKey;
  int leftKey;
  int rightKey;
  int upKey;
  int downKey;
  
  bool forwardKeyPressed;
  bool backwardKeyPressed;
  bool leftKeyPressed;
  bool rightKeyPressed;
  bool upKeyPressed;
  bool downKeyPressed;
  
};

}

#endif

