#ifndef mip_debug_assert_included
#define mip_debug_assert_included
//----------------------------------------------------------------------

#ifdef MIP_DEBUG_ASSERT

  #include <assert.h>

  // c++11 : static_assert(x,"msg");
  //#include <cassert>

  #define MIP_Assert(x)      { if (!(x)) { MIP_Print("MIP_Assert : %s\n",#x); abort(); } }
  #define MIP_StaticAssert   static_assert

#else // MIP_DEBUG_ASSERT

  #define MIP_Assert(x)          {}
  #define MIP_StaticAssert(x,y)  {}

#endif // MIP_DEBUG_ASSERT

//----------------------------------------------------------------------
#endif
