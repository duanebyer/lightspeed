#include "vector.h"

#include <cmath>

using namespace lightspeed;

Vector& Vector::operator+=(Vector const& rhs) {
  
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  return *this;
}

Vector& Vector::operator-=(Vector const& rhs) {
  
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  return *this;
}

Vector& Vector::operator*=(double rhs) {
  x *= rhs;
  y *= rhs;
  z *= rhs;
  return *this;
}

Vector& Vector::operator/=(double rhs) {
  x /= rhs;
  y /= rhs;
  z /= rhs;
  return *this;
}

Vector Vector::operator+() const {
  return *this;
}

Vector Vector::operator-() const {
  return Vector(-x, -y, -z);
}

double Vector::dot(Vector const& rhs) const {
  return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vector Vector::cross(Vector const& rhs) const {
  
  return Vector(
    y * rhs.z - rhs.y * z,
    z * rhs.x - rhs.z * x,
    x * rhs.y - rhs.x * y);
}

double Vector::normSq() const {
  return dot(*this);
}

double Vector::norm() const {
  return std::sqrt(normSq());
}

Vector Vector::unit() const {
  return *this / norm();
}

