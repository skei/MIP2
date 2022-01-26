#ifndef sa_botage_instance_included
#define sa_botage_instance_included
//----------------------------------------------------------------------

#include "kode.h"
//#include "plugin/base/kode_base_editor.h"
#include "plugin/kode_editor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_process_context.h"
#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {

//------------------------------
private:
//------------------------------

  myEditor*   MEditor = KODE_NULL;
  myProcess   MProcess;

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

//------------------------------
public:
//------------------------------

  void update_waveform() {
    if (MEditor) {
      KODE_WaveformWidget* waveform = MEditor->getWaveformWidget();
      if (waveform) {
        waveform->setBuffer(MProcess.MBuffer);
        waveform->setBufferSize(MProcess.MBufferLength);
        waveform->setNumGrid(MProcess.par_BufferNumBeats * MProcess.par_BufferNumSlices);
        waveform->setNumGridSub(MProcess.par_BufferNumSlices);
        float bufferlength = MProcess.MBufferLength;
        if (bufferlength > 0.0) {
          float invbufferlength = 1.0 / bufferlength;

          float writepos = MProcess.MBufferPos * invbufferlength;
          waveform->setMarkerPos(0,writepos);

          float readpos = MProcess.MReadPos * invbufferlength;
          waveform->setMarkerPos(1,readpos);

          if (MProcess.MRangeActive) {
            float rangestart  = (MProcess.MRangeStart) * invbufferlength;
            float rangelength = (MProcess.MRangeNumSlices * MProcess.MSliceLength) * invbufferlength;;
            waveform->setAreaVisible(0,true);
            waveform->setAreaPosSize(0,rangestart,rangelength);
            if (MProcess.MLoopActive) {
              float loopstart   = (MProcess.MLoopStart) * invbufferlength;
              float looplength  = MProcess.MLoopLength * invbufferlength;
              waveform->setAreaVisible(1,true);
              waveform->setAreaPosSize(1,loopstart,looplength);
            }
            waveform->setAreaVisible(2,false);
          }
          else {
            waveform->setAreaVisible(0,false);
            waveform->setAreaVisible(1,false);
            float slicestart = (MProcess.MSliceCounter * MProcess.MSliceLength) * invbufferlength;
            float slicelength = MProcess.MSliceLength * invbufferlength;
            waveform->setAreaVisible(2,true);
            waveform->setAreaPosSize(2,slicestart,slicelength);
          }

        } // buffer length > 0
        waveform->redraw();
      } // waveform
    } // editor
  }

//------------------------------
public:
//------------------------------

  void on_plugin_prepare(float ASamplerate, uint32_t ABlocksize) final {
    MProcess.start();
  }

  //----------

  #ifdef KODE_PLUGIN_EXE
    #define UPDATE update_waveform();
  #else
    #define UPDATE
  #endif

  //

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch (AIndex) {
      case P_BUFFER_NUM_BEATS:      MProcess.par_BufferNumBeats     = AValue; MProcess.start(); UPDATE; break;
      case P_BUFFER_NUM_SLICES:     MProcess.par_BufferNumSlices    = AValue; MProcess.start(); UPDATE; break;
      case P_ENV_LOOP_ATT:          MProcess.par_EnvLoopAtt         = AValue * 0.01; break;
      case P_ENV_LOOP_DEC:          MProcess.par_EnvLoopDec         = AValue * 0.01; break;
      case P_ENV_SLICE_ATT:         MProcess.par_EnvSliceAtt        = AValue * 0.01; break;
      case P_ENV_SLICE_DEC:         MProcess.par_EnvSliceDec        = AValue * 0.01; break;
      case P_REPEAT_PROB:           MProcess.par_RepeatProb         = AValue; break;
      case P_REPEAT_SLICE_BITS:     MProcess.par_RepeatSliceBits    = AValue; break;
      case P_REPEAT_SPLIT_BITS:     MProcess.par_RepeatSplitBits    = AValue; break;
      case P_LOOPSIZE_RANGE_PROB:   MProcess.par_LoopsizeRangeProb  = AValue; break;
      case P_LOOPSIZE_RANGE_MIN:    MProcess.par_LoopsizeRangeMin   = AValue * 0.01; break;
      case P_LOOPSIZE_RANGE_MAX:    MProcess.par_LoopsizeRangeMax   = AValue * 0.01; break;
      case P_LOOPSIZE_LOOP_PROB:    MProcess.par_LoopsizeLoopProb   = AValue; break;
      case P_LOOPSIZE_LOOP_MIN:     MProcess.par_LoopsizeLoopMin    = AValue * 0.01; break;
      case P_LOOPSIZE_LOOP_MAX:     MProcess.par_LoopsizeLoopMax    = AValue * 0.01; break;
      case P_LOOPSPEED_RANGE_PROB:  MProcess.par_LoopspeedRangeProb = AValue; break;
      case P_LOOPSPEED_RANGE_MIN:   MProcess.par_LoopspeedRangeMin  = AValue * 0.01; break;
      case P_LOOPSPEED_RANGE_MAX:   MProcess.par_LoopspeedRangeMax  = AValue * 0.01; break;
      case P_LOOPSPEED_LOOP_PROB:   MProcess.par_LoopspeedLoopProb  = AValue; break;
      case P_LOOPSPEED_LOOP_MIN:    MProcess.par_LoopspeedLoopMin   = AValue * 0.01; break;
      case P_LOOPSPEED_LOOP_MAX:    MProcess.par_LoopspeedLoopMax   = AValue * 0.01; break;
      case P_OFFSET_RANGE_PROB:     MProcess.par_OffsetRangeProb    = AValue; break;
      case P_OFFSET_RANGE_MIN:      MProcess.par_OffsetRangeMin     = AValue; break;
      case P_OFFSET_RANGE_MAX:      MProcess.par_OffsetRangeMax     = AValue; break;
      case P_OFFSET_LOOP_PROB:      MProcess.par_OffsetLoopProb     = AValue; break;
      case P_OFFSET_LOOP_MIN:       MProcess.par_OffsetLoopMin      = AValue; break;
      case P_OFFSET_LOOP_MAX:       MProcess.par_OffsetLoopMax      = AValue; break;
      case P_REVERSE_RANGE_PROB:    MProcess.par_ReverseRangeProb   = AValue; break;
      case P_REVERSE_LOOP_PROB:     MProcess.par_ReverseLoopProb    = AValue; break;
      case P_FX_MULTI:              MProcess.par_FXMulti            = AValue; break;
      case P_FX_RANGE_PROB:         MProcess.par_FXRangeProb        = AValue; break;
      case P_FX_RANGE_MIN:          MProcess.par_FXRangeMin         = AValue * 0.01; break;
      case P_FX_RANGE_MAX:          MProcess.par_FXRangeMax         = AValue * 0.01; break;
      case P_FX_LOOP_PROB:          MProcess.par_FXLoopProb         = AValue; break;
      case P_FX_LOOP_MIN:           MProcess.par_FXLoopMin          = AValue * 0.01; break;
      case P_FX_LOOP_MAX:           MProcess.par_FXLoopMax          = AValue * 0.01; break;
      case P_FX_FILTER_PROB:        MProcess.par_FXFilterProb       = AValue; break;
      case P_FX_FILTER_FREQ:        MProcess.par_FXFilterFreq       = AValue; break;
      case P_FX_FILTER_BW:          MProcess.par_FXFilterBW         = AValue; break;
      case P_FX_FILTER_TYPE:        MProcess.par_FXFilterType       = AValue; break;
    }
  }

  //

  #undef UPDATE

  //----------

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    MProcess.process(AContext);
  }

//------------------------------
//
//------------------------------

  #ifndef KODE_NO_GUI

  KODE_BaseEditor* on_plugin_openEditor(void* AParent) final {
    MEditor = new myEditor(this,AParent);
    return MEditor;
  }

  //----------

  void  on_plugin_closeEditor(KODE_BaseEditor* AEditor) final {
    if (MEditor) {
      delete MEditor;
      MEditor = KODE_NULL;
    }
  }

  //----------

  void on_plugin_updateEditor(KODE_BaseEditor* AEditor) final {
    if (MEditor) {
      update_waveform();
    } // editor
  }

  #endif

};

//----------------------------------------------------------------------
#endif
