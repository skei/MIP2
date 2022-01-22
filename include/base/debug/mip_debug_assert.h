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




#if 0

// parts by lubomir i ivanon, for axonlib
// used with permission

/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

/*
  ISO C/C++ compatible runtime and static assertion for GCC (4.4.x)
  no external dependencies.
*/

#ifndef axAssert_included
#define axAssert_included

/* for c89 compat mainly if the header is used elsewhere */
#ifdef __GNUC_STDC_INLINE__
  #define _AXSA_INLINE inline
#else
  #define _AXSA_INLINE
#endif

#ifdef AX_DEBUG /* AX_DEBUG */

  /*
    axAssert()
    runtime assertion
  */
  unsigned int _axAssert () __attribute__ (( noinline ));
  unsigned int _axAssert (const unsigned int e, const char* file,
    const unsigned int line, const char* e_str)
  {
    if (!e)
    { _trace("### axAssert: " << file << ", " << line << ", (" << e_str << ")");
      __builtin_exit(0); }
    return 1;
  }
  #define axAssert(e) _axAssert((e), __FILE__, __LINE__, #e)

  /*
    axStaticAssert()
    compile time assertion ( requires GCC 4.4.x )
  */
  #if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 4)
    _AXSA_INLINE unsigned int __axSA_true (void) { return 1; }
    unsigned int __axSA (void)
      __attribute__ (( error("### axStaticAssert") ))
      __attribute__ (( optimize(0) ));
    #define axStaticAssert(e) ( (!(e)) ? __axSA(): __axSA_true() )
  #else
    /*
      switch can also be used. it does not generate extra code and is much
      more portable, but not very useful at the same time.
    */
    #define axStaticAssert(e) switch(0) { case 0: case (e): ; }
  #endif

#else /* AX_DEBUG */

  _AXSA_INLINE unsigned int __axSA (void) { return 0; }
  #define axAssert(e)       __axSA()
  #define axStaticAssert(e) __axSA()

#endif /* AX_DEBUG */

#endif /* axAssert_included */

#endif // 0
