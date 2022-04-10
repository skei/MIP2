#ifndef sa_botage_widgets_included
#define sa_botage_widgets_included
//----------------------------------------------------------------------

#include "gui/kode_widgets.h"
#include "sa_botage_logo.h"

//----------------------------------------------------------------------

const char* txt_buttons[] = {
  "1", "2", "3", "4", "5", "6", "7", "8",
  "9", "10", "11", "12", "13", "14", "15", "16"
};


//----------------------------------------------------------------------

class sa_botage_header
: public KODE_ImageWidget {
public:
  sa_botage_header(KODE_FRect ARect,KODE_Drawable* ATarget,KODE_Color ABackground)
  : KODE_ImageWidget(ARect) {
    setImage(ATarget,sa_botage_logo,sa_botage_logo_size,ABackground);
  }
};

//----------------------------------------------------------------------

class sa_botage_waveform
: public KODE_WaveformWidget {
public:
  sa_botage_waveform(KODE_FRect ARect=KODE_FRect())
  : KODE_WaveformWidget(ARect) {
    setBackgroundColor(KODE_Color(0.45));
    setWaveColor(KODE_Color(0.8));
    setGridColor(KODE_Color(0.35));
    setGridSubColor(KODE_Color(0.2));
    setZeroColor(KODE_Color(0.75));
    setNumMarkers(2);
    setMarker(0, 0.0, KODE_COLOR_BRIGHT_RED);         // writepos
    setMarker(1, 0.0, KODE_COLOR_BRIGHT_GREEN);       // readpos
    setNumAreas(3);
    setArea(0, 0.0, 0.0, KODE_Color(0.0, 0.5, 0.0));  // range
    setArea(1, 0.0, 0.0, KODE_Color(0.0, 0.4, 0.0));  // loop
    setArea(2, 0.0, 0.0, KODE_Color(0.4, 0.4, 0.4));  // slice
  }
};

//----------

class sa_botage_text_header1
: public KODE_TextWidget {
public:
  sa_botage_text_header1(KODE_FRect ARect, const char* AText="")
  : KODE_TextWidget(ARect,AText) {
    setFillBackground(true);
    setBackgroundColor(0.4);
    setDrawBorder(false);
    setTextColor(0.8);
    setTextAlignment(KODE_TEXT_ALIGN_CENTER);
  }
};

//----------

class sa_botage_text_header2
: public KODE_TextWidget {
public:
  sa_botage_text_header2(KODE_FRect ARect, const char* AText="")
  : KODE_TextWidget(ARect,AText) {
    setFillBackground(false);
    //setBackgroundColor(0.3);
    setDrawBorder(true);
    setBorderColor(0.2);
    setBorderEdges(KODE_EDGE_BOTTOM);
    setTextColor(0.9);
    setTextAlignment(KODE_TEXT_ALIGN_CENTER);
  }
};

//----------

class sa_botage_text_small
: public KODE_TextWidget {
public:
  sa_botage_text_small(KODE_FRect ARect, const char* AText="")
  : KODE_TextWidget(ARect,AText) {
    setTextColor(0.2);
  }
};

//----------

class sa_botage_text_small2
: public KODE_TextWidget {
public:
  sa_botage_text_small2(KODE_FRect ARect, const char* AText="")
  : KODE_TextWidget(ARect,AText) {
    setTextColor(0.2);
    setTextAlignment(KODE_TEXT_ALIGN_RIGHT);
  }
};

//----------

class sa_botage_buttons
: public KODE_ButtonRowWidget {
public:
  sa_botage_buttons(KODE_FRect ARect, uint32_t ANum)
  : KODE_ButtonRowWidget(ARect, ANum, txt_buttons, KODE_BUTTON_ROW_MULTI) {
    setValueIsBits(true,ANum);
  }
};

//----------

class sa_botage_qslider
: public KODE_SliderWidget {
public:
  sa_botage_qslider(KODE_FRect ARect, uint32_t ANum)
  : KODE_SliderWidget(ARect) {
    setQuantize();
    setQuantizeSteps(ANum);
  }
};

//----------

class sa_botage_slider
: public KODE_SliderWidget {
public:
  sa_botage_slider(KODE_FRect ARect)
  : KODE_SliderWidget(ARect) {
    setDrawText(true);
    setDrawLabel(true);
    setTextColor(0.2);
    setLabelColor(0.3);
    setDigits(0);
    setTextOffset(KODE_FRect(2,2,5,2));
    //setSnap(true);
    //setSnapPos(0.5);
    //setSnapDist(0.1);
    //setSnapMode(0);
  }
};

//----------

class sa_botage_slider2
: public KODE_SliderWidget {
public:
  sa_botage_slider2(KODE_FRect ARect)
  : KODE_SliderWidget(ARect) {
    setDrawText(true);
    setDrawLabel(true);
    setTextColor(0.2);
    setLabelColor(0.3);
    setDigits(0);
    setTextOffset(KODE_FRect(2,2,5,2));
    //setSnap(true);
    //setSnapPos(0.5);
    //setSnapDist(0.1);
    //setSnapMode(0);
  }
};

//----------

class sa_botage_knob
: public KODE_KnobWidget {
public:
  sa_botage_knob(KODE_FRect ARect)
  : KODE_KnobWidget(ARect) {
    //setDrawText(false);
    //setDrawLabel(true);
    //setLabelColor(0.2);
    //setDigits(0);
    //setTextOffset(KODE_FRect(2,2,5,2));
  }
};

//----------

class sa_botage_switch
: public KODE_ButtonWidget {
public:
  sa_botage_switch(KODE_FRect ARect)
  : KODE_ButtonWidget(ARect) {
    setFillBackground(true);
    setDrawBorder(false);
    setIsToggle(true);
    setDrawText(true);
    setDrawParamText(false);
    setText("single","multi");
    setTextColor(0.8,0.8);
    setBackgroundColor(0.40, 0.40);
    setTextAlignment(KODE_TEXT_ALIGN_CENTER,KODE_TEXT_ALIGN_CENTER);

    //setTextOffset(KODE_FRect(2,2,5,2));
    //setDrawLabel(true);
    //setLabelColor(0.2);
    //setDigits(0);
  }
};


//----------------------------------------------------------------------
#endif

