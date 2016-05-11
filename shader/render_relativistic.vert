#version 430

// This structure represents the state of a body at a specific moment in time.
struct Transform {
  // Position is a 4 vector storing the 3 dimensional location of the body
  // as well as the time.
  vec4 position;
  // Momentum stores the four momentum of the body at that time.
  vec4 momentum;
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

vec4 quaternionProduct(in vec4 a, in vec4 b) {
  vec4 result = vec4(a.w * b.xyz + b.w * a.xyz + cross(a.xyz, b.xyz),
                     a.w * b.w - dot(a.xyz, b.xyz));
  return result;
}

vec4 quaternionConjugate(in vec4 a) {
  return vec4(-a.xyz, a.w);
}

vec4 quaternionInverse(in vec4 a) {
  return quaternionConjugate(a) / dot(a, a);
}

vec3 applyQuaternion(in vec4 quaternion, in vec3 p) {
  return quaternionProduct(quaternionProduct(quaternion, vec4(p, 0.0)),
                           quaternionInverse(quaternion)).xyz;
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
  bool intersectsCone = false;
  
  while (i != 0) {
    
    --i;
    
    if (hasNextPosition) {
      lastPosition = nextPosition;
      hasLastPosition = true;
    }
    
    nextPosition = vec4(0.0);
    Transform objectTransform = history[i];
    
    // Calculate the velocity first.
    vec3 objectVelocity = objectTransform.momentum.xyz /
                          objectTransform.momentum.w;
    
    // Perform the rotation first.
    nextPosition.xyz = applyQuaternion(objectTransform.rotation, position.xyz);
    
    // Then apply the scaling along the velocity direction due to length
    // contraction of the object.
    float objectBeta = length(objectVelocity) / lightspeed;
    vec3 objectVelDir = normalize(objectVelocity.xyz);
    if (objectBeta == 0.0) {
      objectVelDir = vec3(1.0, 0.0, 0.0);
    }
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
    
    // First calculate the observer's velocity.
    vec3 observerVelocity = observer.momentum.xyz / observer.momentum.w;
    
    // Then the point is translated.
    nextPosition -= observer.position;
    /*
    // Then the boost/Lorentz transformation is applied.
    float beta = length(observerVelocity) / lightspeed;
    vec3 velDir = normalize(observerVelocity.xyz);
    if (beta == 0.0) {
      velDir = vec3(1.0, 0.0, 0.0);
    }
    float gamma = 1.0 / sqrt(1.0 - beta * beta);
    
    parallelComponent = dot(velDir, nextPosition.xyz) * velDir;
    perpendicularComponent = nextPosition.xyz - parallelComponent;
    
    // These are just the Lorentz transforms in 3 spatial dimensions.
    nextPosition.xyz =
      perpendicularComponent +
      gamma * (parallelComponent - observerVelocity.xyz * nextPosition.w);
    nextPosition.w =
      gamma * (nextPosition.w -
               dot(beta * velDir, parallelComponent) / lightspeed);
    */
    // Finally, the transformed position is rotated.
    vec4 inverse;
    inverse.w = observer.rotation.w;
    inverse.xyz = -observer.rotation.xyz;
    inverse /= dot(inverse, observer.rotation);
    nextPosition.xyz = applyQuaternion(inverse, nextPosition.xyz);
    /*
    // Now we check to see if the transformed position is timelike. If it is
    // timelike, then the light from the position has reached the observer.
    if (minkowskiLength(nextPosition) > 0.0) {
      break;
    }
    */
    hasNextPosition = true;
    break;
  }
  /*
  // Now, take the two positions (lastPosition and nextPosition) and find the
  // intersection of the line between them with the light cone from the
  // observer.
  
  if (hasLastPosition && hasNextPosition) {
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
  else if (hasNextPosition) {
    currentPosition = nextPosition;
  }
  else {
    currentPosition = vec4(0.0);
  }
  
  // The currentPosition variable stores the location that the vertex appears
  // to be at, taking into account the travel time of light. At this point,
  // the vertex can be rendered as normal.
  
  // First though, the w component has to be changed from indicating time to
  // acting as the 4th homogeneous component.
  currentPosition.w = 1.0;
  
  // Return the projected result.
  gl_Position = projection * currentPosition;
  */
  nextPosition.w = 1.0;
  gl_Position = projection * nextPosition;
}

