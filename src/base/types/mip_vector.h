#ifndef mip_vector_included
#define mip_vector_included
//----------------------------------------------------------------------

#include "mip.h"
#include "base/utils/mip_math.h"

#include <math.h>

//----------------------------------------------------------------------
// MIP_Vector2
//----------------------------------------------------------------------

struct MIP_Vector2 {

  union {
    float el[2];
    struct { float x,y; };
  };

  //----------

  MIP_Vector2()                    {}
  MIP_Vector2(float v)             { x=v; y=v; }
  MIP_Vector2(float ax, float ay)  { x=ax; y=ay; }

  MIP_Vector2  operator +  (const MIP_Vector2& rhs)  const { return MIP_Vector2(x+rhs.x,y+rhs.y); }
  MIP_Vector2  operator -  (const MIP_Vector2& rhs)  const { return MIP_Vector2(x-rhs.x,y-rhs.y); }
  MIP_Vector2  operator *  (const MIP_Vector2& rhs)  const { return MIP_Vector2(x*rhs.x,y*rhs.y); }
  MIP_Vector2  operator /  (const MIP_Vector2& rhs)  const { return MIP_Vector2(x/rhs.x,y/rhs.y); }
  MIP_Vector2& operator += (const MIP_Vector2& v)          { x+=v.x; y+=v.y; return *this; }
  MIP_Vector2& operator -= (const MIP_Vector2& v)          { x-=v.x; y-=v.y; return *this; }
  MIP_Vector2& operator *= (const MIP_Vector2& v)          { x*=v.x; y*=v.y; return *this; }
  MIP_Vector2& operator /= (const MIP_Vector2& v)          { x/=v.x; y/=v.y; return *this; }
  MIP_Vector2  operator +  (float s)          const { return MIP_Vector2(x+s,y+s); }
  MIP_Vector2  operator -  (float s)          const { return MIP_Vector2(x-s,y-s); }
  MIP_Vector2  operator *  (float s)          const { return MIP_Vector2(x*s,y*s); }
  MIP_Vector2  operator /  (float s)          const { return MIP_Vector2(x/s,y/s); }
  MIP_Vector2& operator += (const float& s)         { x+=s; y+=s; return *this; }
  MIP_Vector2& operator -= (const float& s)         { x-=s; y-=s; return *this; }
  MIP_Vector2& operator *= (const float& s)         { x*=s; y*=s; return *this; }
  MIP_Vector2& operator /= (const float& s)         { x/=s; y/=s; return *this; }
  MIP_Vector2  operator -  ()                 const { return MIP_Vector2(-x,-y); }

  float dot(MIP_Vector2 v1) {
    return x * v1.x + y * v1.y;
  }

  MIP_Vector2 cross(MIP_Vector2 v1) {
    return MIP_Vector2(
      0,
      0
    );
  }

  void cross(MIP_Vector2 v0, MIP_Vector2 v1) {
    x = 0;
    y = 0;
  }

  float length(void) {
    return sqrtf(x*x + y*y);
  }

  float length_sq(void) {
    return (x*x + y*y);
  }

  void normalize(void) {
    float il = 1.0f / length();
    x *= il;
    y *= il;
  }

  void rotate(float AAngle) {
    float sa = sin(AAngle);
    float ca = cos(AAngle);
    x = (x * ca) - (y * sa);
    y = (x * sa) + (y * ca);
  }

};

//----------------------------------------------------------------------

MIP_Vector2  MIP_Vector2_add(MIP_Vector2 v0, MIP_Vector2 v1)  { MIP_Vector2 r = { v0.x + v1.x, v0.y + v1.y }; return r; }
MIP_Vector2  MIP_Vector2_sub(MIP_Vector2 v0, MIP_Vector2 v1)  { MIP_Vector2 r = { v0.x - v1.x, v0.y - v1.y }; return r; }
MIP_Vector2  MIP_Vector2_mul(MIP_Vector2 v0, MIP_Vector2 v1)  { MIP_Vector2 r = { v0.x * v1.x, v0.y * v1.y }; return r; }
MIP_Vector2  MIP_Vector2_div(MIP_Vector2 v0, MIP_Vector2 v1)  { MIP_Vector2 r = { v0.x / v1.x, v0.y / v1.y }; return r; }

MIP_Vector2  MIP_Vector2_add(MIP_Vector2 v, float f)   { MIP_Vector2 r = { v.x + f, v.y + f }; return r; }
MIP_Vector2  MIP_Vector2_sub(MIP_Vector2 v, float f)   { MIP_Vector2 r = { v.x - f, v.y - f }; return r; }
MIP_Vector2  MIP_Vector2_mul(MIP_Vector2 v, float f)   { MIP_Vector2 r = { v.x * f, v.y * f }; return r; }
MIP_Vector2  MIP_Vector2_div(MIP_Vector2 v, float f)   { MIP_Vector2 r = { v.x / f, v.y / f }; return r; }

float MIP_Vector2_dot(MIP_Vector2 v0, MIP_Vector2 v1) {
  return (v0.x * v1.x)
       + (v0.y * v1.y);
}

MIP_Vector2 MIP_Vector2_cross(MIP_Vector2 v0, MIP_Vector2 v1) {
  return MIP_Vector2(
    0,
    0
  );
} // USUALLY FAIL

float MIP_Vector2_length(MIP_Vector2 v) {
  return sqrtf( (v.x*v.x) + (v.y*v.y) );
}

float MIP_Vector2_length_sq(MIP_Vector2 v) {
  return ( (v.x*v.x) + (v.y*v.y) );
}

MIP_Vector2 MIP_Vector2_normalize(MIP_Vector2 v) {
  float fi = 1.0f / MIP_Vector2_length(v);
  return MIP_Vector2(
    v.x * fi,
    v.y * fi
  );
}

//----------------------------------------------------------------------
// MIP_Vector3
//----------------------------------------------------------------------

struct MIP_Vector3 {

  union {
    float el[3];
    struct { float x,y,z; };
    struct { float r,g,b; };
    MIP_Vector2 xy;
  };

  //----------

  MIP_Vector3()                              { x=0.0f; y=0.0f; z=0.0f; }
  MIP_Vector3(float v)                       { x=v; y=v; z=v; }
  MIP_Vector3(float ax, float ay, float az)  { x=ax; y=ay; z=az; }
  //MIP_Vector3(MIP_Vector2 axy, float az, float aw)            { x=axy.x; y=axy.y; z=az; w=aw; }
  //MIP_Vector3(MIP_Vector3 axyz, float aw)                     { x=axyz.x; y=axyz.y; z=axyz.z; w=aw; }

  MIP_Vector3  operator +  (const MIP_Vector3& rhs) const { return MIP_Vector3(x+rhs.x,y+rhs.y,z+rhs.z); }
  MIP_Vector3  operator -  (const MIP_Vector3& rhs) const { return MIP_Vector3(x-rhs.x,y-rhs.y,z-rhs.z); }
  MIP_Vector3  operator *  (const MIP_Vector3& rhs) const { return MIP_Vector3(x*rhs.x,y*rhs.y,z*rhs.z); }
  MIP_Vector3  operator /  (const MIP_Vector3& rhs) const { return MIP_Vector3(x/rhs.x,y/rhs.y,z/rhs.z); }
  MIP_Vector3& operator += (const MIP_Vector3& v)         { x+=v.x; y+=v.y; z+=v.z; return *this; }
  MIP_Vector3& operator -= (const MIP_Vector3& v)         { x-=v.x; y-=v.y; z-=v.z; return *this; }
  MIP_Vector3& operator *= (const MIP_Vector3& v)         { x*=v.x; y*=v.y; z*=v.z; return *this; }
  MIP_Vector3& operator /= (const MIP_Vector3& v)         { x/=v.x; y/=v.y; z/=v.z; return *this; }
  MIP_Vector3  operator +  (float s)           const { return MIP_Vector3(x+s,y+s,z+s); }
  MIP_Vector3  operator -  (float s)           const { return MIP_Vector3(x-s,y-s,z-s); }
  MIP_Vector3  operator *  (float s)           const { return MIP_Vector3(x*s,y*s,z*s); }
  MIP_Vector3  operator /  (float s)           const { return MIP_Vector3(x/s,y/s,z/s); }
  MIP_Vector3& operator += (const float& s)          { x+=s; y+=s; z+=s; return *this; }
  MIP_Vector3& operator -= (const float& s)          { x-=s; y-=s; z-=s; return *this; }
  MIP_Vector3& operator *= (const float& s)          { x*=s; y*=s; z*=s; return *this; }
  MIP_Vector3& operator /= (const float& s)          { x/=s; y/=s; z/=s; return *this; }
  MIP_Vector3  operator -  ()                  const { return MIP_Vector3(-x,-y,-z); }

  float dot(MIP_Vector3 v1) {
    return (x * v1.x) + (y * v1.y) + (z * v1.z);
  }

  MIP_Vector3 cross(MIP_Vector3 v1) {
    return MIP_Vector3(
      (y * v1.z) - (z * v1.y),
      (z * v1.x) - (x * v1.z),
      (x * v1.y) - (y * v1.x)
    );
  }

  void cross(MIP_Vector3 v0, MIP_Vector3 v1) {
    x = (v0.y * v1.z) - (v0.z * v1.y);
    y = (v0.z * v1.x) - (v0.x * v1.z);
    z = (v0.x * v1.y) - (v0.y * v1.x);
  }

  float length(void) {
    return sqrtf(x*x + y*y + z*z);
  }

  float length_sq(void) {
    return (x*x + y*y + z*z);
  }

  void normalize(void) {
    float il = 1.0f / length();
    x *= il;
    y *= il;
    z *= il;
  }

};

//----------------------------------------------------------------------

MIP_Vector3  MIP_Vector3_add(MIP_Vector3 v0, MIP_Vector3 v1)  { MIP_Vector3 r = { v0.x + v1.x, v0.y + v1.y, v0.z + v1.z }; return r; }
MIP_Vector3  MIP_Vector3_sub(MIP_Vector3 v0, MIP_Vector3 v1)  { MIP_Vector3 r = { v0.x - v1.x, v0.y - v1.y, v0.z - v1.z }; return r; }
MIP_Vector3  MIP_Vector3_mul(MIP_Vector3 v0, MIP_Vector3 v1)  { MIP_Vector3 r = { v0.x * v1.x, v0.y * v1.y, v0.z * v1.z }; return r; }
MIP_Vector3  MIP_Vector3_div(MIP_Vector3 v0, MIP_Vector3 v1)  { MIP_Vector3 r = { v0.x / v1.x, v0.y / v1.y, v0.z / v1.z }; return r; }

MIP_Vector3  MIP_Vector3_add(MIP_Vector3 v, float f)   { MIP_Vector3 r = { v.x + f, v.y + f, v.z + f }; return r; }
MIP_Vector3  MIP_Vector3_sub(MIP_Vector3 v, float f)   { MIP_Vector3 r = { v.x - f, v.y - f, v.z - f }; return r; }
MIP_Vector3  MIP_Vector3_mul(MIP_Vector3 v, float f)   { MIP_Vector3 r = { v.x * f, v.y * f, v.z * f }; return r; }
MIP_Vector3  MIP_Vector3_div(MIP_Vector3 v, float f)   { MIP_Vector3 r = { v.x / f, v.y / f, v.z / f }; return r; }

float MIP_Vector3_dot(MIP_Vector3 v0, MIP_Vector3 v1) {
  return (v0.x * v1.x)
       + (v0.y * v1.y)
       + (v0.z * v1.z);
}

MIP_Vector3 MIP_Vector3_cross(MIP_Vector3 v0, MIP_Vector3 v1) {
  return MIP_Vector3(
    (v0.y * v1.z) - (v0.z * v1.y),
    (v0.z * v1.x) - (v0.x * v1.z),
    (v0.x * v1.y) - (v0.y * v1.x)
  );
}

float MIP_Vector3_length(MIP_Vector3 v) {
  return sqrtf( (v.x*v.x) + (v.y*v.y) + (v.z*v.z) );
}

float MIP_Vector3_length_sq(MIP_Vector3 v) {
  return ( (v.x*v.x) + (v.y*v.y) + (v.z*v.z) );
}

MIP_Vector3 MIP_Vector3_normalize(MIP_Vector3 v) {
  float fi = 1.0f / MIP_Vector3_length(v);
  return MIP_Vector3(
    v.x * fi,
    v.y * fi,
    v.z * fi
  );
}

//----------------------------------------------------------------------
#endif
