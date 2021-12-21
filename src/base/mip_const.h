#ifndef mip_const_included
#define mip_const_included
//----------------------------------------------------------------------

//#include "base/mip_types.h"
//#include <inttypes.h> // uint8_t

//----------------------------------------------------------------------

//const uint8_t MIP_NULL_ID[16]    = {0};
const char    MIP_HEX_TABLE[]    = "0123456789ABCDEF";
const char    MIP_CSYMBOLS[]     = "________________________________________________0123456789_______ABCDEFGHIJKLMNOPQRSTUVWXYZ______abcdefghijklmnopqrstuvwxyz_____";
const char    MIP_NULL_STRING[]  = "";

// 4         4    5         5
// 123456789abcdef0123456789a
// abcdefghijklmnopqrstuvwxyz
// 5f = _

#define MIP_MAGIC        0x4d5f0000    // 'M_'   // + 0x0000
#define MIP_MAGIC_USER   0x6d5f0000    // 'm_'   // + 0x0000
#define MIP_MAGIC_M_PL   0x4d5f504c    // M_PL   // plugin
#define MIP_MAGIC_M_ED   0x4d5f4544    // M_ED   // editor

//#ifdef MIP_CPP11
//  #define nullptr nullptr
//#else
//  #define nullptr 0
//#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define MIP_ALIGNMENT_SIMD           16
#define MIP_ALIGNMENT_CACHE          64

#define MIP_AUDIO_MAX_EVENTS         256
#define MIP_AUDIO_MAX_MIDI_EVENTS    256
#define MIP_AUDIO_MAX_NODES          256
#define MIP_AUDIO_TICKSIZE           16
#define MIP_AUDIO_NUM_TICK_BUFFERS   16

#define MIP_BITMAP_RASTER_MAX_HEIGHT 2048

#define MIP_GUI_DBLCLICK_MS          200

#define MIP_MAX_PATH_LENGTH          512
#define MIP_MAX_STRING_LENGTH        256

//----------------------------------------------------------------------
// midi
//----------------------------------------------------------------------

#define MIP_MIDI_NOTE_OFF            0x80
#define MIP_MIDI_NOTE_ON             0x90
#define MIP_MIDI_POLY_AFTERTOUCH     0xA0
#define MIP_MIDI_CONTROL_CHANGE      0xB0
#define MIP_MIDI_PROGRAM_CHANGE      0xC0
#define MIP_MIDI_CHANNEL_AFTERTOUCH  0xD0
#define MIP_MIDI_PITCHBEND           0xE0
#define MIP_MIDI_SYS                 0xF0

//----------------------------------------------------------------------
// math
//----------------------------------------------------------------------

#define MIP_PI             3.1415926535
#define MIP_PI2            (MIP_PI * 2.0)
#define MIP_PI05           (MIP_PI * 0.5)
#define MIP_INVPI          (1.0 / MIP_PI)
#define MIP_INVPI2         (1.0 / MIP_PI2)
#define MIP_INVPI05        (1.0 / MIP_PI05)

#define MIP_TINY           (0.001)
#define MIP_VERY_TINY      (0.00001)

#define MIP_INV360F        (1.0 / 360.0)
#define MIP_INV12F         (1.0 / 12.0)
#define MIP_INV16F         (1.0 / 16.0)
#define MIP_INV32F         (1.0 / 32.0)
#define MIP_INV127F        (1.0 / 127.0)
#define MIP_INV255F        (1.0 / 255.0)
#define MIP_INV256F        (1.0 / 256.0)
#define MIP_INV8192F       (1.0 / 8192.0)

#define MIP_SQRT2          sqrtf(2.0f)

//----------------------------------------------------------------------
// type sizes
//----------------------------------------------------------------------

#define MIP_CHAR_BIT       8
#define MIP_CHAR_MAX       0x7f                /* 127 */
#define MIP_SHORT_MAX      0x7fff              /* 32767 */
#define MIP_INT32_MAX      0x7fffffff          /* 2147483647 */
#define MIP_UINT32_MAX     0xffffffff          /* 8589934591U */
#define MIP_INT64_MAX      0x7fffffffffffffff  /* 9223372036854775807LL */
#define MIP_UINT64_MAX     0x7fffffffffffffff  /* 18446744073709551615ULL */
#define MIP_FLOAT_MAX      3.40282347e+38F
#define MIP_FLOAT_MIN      1.17549435E-38F
#define MIP_FLOAT_EPSILON  1.19209290E-7F
//#define MIP_FLOAT_DENORM   1.40129846E-45F
#define MIP_DOUBLE_MAX     1.7976931348623157e+308
#define MIP_DOUBLE_MIN     2.2250738585072014E-308
#define MIP_DOUBLE_EPSILON 2.2204460492503131E-16
#define MIP_DOUBLE_DENORM  4.9406564584124654E-324

#ifdef MIP_32BIT
  #define MIP_INT_MAX      MIP_INT32_MAX   // 2147483647L
  #define MIP_UINT_MAX     MIP_UINT32_MAX  // 8589934591UL
#endif

#ifdef MIP_64BIT
  #define MIP_INT_MAX      MIP_INT64_MAX   // 9223372036854775807L
  #define MIP_UINT_MAX     MIP_UINT64_MAX  // 18446744073709551615UL
#endif

#define MIP_RANDMAX        RAND_MAX  // 2147483647
#define MIP_INVRANDMAX     (1.0 / (float)MIP_RANDMAX)

//----------------------------------------------------------------------
#endif
