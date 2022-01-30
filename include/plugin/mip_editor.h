#ifndef mip_editor_included
#define mip_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/clap/mip_clap_plugin.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Editor {

//------------------------------
public:
//------------------------------

  MIP_Editor() {
  }

  //----------

  virtual ~MIP_Editor() {
  }

//------------------------------
public:
//------------------------------

  virtual bool attach(void* display, void* window) { return false; }
  virtual void detach() {}
  virtual void open() {}
  virtual void close() {}

};

//----------------------------------------------------------------------
#endif
