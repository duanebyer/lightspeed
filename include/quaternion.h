#ifndef __LIGHTSPEED_QUATERNION_H_
#define __LIGHTSPEED_QUATERNION_H_

#include "vector.h"

#include "vector.h"

namespace lightspeed {

struct Quaternion final {
  
  Quaternion() :
      pure(),
      real(0.0) {
  }
  
  Quaternion(double real, Vector pure) :
      pure(pure),
      real(real) {
  }
  
  Quaternion operator+() const;
  Quaternion operator-() const;
  
  Quaternion& operator+=(Quaternion const& rhs);
  Quaternion& operator-=(Quaternion const& rhs);
  Quaternion& operator*=(Quaternion const& rhs);
  Quaternion& operator/=(Quaternion const& rhs);
  
  Quaternion& operator*=(double rhs);
  Quaternion& operator/=(double rhs);
  
  double dot(Quaternion const& rhs) const;
  Quaternion conjugate() const;
  Quaternion inverse() const;
  double norm() const;
  double normSq() const;
  Quaternion unit() const;
  
  Vector rotate(Vector const& vec) const;
  
  Vector pure;
  double real;
  
};

inline Quaternion operator+(Quaternion lhs, Quaternion const& rhs) {
  return (lhs += rhs);
}
inline Quaternion operator-(Quaternion lhs, Quaternion const& rhs) {
  return (lhs -= rhs);
}
inline Quaternion operator*(Quaternion lhs, Quaternion const& rhs) {
  return (lhs *= rhs);
}
inline Quaternion operator/(Quaternion lhs, Quaternion const& rhs) {
  return (lhs /= rhs);
}

inline Quaternion operator*(Quaternion lhs, double rhs) {
  return (lhs *= rhs);
}
inline Quaternion operator*(double lhs, Quaternion rhs) {
  return (rhs *= lhs);
}
inline Quaternion operator/(Quaternion lhs, double rhs) {
  return (lhs /= rhs);
}

}

#endif

