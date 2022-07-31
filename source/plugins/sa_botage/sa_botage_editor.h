#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_editor.h"
#include "gui/widgets/mip_widgets.h"

#include "sa_botage_process.h"

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

  }

  //----------

  virtual ~sa_botage_editor() {
  }

//------------------------------
public:
//------------------------------

  void timer_callback(sa_botage_process* process) {
    MWaveform->setBuffer(process->MBuffer,process->MBufferLength);
    MWaveform->setNumGrid(process->par_num_beats);
    MWaveform->setNumSubGrid(process->par_num_slices);

    MWaveform->setMarkerActive(0,true);
    MWaveform->setMarkerPos(0,process->MWritePos);
    MWaveform->setMarkerColor(0, MIP_Color(1,0,0,1) );
    MWaveform->setMarkerWidth(0,1);

    MWaveform->setMarkerActive(1,true);
    MWaveform->setMarkerPos(1,process->MReadPos);
    MWaveform->setMarkerColor(1, MIP_Color(0,1,0,1) );
    MWaveform->setMarkerWidth(1,1);

    MWaveform->setAreaActive(0,true);
    MWaveform->setAreaStart(0,process->MCurrentSliceStart);
    MWaveform->setAreaLength(0,process->MCurrentSliceLength);
    MWaveform->setAreaColor(0, MIP_Color(0,0,0,0.3) );

    MWaveform->redraw();//do_widget_redraw(MWaveform);
  }


};


//----------------------------------------------------------------------
#endif
