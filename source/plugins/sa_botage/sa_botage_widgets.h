#ifndef sa_botage_widgets_included
#define sa_botage_widgets_included
//----------------------------------------------------------------------

#include "gui/mip_widgets.h"
//#include "sa_botage_logo.h"

//----------------------------------------------------------------------

const char* txt_buttons[] = {
  "1", "2", "3", "4", "5", "6", "7", "8",
  "9", "10", "11", "12", "13", "14", "15", "16"
};


//----------------------------------------------------------------------

//class sa_botage_header
//: public MIP_ImageWidget {
//public:
//  sa_botage_header(MIP_FRect ARect,MIP_Drawable* ATarget,MIP_Color ABackground)
//  : MIP_ImageWidget(ARect) {
//    setImage(ATarget,sa_botage_logo,sa_botage_logo_size,ABackground);
//  }
//};

//----------------------------------------------------------------------

class sa_botage_waveform
: public MIP_WaveformWidget {
public:
  sa_botage_waveform(MIP_FRect ARect=MIP_FRect())
  : MIP_WaveformWidget(ARect) {
    setBackgroundColor(MIP_Color(0.45));
    setWaveColor(MIP_Color(0.8));
    setGridColor(MIP_Color(0.35));
    setGridSubColor(MIP_Color(0.2));
    setZeroColor(MIP_Color(0.75));
    setNumMarkers(2);
    setMarker(0, 0.0, MIP_COLOR_BRIGHT_RED);         // writepos
    setMarker(1, 0.0, MIP_COLOR_BRIGHT_GREEN);       // readpos
    setNumAreas(3);
    setArea(0, 0.0, 0.0, MIP_Color(0.0, 0.5, 0.0));  // range
    setArea(1, 0.0, 0.0, MIP_Color(0.0, 0.4, 0.0));  // loop
    setArea(2, 0.0, 0.0, MIP_Color(0.4, 0.4, 0.4));  // slice
  }
};

//----------

class sa_botage_text_header1
: public MIP_TextWidget {
public:
  sa_botage_text_header1(MIP_FRect ARect, const char* AText="")
  : MIP_TextWidget(ARect,AText) {
    setFillBackground(true);
    setBackgroundColor(0.4);
    setDrawBorder(false);
    setTextColor(0.8);
    setTextAlignment(MIP_TEXT_ALIGN_CENTER);
  }
};

//----------

class sa_botage_text_header2
: public MIP_TextWidget {
public:
  sa_botage_text_header2(MIP_FRect ARect, const char* AText="")
  : MIP_TextWidget(ARect,AText) {
    setFillBackground(false);
    //setBackgroundColor(0.3);
    setDrawBorder(true);
    setBorderColor(0.2);
    setBorderEdges(MIP_EDGE_BOTTOM);
    setTextColor(0.9);
    setTextAlignment(MIP_TEXT_ALIGN_CENTER);
  }
};

//----------

class sa_botage_text_small
: public MIP_TextWidget {
public:
  sa_botage_text_small(MIP_FRect ARect, const char* AText="")
  : MIP_TextWidget(ARect,AText) {
    setTextColor(0.2);
  }
};

//----------

class sa_botage_text_small2
: public MIP_TextWidget {
public:
  sa_botage_text_small2(MIP_FRect ARect, const char* AText="")
  : MIP_TextWidget(ARect,AText) {
    setTextColor(0.2);
    setTextAlignment(MIP_TEXT_ALIGN_RIGHT);
  }
};

//----------

class sa_botage_buttons
: public MIP_ButtonRowWidget {
public:
  sa_botage_buttons(MIP_FRect ARect, uint32_t ANum)
  : MIP_ButtonRowWidget(ARect, ANum, txt_buttons, MIP_BUTTON_ROW_MULTI) {
    setValueIsBits(true,ANum);
  }
};

//----------

class sa_botage_qslider
: public MIP_SliderWidget {
public:
  sa_botage_qslider(MIP_FRect ARect, uint32_t ANum)
  : MIP_SliderWidget(ARect) {
    setQuantize();
    setQuantizeSteps(ANum);
  }
};

//----------

class sa_botage_slider
: public MIP_SliderWidget {
public:
  sa_botage_slider(MIP_FRect ARect)
  : MIP_SliderWidget(ARect) {
    setDrawText(true);
    setDrawLabel(true);
    setTextColor(0.2);
    setLabelColor(0.3);
//    setDigits(0);
    setTextOffset(MIP_FRect(2,2,5,2));
    //setSnap(true);
    //setSnapPos(0.5);
    //setSnapDist(0.1);
    //setSnapMode(0);
  }
};

//----------

class sa_botage_slider2
: public MIP_SliderWidget {
public:
  sa_botage_slider2(MIP_FRect ARect)
  : MIP_SliderWidget(ARect) {
    setDrawText(true);
    setDrawLabel(true);
    setTextColor(0.2);
    setLabelColor(0.3);
//    setDigits(0);
    setTextOffset(MIP_FRect(2,2,5,2));
    //setSnap(true);
    //setSnapPos(0.5);
    //setSnapDist(0.1);
    //setSnapMode(0);
  }
};

//----------

class sa_botage_knob
: public MIP_KnobWidget {
public:
  sa_botage_knob(MIP_FRect ARect)
  : MIP_KnobWidget(ARect) {
    setBackgroundColor(0.5);
    //setDrawText(false);
    //setDrawLabel(true);
    //setLabelColor(0.2);
    //setDigits(0);
    //setTextOffset(MIP_FRect(2,2,5,2));
  }
};

//----------

class sa_botage_switch
: public MIP_ButtonWidget {
public:
  sa_botage_switch(MIP_FRect ARect)
  : MIP_ButtonWidget(ARect) {
    setFillBackground(true);
    setDrawBorder(false);
    setIsToggle(true);
    setDrawText(true);
//    setDrawParamText(false);
    setText("single","multi");
    setTextColors(0.8,0.8);
    setBackgroundColors(0.40, 0.40);
    setTextAlignment(MIP_TEXT_ALIGN_CENTER,MIP_TEXT_ALIGN_CENTER);

    //setTextOffset(MIP_FRect(2,2,5,2));
    //setDrawLabel(true);
    //setLabelColor(0.2);
    //setDigits(0);
  }
};


//----------------------------------------------------------------------
#endif

