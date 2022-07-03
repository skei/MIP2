#ifndef mip_cairo_utils_included
#define mip_cairo_utils_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/cairo/mip_cairo.h"

//----------------------------------------------------------------------

#ifdef MIP_DEBUG

  #define MIP_CHECK_CAIRO_ERROR(cairo)                                    \
    {                                                                     \
      cairo_status_t status = cairo_status(cairo);                        \
      if (status != CAIRO_STATUS_SUCCESS) {                               \
        MIP_Print("cairo status: %s\n",cairo_status_to_string(status));   \
      }                                                                   \
    }

  //----------

  #define MIP_CHECK_CAIRO_SURFACE_ERROR(surface)                          \
    {                                                                     \
      cairo_status_t status = cairo_surface_status(surface);              \
      if (status != CAIRO_STATUS_SUCCESS) {                               \
        MIP_Print("cairo status: %s\n",cairo_status_to_string(status));   \
      }                                                                   \
    }

  //----------

  #define MIP_CHECK_CAIRO_REFCOUNT(cairo)                                 \
    {                                                                     \
      uint32_t refcount = cairo_get_reference_count(cairo);               \
      MIP_Print("refcount: %i\n",refcount);                               \
    }

  //----------

  #define MIP_CHECK_CAIRO_SURFACE_REFCOUNT(cairo)                         \
    {                                                                     \
      uint32_t refcount = cairo_surface_get_reference_count(cairo);       \
      MIP_Print("srf refcount: %i\n",refcount);                           \
    }

#else

  #define MIP_CHECK_CAIRO_ERROR(cairo) {}
  #define MIP_CHECK_CAIRO_SURFACE_ERROR(surface) {}
  #define MIP_CHECK_CAIRO_REFCOUNT(cairo) {}
  #define MIP_CHECK_CAIRO_SURFACE_REFCOUNT(surface) {}

#endif

//----------------------------------------------------------------------
#endif
