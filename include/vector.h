#ifndef __LIGHTSPEED_VECTOR_H_
#define __LIGHTSPEED_VECTOR_H_

namespace lightspeed {

struct Vector final {
  
  Vector() :
      x(0.0),
      y(0.0),
      z(0.0) {
  }
  
  Vector(double x, double y, double z) :
      x(x),
      y(y),
      z(z) {
  }
  
  Vector operator+() const;
  Vector operator-() const;
  
  Vector& operator+=(Vector const& rhs);
  Vector& operator-=(Vector const& rhs);
  Vector& operator*=(double rhs);
  Vector& operator/=(double rhs);
  
  double dot(Vector const& rhs) const;
  Vector cross(Vector const& rhs) const;
  double norm() const;
  double normSq() const;
  Vector unit() const;
  
  double x;
  double y;
  double z;
  
};

inline Vector operator+(Vector lhs, Vector const& rhs) {
  return (lhs += rhs);
}
inline Vector operator-(Vector lhs, Vector const& rhs) {
  return (lhs -= rhs);
}
inline Vector operator*(Vector lhs, double rhs) {
  return (lhs *= rhs);
}
inline Vector operator*(double lhs, Vector rhs) {
  return (rhs *= lhs);
}
inline Vector operator/(Vector lhs, double rhs) {
  return (lhs /= rhs);
}

}

#endif

