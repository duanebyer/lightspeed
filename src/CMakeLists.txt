cmake_minimum_required(VERSION 2.8)
set(
  SOURCES
  main.cpp
  quaternion.cpp
  vector.cpp
  system/acceleration_system.cpp
  system/movement_system.cpp
  system/player_system.cpp
  system/relativistic_update_system.cpp
  system/render_system.cpp
)

add_executable(lightspeed ${SOURCES})

find_package(PkgConfig REQUIRED)

pkg_search_module(EntityX REQUIRED entityx)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(X11 REQUIRED)
find_package(Threads REQUIRED)

include_directories(
  ${LightSpeed_SOURCE_DIR}/include
  ${EntityX_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS}
  ${X11_X11_INCLUDE_PATH}
)

target_link_libraries(
  lightspeed
  ${EntityX_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${X11_X11_LIB}
  ${X11_Xxf86vm_LIB}
  ${X11_Xrandr_LIB}
  ${X11_Xi_LIB}
  ${X11_Xcursor_LIB}
  ${X11_Xinerama_LIB}
  ${CMAKE_THREAD_LIBS_INIT}
  ${CMAKE_DL_LIBS}
)

