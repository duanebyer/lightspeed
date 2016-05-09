#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>

#include <entityx/entityx.h>

#include "internal/opengl.h"

#include "component/acceleration_component.h"
#include "component/body_component.h"
#include "component/camera_component.h"
#include "component/model_component.h"
#include "component/player_component.h"
#include "component/timeline_component.h"

#include "event/finalize_event.h"
#include "event/initialize_event.h"
#include "event/keyboard_event.h"
#include "event/mouse_button_event.h"
#include "event/mouse_position_event.h"
#include "event/render_event.h"

#include "system/acceleration_system.h"
#include "system/movement_system.h"
#include "system/player_system.h"
#include "system/render_system.h"

#include "texture.h"
#include "vector.h"
#include "vertex.h"

#define RESULT_SUCCESS (0)
#define RESULT_FAILURE (-1)

using namespace lightspeed;

// Variables to do with the entity component system framework.
entityx::EventManager events;
entityx::EntityManager entities(events);
entityx::SystemManager systems(entities, events);

// Functions that set up the scene.
void createScene();
void createPlayer();
void createBox(Vector position, Vector dimensions);

// Event handling functions.
void onGlfwError(int error, char const* description);
void onGlError(int error);

void onInitialize(GLFWwindow* window);
void onFinalize(GLFWwindow* window);
void onUpdate(GLFWwindow* window, double delta);
void onRender(GLFWwindow* window, int viewportWidth, int viewportHeight);
void onMousePosition(GLFWwindow* window, double xpos, double ypos);
void onMouseButton(GLFWwindow* window, int button, int action, int mods);
void onKeyboard(
  GLFWwindow* window,
  int key,
  int scancode,
  int action,
  int mods);

int main(int argc, char** argv) {
  
  // Initialize GLFW.
  if (!glfwInit()) {
    std::cerr << "GLFW failed to initialize." << '\n';
    exit(RESULT_FAILURE);
  }
  
  // Set the error callback.
  glfwSetErrorCallback(onGlfwError);
  
  // Create the window and check that it is valid.
  GLFWwindow* window = glfwCreateWindow(800, 600, "Lightspeed", NULL, NULL);
  if (!window) {
    glfwTerminate();
    std::cerr << "GLFW failed to create window." << '\n';
    exit(RESULT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  
  // Initialize GLEW.
  glewExperimental = GL_TRUE;
  if (glewInit()) {
    std::cerr << "GLEW failed to initialize." << '\n';
    exit(RESULT_FAILURE);
  }
  
  // Initialize the entity component system framework.
  onInitialize(window);
  
  // Disable the cursor so that it can't leave the window (standard FPS control
  // scheme).
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  
  // Register the input listeners.
  glfwSetCursorPosCallback(window, onMousePosition);
  glfwSetMouseButtonCallback(window, onMouseButton);
  glfwSetKeyCallback(window, onKeyboard);
  
  // Set up the entities and components.
  createScene();
  
  // Enter the main program loop.
  double previousTime = glfwGetTime();
  while (!glfwWindowShouldClose(window)) {
    
    // First check for any errors.
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
      onGlError(error);
    }
    
    // Update the time.
    double currentTime = glfwGetTime();
    double delta = currentTime - previousTime;
    previousTime = currentTime;
    
    // Determine the width and height of the framebuffer.
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    // Call the events.
    onUpdate(window, delta);
    onRender(window, width, height);
    
    // Swap the buffers and poll any window or keyboard events.
    glfwPollEvents();
    glfwSwapBuffers(window);
  }
  
  // Clean up the entity component system framework.
  onFinalize(window);
  
  // Clean up GLFW.
  glfwDestroyWindow(window);
  glfwTerminate();
  
  return RESULT_SUCCESS;
}

void createScene() {
  
  createPlayer();
  
  // Create 121 boxes arrayed in a grid.
  for (int i = -5; i <= +5; ++i) {
    for (int j = -5; j <= +5; ++j) {
      createBox(Vector(i, j, 10.0), Vector(0.5, 0.5, 0.5));
    }
  }
  
}

void createPlayer() {
  
  entityx::Entity player = entities.create();
  player.assign<AccelerationComponent>();
  player.assign<BodyComponent>();
  player.assign<CameraComponent>();
  player.assign<PlayerComponent>(
    GLFW_KEY_W,
    GLFW_KEY_S,
    GLFW_KEY_A,
    GLFW_KEY_D,
    1.0,
    1.0);
}

void createBox(Vector position, Vector dimensions) {
  
  GLfloat x1 = -dimensions.x / 2.0;
  GLfloat x2 = +dimensions.x / 2.0;
  GLfloat y1 = -dimensions.y / 2.0;
  GLfloat y2 = +dimensions.y / 2.0;
  GLfloat z1 = -dimensions.z / 2.0;
  GLfloat z2 = +dimensions.z / 2.0;
  
  std::initializer_list<Vertex> vertices = {
    
    // Constant x surfaces of the cube:
    { x1, y1, z1, 0, 0.0, 0.0 },
    { x1, y1, z2, 0, 0.0, 0.0 },
    { x1, y2, z1, 0, 0.0, 0.0 },
    { x1, y2, z1, 0, 0.0, 0.0 },
    { x1, y1, z2, 0, 0.0, 0.0 },
    { x1, y2, z2, 0, 0.0, 0.0 },
    
    { x2, y1, z1, 0, 0.0, 0.0 },
    { x2, y2, z1, 0, 0.0, 0.0 },
    { x2, y1, z2, 0, 0.0, 0.0 },
    { x2, y1, z2, 0, 0.0, 0.0 },
    { x2, y2, z1, 0, 0.0, 0.0 },
    { x2, y2, z2, 0, 0.0, 0.0 },
    
    // Constant y surfaces of the cube:
    { x1, y1, z1, 0, 0.0, 0.0 },
    { x2, y1, z1, 0, 0.0, 0.0 },
    { x1, y1, z2, 0, 0.0, 0.0 },
    { x1, y1, z2, 0, 0.0, 0.0 },
    { x2, y1, z1, 0, 0.0, 0.0 },
    { x2, y1, z2, 0, 0.0, 0.0 },
    
    { x1, y2, z1, 0, 0.0, 0.0 },
    { x1, y2, z2, 0, 0.0, 0.0 },
    { x2, y2, z1, 0, 0.0, 0.0 },
    { x2, y2, z1, 0, 0.0, 0.0 },
    { x1, y2, z2, 0, 0.0, 0.0 },
    { x2, y2, z2, 0, 0.0, 0.0 },
    
    // Constant z surfaces of the cube:
    { x1, y1, z1, 0, 0.0, 0.0 },
    { x1, y2, z1, 0, 0.0, 0.0 },
    { x2, y1, z1, 0, 0.0, 0.0 },
    { x2, y1, z1, 0, 0.0, 0.0 },
    { x1, y2, z1, 0, 0.0, 0.0 },
    { x2, y2, z1, 0, 0.0, 0.0 },
    
    { x1, y1, z2, 0, 0.0, 0.0 },
    { x2, y1, z2, 0, 0.0, 0.0 },
    { x1, y2, z2, 0, 0.0, 0.0 },
    { x1, y2, z2, 0, 0.0, 0.0 },
    { x2, y1, z2, 0, 0.0, 0.0 },
    { x2, y2, z2, 0, 0.0, 0.0 }
  };
  
  std::initializer_list<Texture*> textures = { NULL };
  
  entityx::Entity box = entities.create();
  box.assign<BodyComponent>(position, Quaternion(), Vector());
  box.assign<ModelComponent>(vertices, textures);
  box.assign<TimelineComponent<BodyComponent> >(10.0);
}

void onInitialize(GLFWwindow* window) {
  
  systems.add<AccelerationSystem>();
  systems.add<MovementSystem>();
  systems.add<PlayerSystem>();
  systems.add<RenderSystem>();
  systems.configure();
  
  events.emit<InitializeEvent>();
}

void onFinalize(GLFWwindow* window) {
  events.emit<FinalizeEvent>();
}

void onUpdate(GLFWwindow* window, double delta) {
  systems.update_all(delta);
}

void onRender(GLFWwindow* window, int viewportWidth, int viewportHeight) {
  events.emit<RenderEvent>(viewportWidth, viewportHeight);
}

void onMousePosition(GLFWwindow* window, double xpos, double ypos) {
  events.emit<MousePositionEvent>(xpos, ypos);
}

void onMouseButton(GLFWwindow* window, int button, int action, int mods) {
  events.emit<MouseButtonEvent>(button, action, mods);
}

void onKeyboard(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods) {
  
  events.emit<KeyboardEvent>(key, scancode, action, mods);
}

void onGlfwError(int error, char const* description) {
  std::cerr << "GLFW failed with error code "
            << error << ": " << description << '\n';
  exit(RESULT_FAILURE);
}

void onGlError(int error) {
  std::cerr << "OpenGL failed with error code " << error << '\n';
  exit(RESULT_FAILURE);
}

