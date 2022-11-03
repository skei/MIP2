#ifndef sa_tyr_widgets_included
#define sa_tyr_widgets_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_widgets.h"

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class sa_tyr_VoiceWidget
: public MIP_PanelWidget {

//------------------------------
public:

  sa_tyr_VoiceWidget(MIP_DRect ARect)
  : MIP_PanelWidget(ARect) {
    setFillBackground(false);
  }

  //----------

  virtual ~sa_tyr_VoiceWidget() {
  }

//------------------------------
public:
//------------------------------

  uint32_t voice_state[SA_TYR_NUM_VOICES]  = {0};

  //----------

  void on_widget_paint(MIP_PaintContext* AContext) final {
    MIP_Painter* painter = AContext->painter;
    float w = MRect.w / SA_TYR_NUM_VOICES;
    MIP_DRect rect = MIP_DRect(MRect.x,MRect.y,w, MRect.h);
    MIP_Color color = MIP_COLOR_DARK_GRAY;
    for (uint32_t i=0; i<SA_TYR_NUM_VOICES; i++) {

      painter->beginPath();
      painter->rectangle(rect);

      switch (voice_state[i]) {
        case MIP_VOICE_OFF:       color = MIP_COLOR_BLACK;        break;
        case MIP_VOICE_WAITING:   color = MIP_COLOR_LIGHT_YELLOW; break;
        case MIP_VOICE_PLAYING:   color = MIP_COLOR_BRIGHT_GREEN; break;
        case MIP_VOICE_RELEASED:  color = MIP_COLOR_GREEN;        break;
        case MIP_VOICE_FINISHED:  color = MIP_COLOR_WHITE;        break;
      }

      painter->fillColor(color);
      painter->fill();

      rect.x += w; // 8
    }
  }

};

//----------------------------------------------------------------------
#endif

