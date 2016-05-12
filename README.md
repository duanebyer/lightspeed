# Lightspeed
### Duane Byer

## Overview

This project provides a simple visualization of the effects of special
relativity. Currently, length contraction, time dilation, and light travel time
are all accounted for. In the future, other possible features may include:

  * Textures and the Doppler effect for light
  * Automatic subdivision of geometry to allow for straight edges to be
    transformed more accurately
  * Clocks and geometry that internally changes over time

To run the software, OpenGL 4.30 must be supported.

## Building

The project can be built using CMake. The following libraries are required:

  * OpenGL
  * GLFW
  * [entityx](https://github.com/alecthomas/entityx)

## How it works

The history (position vs time) of every object in the scene is stored. Then,
each frame, the entire history is transformed using a Lorentz boost into the
frame of the player. The intersection of a null cone (centered on the player)
with the history of the object is calculated. The intersection point is then
rendered to the screen using OpenGL, resulting in a visual representation of the
object as it would be seen by the observer.

## Images
---
![](https://raw.githubusercontent.com/duanebyer/lightspeed/master/images/image_0.png)

