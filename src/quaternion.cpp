#include "quaternion.h"

#include <cmath>

using namespace lightspeed;

Quaternion& Quaternion::operator+=(Quaternion const& rhs) {
  
  real += rhs.real;
  pure += rhs.pure;
  return *this;
}

Quaternion& Quaternion::operator-=(Quaternion const& rhs) {
  
  real -= rhs.real;
  pure -= rhs.pure;
  return *this;
}

Quaternion& Quaternion::operator*=(Quaternion const& rhs) {
  
  double oldReal = real;
  real = real * rhs.real - pure.dot(rhs.pure);
  pure = oldReal * rhs.pure + rhs.real * pure + pure.cross(rhs.pure);
  return *this;
}

Quaternion& Quaternion::operator/=(Quaternion const& rhs) {
  
  Quaternion inv = rhs.inverse();
  return operator*=(inv);
}

Quaternion& Quaternion::operator*=(double rhs) {
  
  real *= rhs;
  pure *= rhs;
  return *this;
}

Quaternion& Quaternion::operator/=(double rhs) {
  real /= rhs;
  pure /= rhs;
  return *this;
}

Quaternion Quaternion::operator+() const {
  return *this;
}

Quaternion Quaternion::operator-() const {
  return Quaternion(-real, -pure);
}

double Quaternion::dot(Quaternion const& rhs) const {
  return real * rhs.real + pure.dot(rhs.pure);
}

Quaternion Quaternion::conjugate() const {
  return Quaternion(real, -pure);
}

Quaternion Quaternion::inverse() const {
  return conjugate() / normSq();
}

double Quaternion::norm() const {
  return std::sqrt(normSq());
}

double Quaternion::normSq() const {
  return dot(conjugate());
}

Quaternion Quaternion::unit() const {
  return *this / norm();
}

Vector Quaternion::rotate(Vector const& vec) const {
  
  Quaternion pureVec(0.0, vec);
  Quaternion result = *this * pureVec * inverse();
  return result.pure;
}

