#ifndef mip_attributes_included
#define mip_attributes_included
//----------------------------------------------------------------------

//------------------------------
// win32
// dllexport, dllimport, cdecl, stdcall
//------------------------------

#ifdef MIP_LINUX
  #define __MIP_EXPORT
  #define __MIP_IMPORT
  #define __MIP_CDECL
  #define __MIP_STDCALL
#endif

#ifdef MIP_WIN32
  #define __MIP_EXPORT  __attribute__ ((dllexport))
  #define __MIP_IMPORT  __attribute__ ((dllimport))
  #define __MIP_CDECL   __attribute__ ((cdecl))
  #define __MIP_STDCALL __attribute__ ((stdcall))
#endif

//------------------------------
// linux
// dll public/local (-fvisibility=hidden)
//------------------------------

#ifdef MIP_LINUX
  #define __MIP_PUBLIC __attribute__ ((visibility ("default")))
  #define __MIP_LOCAL  __attribute__ ((visibility ("hidden")))
  #undef  __MIP_EXPORT
  #define __MIP_EXPORT __MIP_PUBLIC
#endif

#ifdef MIP_WIN32
  #define __MIP_PUBLIC
  #define __MIP_LOCAL
#endif

//------------------------------
// attributes
//------------------------------

#define __MIP_ALIAS(name)         __attribute__ ((alias (name)))
#define __MIP_ALIGNED(x)          __attribute__ ((aligned (x)))
#define __MIP_ALIGNEDMAX          __attribute__ ((aligned))
#define __MIP_ALLOC_ALIGN(x)      __attribute__ ((alloc_align(x)))
#define __MIP_ALLOC_SIZE1(x)      __attribute__ ((alloc_size(x)))
#define __MIP_ALLOC_SIZE2(x,y)    __attribute__ ((alloc_size(x,y)))
#define __MIP_ALWAYS_INLINE       inline __attribute__ ((always_inline))
#define __MIP_ASM_VOLATILE        __asm__ __volatile__
#define __MIP_CLEANUP(x)          __attribute__ ((__cleanup__(x)))
#define __MIP_COLD                __attribute__ ((cold))
#define __MIP_CONSTRUCTOR         __attribute__ ((constructor))
//#define __MIP_CONSTRUCTOR_PRI(x)  __attribute__ ((constructor(x)))
#define __MIP_DEPRECATED          __attribute__ ((deprecated))
#define __MIP_DESTRUCTOR          __attribute__ ((destructor))
#define __MIP_ERROR(message)      __attribute__ ((error(message)))
#define __MIP_EXTERNC             extern "C"
#define __MIP_FLATTEN             __attribute__ ((flatten))
#define __MIP_HOT                 __attribute__ ((hot))
#define __MIP_HOTINLINE           ___always_inline __hot __optimize(3)
#define __MIP_INIT_PRIORITY(x)    __attribute__ ((init_priority(x)))
#define __MIP_MALLOC              __attribute__ ((malloc))
#define __MIP_MAY_ALIAS           __attribute__ ((may_alias))
#define __MIP_NO_REORDER          __attribute__ ((no_reorder))
#define __MIP_NOINLINE            __attribute__ ((noinline))
#define __MIP_NORETURN            __attribute__ ((noreturn))
#define __MIP_OPTIMIZE(level)     __attribute__ ((optimize (level)))
#define __MIP_PACKED              __attribute__ ((packed))
#define __MIP_PURE                __attribute__ ((pure))
#define __MIP_SHARED              __attribute__ ((shared))
#define __MIP_SECTION(name)       __attribute__ ((section (name)))
#define __MIP_SELECTANY           __attribute__ ((selectany))
#define __MIP_TARGET(value)       __attribute__ ((target(value)))
#define __MIP_TRANSPARENT_UNION   __attribute__ ((transparent_union))
#define __MIP_USED                __attribute__ ((used))
#define __MIP_UNUSED              __attribute__ ((unused))
#define __MIP_VECTOR(x)           __attribute__ ((vector_size (x)))
#define __MIP_VOLATILE            __volatile__
#define __MIP_VISIBILITY(type)    __attribute__ ((visibility(type)))
#define __MIP_WARNING(message)    __attribute__ ((warning(message)))
#define __MIP_WEAK                __attribute__ ((weak))
#define __MIP_WEAKREF(name)       __attribute__ ((weakref(name)))

#define __MIP_LIKELY(expr)        __builtin_expect(!!(expr),1)
#define __MIP_UNLIKELY(expr)      __builtin_expect(!!(expr),0)

//------------------------------

//#define __MIP_LIKELY(expr)        (expr)
//#define __MIP_UNLIKELY(expr)      (expr)

//----------------------------------------------------------------------
#endif
