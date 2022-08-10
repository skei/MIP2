#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_editor.h"
#include "gui/widgets/mip_widgets.h"

#include "sa_botage_process.h"
#include "sa_botage_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_botage_editor
: public MIP_Editor {

  friend class sa_botage_plugin;

//------------------------------
private:
//------------------------------

  MIP_WaveformWidget* MWaveform = nullptr;

//------------------------------
public:
//------------------------------

  sa_botage_editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, MIP_ParameterArray AParameters)
  : MIP_Editor(AListener,AWidth,AHeight) {

    setWindowFillBackground(false);

    // background

    MIP_ColorWidget* background = new MIP_ColorWidget( MIP_DRect(), MIP_COLOR_GRAY );
    appendChildWidget(background);
    background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

    // waveform

    MWaveform = new MIP_WaveformWidget(MIP_DRect(10,10,480,100));
    MWaveform->Layout.alignment = MIP_WIDGET_ALIGN_PARENT;
    background->appendChildWidget(MWaveform);

    // beats/slices

    MIP_SliderWidget* w_beats = new MIP_SliderWidget(MIP_DRect(10,120,235,20), "Beats", 4);
    background->appendChildWidget(w_beats);
    connectWidget( AParameters[PAR_NUM_BEATS], w_beats);

    MIP_SliderWidget* w_slices = new MIP_SliderWidget(MIP_DRect(255,120,235,20), "Slices", 2);
    background->appendChildWidget(w_slices);
    connectWidget( AParameters[PAR_NUM_SLICES], w_slices);

    // knob2

    MIP_Knob2Widget* knob1 = new MIP_Knob2Widget(MIP_DRect(10,150,42,74), "Knob", 0.5);
    background->appendChildWidget(knob1);
    connectWidget( AParameters[PAR_TEST], knob1);
  }

  //----------

  virtual ~sa_botage_editor() {
    //MIP_PRINT;
  }

//------------------------------
public:
//------------------------------

  void timer_update(sa_botage_process* process) {
    //MIP_Assert(MWaveform);
    if (MIsEditorOpen) {
      MWaveform->setBuffer(process->MLeftBuffer,process->MBufferLength);
      MWaveform->setNumGrid(process->par_num_beats);
      MWaveform->setNumSubGrid(process->par_num_slices);
      // marker 0 write pos
      MWaveform->setMarkerActive(0,true);
      MWaveform->setMarkerPos(0,process->MWritePos);
      MWaveform->setMarkerColor(0, MIP_Color(1,0,0,1) );
      MWaveform->setMarkerWidth(0,1);
      // marker 1 read pos
      MWaveform->setMarkerActive(1,true);
      MWaveform->setMarkerPos(1,process->MReadPos);
      MWaveform->setMarkerColor(1, MIP_Color(0,1,0,1) );
      MWaveform->setMarkerWidth(1,1);
      if (process->MRange) {
        // area 1 range
        MWaveform->setAreaActive(0,false);
        MWaveform->setAreaActive(1,true);
        MWaveform->setAreaStart(1,process->MRangeStart);
        MWaveform->setAreaLength(1,process->MRangeLength);
        MWaveform->setAreaColor(1, MIP_Color(0,0.5,0,0.3) );
        if (process->MLoop) {
          // area 2 loop
          MWaveform->setAreaActive(2,true);
          MWaveform->setAreaStart(2,process->MLoopStart);
          MWaveform->setAreaLength(2,process->MLoopLength);
          MWaveform->setAreaColor(2, MIP_Color(0,1,0,0.3) );
        }
        else {
          MWaveform->setAreaActive(2,false);
        }
      }
      else {
        MWaveform->setAreaActive(1,false);
        MWaveform->setAreaActive(2,false);
        // area 0 current slice
        MWaveform->setAreaActive(0,true);
        MWaveform->setAreaStart(0,process->MSliceStart);
        MWaveform->setAreaLength(0,process->MSliceLength);
        MWaveform->setAreaColor(0, MIP_Color(0,0,0,0.4) );
      }
      MWaveform->redraw();//do_widget_redraw(MWaveform);
    }
  }


};


//----------------------------------------------------------------------
#endif
