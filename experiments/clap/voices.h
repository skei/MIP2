#ifndef voices_included
#define voices_included
//----------------------------------------------------------------------

//class Voice {
//};

//----------------------------------------------------------------------

class Voices {
public:
  void strike(uint32_t key, uint32_t chan, float vel) {}  // note on
  void lift(uint32_t key, uint32_t chan, float vel) {}    // note off
  void press() {}                                         // aftertouch
  void slide() {}                                         // 'brightness'
  void bend() {}                                          // pitchbend
};

//----------------------------------------------------------------------
#endif
