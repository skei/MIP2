#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_editor.h"
#include "gui/widgets/mip_widgets.h"

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

  sa_botage_editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight)
  : MIP_Editor(AListener,AWidth,AHeight) {

    setWindowFillBackground(false);

    MIP_ColorWidget* background = new MIP_ColorWidget( MIP_DRect(), MIP_COLOR_GRAY );
    appendChildWidget(background);
    background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

    MWaveform = new MIP_WaveformWidget(MIP_DRect(10,10,480,100));
    MWaveform->Layout.alignment = MIP_WIDGET_ALIGN_PARENT;
    background->appendChildWidget(MWaveform);

  }

  //----------

  virtual ~sa_botage_editor() {
  }

};


//----------------------------------------------------------------------
#endif
