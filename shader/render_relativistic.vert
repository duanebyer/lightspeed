#version 430

// This structure represents the state of a body at a specific moment in time.
struct Transform {
  // Position is a 4 vector storing the 3 dimensional location of the body
  // as well as the time.
  vec4 position;
  // Velocity stores the ordinary linear velocity of the body at that time.
  vec3 velocity;
  // Rotation stores a quaternion representing the rotation at that time.
  vec4 rotation;
};

layout (std430, binding = 0) readonly buffer Timeline {
  Transform history[];
};

layout(location = 0) uniform Transform observer;
layout(location = 3) uniform mat4 projection;
layout(location = 4) uniform float lightspeed;

layout(location = 0) in vec4 position;

vec3 applyQuaternion(in vec4 quaternion, in vec3 p) {
  vec4 intermediate;
  vec4 result;
  vec4 inverse;
  
  inverse.w = quaternion.w;
  inverse.xyz = -quaternion.xyz;
  inverse /= dot(quaternion, inverse);
  
  intermediate.w = -dot(quaternion.xyz, p);
  intermediate.xyz = quaternion.w * p + cross(quaternion.xyz, p);
  
  result.w = intermediate.w * inverse.w - dot(intermediate.xyz, inverse.xyz);
  result.xyz = intermediate.w * inverse.xyz + inverse.w * intermediate.xyz +
               cross(intermediate.xyz, inverse.xyz);
  
  return result.xyz;
}

float minkowskiDot(in vec4 a, in vec4 b) {
  return a.w * b.w - dot(a.xyz / lightspeed, b.xyz / lightspeed);
}

float minkowskiLength(in vec4 a) {
  return minkowskiDot(a, a);
}

void main() {
  
  // Go through the history, from the newest to oldest position.
  uint i = history.length();
  vec4 lastPosition;
  vec4 nextPosition;
  vec4 currentPosition;
  bool hasNextPosition = false;
  bool hasLastPosition = false;
  
  while (i != 0) {
    
    --i;
    hasNextPosition = true;
    
    vec4 nextPosition;
    Transform objectTransform = history[i];
    
    // The w coordinate is used to represent time.
    nextPosition.w = position.w;
    
    // Perform the rotation first.
    nextPosition.xyz = applyQuaternion(objectTransform.rotation, position.xyz);
    
    // Then apply the scaling along the velocity direction due to length
    // contraction of the object.
    float objectBeta = length(objectTransform.velocity) / lightspeed;
    vec3 objectVelDir = normalize(objectTransform.velocity);
    float scaleFactor = sqrt(1.0 - objectBeta * objectBeta);
    
    vec3 parallelComponent = dot(objectVelDir, nextPosition.xyz) *
                             objectVelDir;
    vec3 perpendicularComponent = nextPosition.xyz - parallelComponent;
    
    nextPosition.xyz = perpendicularComponent +
                              scaleFactor * parallelComponent;
    
    // Finally, translate the object to the position it should be in.
    nextPosition += objectTransform.position;
    
    // Now the vertex has been transformed into the rest frame, but we have to
    // Lorentz transform it into the observer's frame.
    
    // First, the point is translated.
    nextPosition -= observer.position;
    
    // Then the boost/Lorentz transformation is applied.
    float beta = length(observer.velocity) / lightspeed;
    vec3 velDir = normalize(observer.velocity);
    float gamma = 1.0 / sqrt(1.0 - beta * beta);
    
    parallelComponent = dot(velDir, nextPosition.xyz) * velDir;
    perpendicularComponent = nextPosition.xyz - parallelComponent;
    
    // These are just the Lorentz transforms in 3 spatial dimensions.
    nextPosition.xyz =
      perpendicularComponent +
      gamma * (parallelComponent - observer.velocity * nextPosition.w);
    nextPosition.w =
      gamma * (nextPosition.w -
               dot(beta * velDir, parallelComponent) / lightspeed);
    
    // Finally, the transformed position is rotated.
    vec4 inverse;
    inverse.w = observer.rotation.w;
    inverse.xyz = -observer.rotation.xyz;
    inverse /= dot(inverse, observer.rotation);
    nextPosition.xyz = applyQuaternion(inverse, nextPosition.xyz);
    
    // Now we check to see if the transformed position is timelike. If it is
    // timelike, then the light from the position has reached the observer.
    if (minkowskiLength(nextPosition) > 0.0) {
      break;
    }
    else {
      // Otherwise, store the transformed position, and move to the next point
      // in the history.
      lastPosition = nextPosition;
      hasLastPosition = true;
    }
  }
  
  // Now, take the two positions (lastPosition and nextPosition) and find the
  // intersection of the line between them with the light cone from the
  // observer.
  if (!hasNextPosition) {
    currentPosition = vec4(0.0, 0.0, 0.0, 1.0);
  }
  else if (!hasLastPosition) {
    currentPosition = nextPosition;
  }
  else {
    vec4 dir = nextPosition - lastPosition;
    float a = minkowskiLength(dir);
    float b = 2.0 * minkowskiDot(dir, lastPosition);
    float c = minkowskiLength(lastPosition);
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0) {
      currentPosition = nextPosition;
    }
    else {
      float s1 = (-b + sqrt(discriminant)) / (2.0 * a);
      float s2 = (-b - sqrt(discriminant)) / (2.0 * a);
      vec4 intersection1 = lastPosition + s1 * dir;
      vec4 intersection2 = lastPosition + s2 * dir;
      if (intersection2.w <= observer.position.w) {
        currentPosition = intersection2;
      }
      else if (intersection1.w <= observer.position.w) {
        currentPosition = intersection1;
      }
      else {
        currentPosition = nextPosition;
      }
    }
  }
  
  // The currentPosition variable stores the location that the vertex appears
  // to be at, taking into account the travel time of light. At this point,
  // the vertex can be rendered as normal.
  
  // First though, the w component has to be changed from indicating time to
  // acting as the 4th homogeneous component.
  currentPosition.w = 1.0;
  
  // Return the projected result.
  gl_Position = projection * currentPosition;
}

