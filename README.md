# Lightspeed
### Duane Byer

## Overview
===========
This project provides a simple visualization of the effects of special
relativity. Currently, length contraction, time dilation, and light travel time
are all accounted for. In the future, other possible features may include:

  * Textures and the Doppler effect for light
  * Automatic subdivision of geometry to allow for straight edges to be
    transformed more accurately
  * Clocks and geometry that internally changes over time

To run the software, OpenGL 4.30 must be supported.

## Building
===========
The project can be built using CMake. The following libraries are required:

  * OpenGL
  * GLFW
  * [entityx](https://github.com/alecthomas/entityx)

