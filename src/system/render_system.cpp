#include "system/render_system.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <vector>

#include "internal/opengl.h"

#include "component/body_component.h"
#include "component/camera_component.h"
#include "component/model_component.h"
#include "component/timeline_component.h"

#include "event/finalize_event.h"
#include "event/initialize_event.h"
#include "event/render_event.h"

#include "quaternion.h"
#include "utility.h"
#include "vector.h"
#include "vertex.h"

#define UNIFORM_OBSERVER (0)
#define UNIFORM_OBSERVER_POSITION (0)
#define UNIFORM_OBSERVER_VELOCITY (1)
#define UNIFORM_OBSERVER_ROTATION (2)
#define UNIFORM_PROJECTION (3)
#define UNIFORM_LIGHTSPEED (4)

#define BUFFER_TIMELINE (0)

#define ATTRIBUTE_POSITION (0)

using namespace lightspeed;

GLuint createShader(
    unsigned int num,
    GLenum* shaderTypes,
    std::string* fileNames);
void destroyShader(GLuint shader);

void fillObserver(BodyComponent& body);
void fillProjection(CameraComponent& camera);
void fillLightspeed();
void fillTimeline(TimelineComponent<BodyComponent>& timeline, GLuint buffer);

void RenderSystem::configure(
    entityx::EntityManager& entities,
    entityx::EventManager& events) {
  
  // Store internal references to the entity and event managers.
  m_entities = &entities;
  m_events = &events;
  
  // Subscribe to all of the events.
  events.subscribe<InitializeEvent>(*this);
  events.subscribe<FinalizeEvent>(*this);
  events.subscribe<RenderEvent>(*this);
  
  events.subscribe<entityx::ComponentAddedEvent<ModelComponent> >(*this);
  events.subscribe<entityx::ComponentRemovedEvent<ModelComponent> >(*this);
  
  events.subscribe<entityx::ComponentAddedEvent<
    TimelineComponent<BodyComponent> > >(*this);
  events.subscribe<entityx::ComponentRemovedEvent<
    TimelineComponent<BodyComponent> > >(*this);
}

void RenderSystem::update(
    entityx::EntityManager& entities,
    entityx::EventManager& events,
    entityx::TimeDelta delta) {
}

void RenderSystem::receive(InitializeEvent const& event) {
  
  // Create the shaders.
  std::string renderRelativisticShaderFilenames[] = {
    "render_relativistic.vert",
    "render_relativistic.frag"
  };
  GLenum renderRelativisticShaderTypes[] = {
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER
  };
  
  m_renderRelativisticShader = createShader(
    2,
    renderRelativisticShaderTypes,
    renderRelativisticShaderFilenames
  );
}

void RenderSystem::receive(FinalizeEvent const& event) {
  
  // Clean up the shaders.
  destroyShader(m_renderRelativisticShader);
}

void RenderSystem::receive(
    entityx::ComponentAddedEvent<ModelComponent> const& event) {
  
  // Create a new vertex buffer array to hold the geometry.
  GLuint vertexBuffer;
  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  
  // Fill the buffer with the vertex data.
  glBufferData(
    GL_ARRAY_BUFFER,
    sizeof(Vertex) * event.component->vertices.size(),
    event.component->vertices.data(),
    GL_STATIC_DRAW);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  // Store the buffer index in the map.
  m_vertexBuffers[event.component.get()] = vertexBuffer;
}

void RenderSystem::receive(
    entityx::ComponentAddedEvent<
      TimelineComponent<BodyComponent> > const& event) {
  
  // Create a buffer to store timeline information.
  GLuint timelineBuffer;
  glGenBuffers(1, &timelineBuffer);
  m_timelineBuffers[event.component.get()] = timelineBuffer;
}

void RenderSystem::receive(
    entityx::ComponentRemovedEvent<ModelComponent> const& event) {
  
  // Look up the buffer from the map, remove it, and clean it up.
  GLuint vertexBuffer = m_vertexBuffers[event.component.get()];
  m_vertexBuffers.erase(event.component.get());
  glDeleteBuffers(1, &vertexBuffer);
}

void RenderSystem::receive(
    entityx::ComponentRemovedEvent<
      TimelineComponent<BodyComponent> > const& event) {
  
  // Clean up any buffers corresponding to timeline components as well.
  GLuint timelineBuffer = m_timelineBuffers[event.component.get()];
  m_timelineBuffers.erase(event.component.get());
  glDeleteBuffers(1, &timelineBuffer);
}

void RenderSystem::receive(RenderEvent const& event) {
  
  // Find an entity with a camera component, and use it to create the projection
  // matrix. Exactly one entity should have a camera component.
  bool hasCamera = false;
  CameraComponent camera;
  BodyComponent body;
  m_entities->each<CameraComponent>(
    [&hasCamera, &camera, &body](entityx::Entity entity, CameraComponent& c) {
      
      if (hasCamera) {
        throw std::runtime_error(
          "Can't have multiple entities with a camera component.");
      }
      camera = c;
      hasCamera = true;
      
      // Check if a body component is present, and if so, remember it.
      entityx::ComponentHandle<BodyComponent> b =
        entity.component<BodyComponent>();
      if (b) {
        body = *b.get();
      }
    });
  if (!hasCamera) {
    throw std::runtime_error("Must have an entity with a camera component.");
  }
  
  // First set up the viewport and clear the background.
  glViewport(0, 0, event.viewportWidth, event.viewportHeight);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // Set the shader that will be used.
  glUseProgram(m_renderRelativisticShader);
  
  // Set up the attributes of the vertices.
  glEnableVertexAttribArray(ATTRIBUTE_POSITION);
  
  // Pass relevent data about the observer to the shader.
  fillObserver(body);
  fillProjection(camera);
  fillLightspeed();
  
  // Loop through every entity with a timeline and model component and render
  // it.
  m_entities->each<ModelComponent, TimelineComponent<BodyComponent> >(
    [this](
        entityx::Entity entity,
        ModelComponent& model,
        TimelineComponent<BodyComponent>& timeline) {
      
      // Get the indices of the buffers.
      GLuint vertexBuffer = m_vertexBuffers[&model];
      GLuint timelineBuffer = m_timelineBuffers[&timeline];
      
      fillTimeline(timeline, timelineBuffer);
      glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
      glVertexAttribPointer(
        ATTRIBUTE_POSITION,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        0);
      glDrawArrays(GL_TRIANGLES, 0, model.vertices.size());
    });
  
  // Unset things so that the state resets.
  glDisableVertexAttribArray(ATTRIBUTE_POSITION);
  glUseProgram(0);
}

// Passes the data from a body component to the shader.
void fillObserver(BodyComponent& body) {
  
  GLfloat position[4] = {
    (GLfloat) body.position.x,
    (GLfloat) body.position.y,
    (GLfloat) body.position.z,
    0.0
  };
  
  GLfloat velocity[3] = {
    (GLfloat) body.velocity.x,
    (GLfloat) body.velocity.y,
    (GLfloat) body.velocity.z
  };
  
  GLfloat rotation[4] = {
    (GLfloat) body.rotation.pure.x,
    (GLfloat) body.rotation.pure.y,
    (GLfloat) body.rotation.pure.z,
    (GLfloat) body.rotation.real
  };
  
  glUniform4fv(UNIFORM_OBSERVER_POSITION, 1, position);
  glUniform3fv(UNIFORM_OBSERVER_VELOCITY, 1, velocity);
  glUniform4fv(UNIFORM_OBSERVER_ROTATION, 1, rotation);
}

void fillProjection(CameraComponent& camera) {
  
  // Calculate the projection matrix and pass it to the shader.
  GLfloat projectionMatrix[16] = { 0.0 };
  
  double cotHorz = 1.0 / std::tan(camera.fov * camera.aspectRatio / 2.0);
  double cotVert = 1.0 / std::tan(camera.fov / 2.0);
  double clipDiff = camera.clipFar - camera.clipNear;
  double clipSum = camera.clipFar + camera.clipNear;
  double clipProd = camera.clipFar * camera.clipNear;
  
  projectionMatrix[0] = (GLfloat) cotHorz;
  projectionMatrix[5] = (GLfloat) cotVert;
  projectionMatrix[9] = (GLfloat) (-clipSum / clipDiff);
  projectionMatrix[10] = (GLfloat) (-2 * clipProd / clipDiff);
  projectionMatrix[14] = -1;
  
  glUniformMatrix4fv(UNIFORM_PROJECTION, 1, GL_TRUE, projectionMatrix);
}

void fillLightspeed() {
  glUniform1f(UNIFORM_LIGHTSPEED, LIGHT_SPEED);
}

void fillTimeline(TimelineComponent<BodyComponent>& timeline, GLuint buffer) {
  
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
  
  // Translate the timeline component into a buffer that will be passed to the
  // shader.
  unsigned int floatsPerEntry = 4 * 3;
  std::vector<GLfloat> data(timeline.timeline.size() * floatsPerEntry);
  for (unsigned int i = 0; i < timeline.timeline.size(); ++i) {
    unsigned int dataIndex = floatsPerEntry * i;
    BodyComponent body = timeline.timeline[i].second;
    Vector position = body.position;
    Vector velocity = body.velocity;
    Quaternion rotation = body.rotation;
    double time = timeline.timeline[i].first;
    
    data[dataIndex + 0] = (GLfloat) position.x;
    data[dataIndex + 1] = (GLfloat) position.y;
    data[dataIndex + 2] = (GLfloat) position.z;
    data[dataIndex + 3] = (GLfloat) time;
    
    data[dataIndex + 4] = (GLfloat) velocity.x;
    data[dataIndex + 5] = (GLfloat) velocity.y;
    data[dataIndex + 6] = (GLfloat) velocity.z;
    data[dataIndex + 7] = 0.0;
    
    data[dataIndex + 8] = (GLfloat) rotation.pure.x;
    data[dataIndex + 9] = (GLfloat) rotation.pure.y;
    data[dataIndex + 10] = (GLfloat) rotation.pure.z;
    data[dataIndex + 11] = (GLfloat) rotation.real;
  }
  
  glBufferData(
    GL_SHADER_STORAGE_BUFFER,
    sizeof(GLfloat) * data.size(),
    data.data(),
    GL_DYNAMIC_DRAW);
  
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BUFFER_TIMELINE, buffer);
}

GLuint createShader(
    unsigned int num,
    GLenum* shaderTypes,
    std::string* fileNames) {
  
  // Create a vector to store every part of the shader (e.g. the vertex shader
  // part, the fragment shader part, and so on).
  std::vector<GLuint> shaders(num);
  
  for (unsigned int i = 0; i < num; ++i) {
    
    // Load the file and put its contents into a string.
    std::ifstream file(fileNames[i]);
    std::string fileContents(
      (std::istreambuf_iterator<char>(file)),
      (std::istreambuf_iterator<char>()));
    
    // Create and compile the shader.
    shaders[i] = glCreateShader(shaderTypes[i]);
    GLchar const* sourceChars = fileContents.c_str();
    glShaderSource(shaders[i], 1, &sourceChars, NULL);
    glCompileShader(shaders[i]);
    
    // Check that the compilation was a success.
    GLint status;
    glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
      GLint errorLength;
      glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &errorLength);
      GLchar* errorChars = new GLchar[errorLength + 1];
      glGetShaderInfoLog(shaders[i], errorLength, NULL, errorChars);
      std::string errorStr(errorChars);
      delete [] errorChars;
      throw std::runtime_error(
        "Couldn't compile the shader " + fileNames[i] + ". Error: " + errorStr);
    }
  }
  
  // Now create the final shader program.
  GLuint program = glCreateProgram();
  
  // Attach all of the parts of the shader to the shader program and link.
  for (unsigned int i = 0; i < num; ++i) {
    glAttachShader(program, shaders[i]);
  }
  glLinkProgram(program);
  
  // Check for linking errors.
  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE) {
    GLint errorLength;
    glGetShaderiv(program, GL_INFO_LOG_LENGTH, &errorLength);
    GLchar* errorChars = new GLchar[errorLength + 1];
    glGetShaderInfoLog(program, errorLength, NULL, errorChars);
    std::string errorStr(errorChars);
    delete [] errorChars;
    throw std::runtime_error(
      "Couldn't link the shader program. Error: " + errorStr);
  }
  
  // Now delete the leftover resources that are no longer needed.
  for (unsigned int i = 0; i < num; ++i) {
    glDeleteShader(shaders[i]);
  }
  
  return program;
}

void destroyShader(GLuint shader) {
  glDeleteProgram(shader);
}

