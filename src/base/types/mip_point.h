#ifndef mip_point_included
#define mip_point_included
//----------------------------------------------------------------------

template <typename T>
class MIP_Point {

public:

  union {
    struct { T x,y; };
    struct { T w,h; };
    struct { T hor,vert; };
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

};

//----------------------------------------------------------------------

typedef MIP_Point<float>   MIP_FPoint;
typedef MIP_Point<int32_t> MIP_IPoint;
//----------------------------------------------------------------------
#endif
