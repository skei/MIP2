#ifndef mip_point_included
#define mip_point_included
//----------------------------------------------------------------------

template <typename T>
class MIP_Point {

public:

  union {
    struct { T x,y; };
    struct { T w,h; };
    struct { T horiz,vert; };
  };

public:

  MIP_Point() {
    x = 0;
    y = 0;
  }

  MIP_Point(T v) {
    x = v;
    y = v;
  }

  MIP_Point(T x_, T y_) {
    x = x_;
    y = y_;
  }

  //MIP_Point(MIP_Point<T> APoint) {}

public:

//------------------------------
public:
//------------------------------

  MIP_Point<T>& operator += (MIP_Point<T> P) {
    add(P);
    return *this;
  }

  MIP_Point<T>& operator -= (MIP_Point<T> P) {
    sub(P);
    return *this;
  }

//  MIP_Rect<T>& operator += (MIP_Point<T> P) {
//    add(P);
//    return *this;
//  }
//
//  MIP_Rect<T>& operator -= (MIP_Point<T> P) {
//    sub(P);
//    return *this;
//  }

//------------------------------
public:
//------------------------------

  void scale(T AScale) {
    x *= AScale;
    y *= AScale;
  }

  void scale(T sx, T sy) {
    x *= sx;
    y *= sy;
  }

  void scale(MIP_Point<T> s) {
    x *= s.x;
    y *= s.y;
  }

  //----------

  void add(T AValue) {
    x += AValue;
    y += AValue;
  }

  void add(T ax, T ay) {
    x += ax;
    y += ay;
  }

  void sub(T AValue) {
    x -= AValue;
    y -= AValue;
  }

  void sub(T ax, T ay) {
    x -= ax;
    y -= ay;
  }

};

//----------------------------------------------------------------------

typedef MIP_Point<float>    MIP_FPoint;
typedef MIP_Point<double>   MIP_DPoint;
typedef MIP_Point<int32_t>  MIP_IPoint;

//----------------------------------------------------------------------
#endif
