#ifndef mip_descriptor_included
#define mip_descriptor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/clap/mip_clap.h"
#include "plugin/mip_parameter.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_AudioPort {};
class MIP_NotePort {};

typedef MIP_Array<MIP_AudioPort*> MIP_AudioPortArray;
typedef MIP_Array<MIP_NotePort*> MIP_NotePortArray;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_Descriptor {

public:

  char            name[256]         = {0};
  char            author[256]       = {0};
  char            email[256]        = {0};
  char            url[256]          = {0};
  char            description[256]  = {0};
  MParameterArray parameters        = {};
  MAudioPortArray audio_inputs      = {};
  MAudioPortArray audio_outputs     = {};
  MNotePortArray  note_inputs       = {};
  MNotePortArray  note_outputs      = {};

  const clap_plugin_descriptor_t  clap_descriptor = {0};

};

//----------------------------------------------------------------------
#endif
