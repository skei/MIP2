#ifndef sa_ts1_widgets_included
#define sa_ts1_widgets_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------

class sa_ts1_VoiceWidget
: public MIP_PanelWidget {

//------------------------------
public:

  sa_ts1_VoiceWidget(MIP_FRect ARect)
  : MIP_PanelWidget(ARect) {
    setFillBackground(false);
  }

  //----------

  virtual ~sa_ts1_VoiceWidget() {
  }

//------------------------------
public:
//------------------------------

  uint32_t voice_state[NUM_VOICES]  = {0};

  //----------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) final {
    float w = MRect.w / NUM_VOICES;
    MIP_FRect rect = MIP_FRect(MRect.x,MRect.y,w, MRect.h);
    MIP_Color color = MIP_COLOR_DARK_GRAY;
    for (uint32_t i=0; i<NUM_VOICES; i++) {
      APainter->rectangle(rect);
      switch (voice_state[i]) {
        case MIP_VOICE_OFF:       color = MIP_COLOR_BLACK;        break;
        case MIP_VOICE_WAITING:   color = MIP_COLOR_LIGHT_YELLOW; break;
        case MIP_VOICE_PLAYING:   color = MIP_COLOR_BRIGHT_GREEN; break;
        case MIP_VOICE_RELEASED:  color = MIP_COLOR_GREEN;        break;
        case MIP_VOICE_FINISHED:  color = MIP_COLOR_WHITE;        break;
      }
      APainter->setColor(color);
      APainter->fillPath();
      rect.x += w; // 8
    }
  }

};

//----------------------------------------------------------------------
#endif

