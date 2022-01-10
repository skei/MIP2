#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#ifndef KODE_NO_GUI

#include "kode.h"
#include "plugin/kode_instance.h"
//#include "plugin/kode_editor.h"
//#include "plugin/kode_widget.h"
#include "gui/kode_widgets.h"

#include "sa_botage_widgets.h"

const char* txt_1to8[8] = {
  "1", "2", "3", "4", "5", "6", "7", "8"
};

//----------------------------------------------------------------------

class myEditor
: public KODE_Editor {

  myEditor(KODE_EditorListener* AListener, KODE_Descriptor* ADescriptor) {
  : KODE_Editor(AListener,ADescriptor) {
  }

  virtual ~myEditor() {
  }

};

//----------------------------------------------------------------------

#if 0

//class myEditor
//: public KODE_Editor {

class myEditorWidget
: public KODE_Widget {

//------------------------------
public:
//------------------------------

  sa_botage_header*   wdg_Header              = KODE_NULL;
  sa_botage_waveform* wdg_Waveform            = KODE_NULL;
  sa_botage_qslider*  wdg_BufferNumBeats      = KODE_NULL;
  sa_botage_qslider*  wdg_BufferNumSlices     = KODE_NULL;

  sa_botage_slider*   wdg_EnvLoopAtt          = KODE_NULL;
  sa_botage_slider*   wdg_EnvLoopDec          = KODE_NULL;
  sa_botage_slider*   wdg_EnvSliceAtt         = KODE_NULL;
  sa_botage_slider*   wdg_EnvSliceDec         = KODE_NULL;

  sa_botage_knob*     wdg_RepeatProb          = KODE_NULL;
  sa_botage_buttons*  wdg_RepeatSliceBits     = KODE_NULL;
  sa_botage_buttons*  wdg_RepeatSplitBits     = KODE_NULL;

  sa_botage_knob*     wdg_LoopsizeRangeProb   = KODE_NULL;
  sa_botage_slider*   wdg_LoopsizeRangeMin    = KODE_NULL;
  sa_botage_slider*   wdg_LoopsizeRangeMax    = KODE_NULL;
  sa_botage_knob*     wdg_LoopsizeLoopProb    = KODE_NULL;
  sa_botage_slider*   wdg_LoopsizeLoopMin     = KODE_NULL;
  sa_botage_slider*   wdg_LoopsizeLoopMax     = KODE_NULL;

  sa_botage_knob*     wdg_LoopspeedRangeProb  = KODE_NULL;
  sa_botage_slider*   wdg_LoopspeedRangeMin   = KODE_NULL;
  sa_botage_slider*   wdg_LoopspeedRangeMax   = KODE_NULL;
  sa_botage_knob*     wdg_LoopspeedLoopProb   = KODE_NULL;
  sa_botage_slider*   wdg_LoopspeedLoopMin    = KODE_NULL;
  sa_botage_slider*   wdg_LoopspeedLoopMax    = KODE_NULL;

  sa_botage_knob*     wdg_OffsetRangeProb     = KODE_NULL;
  sa_botage_slider2*  wdg_OffsetRangeMin      = KODE_NULL;
  sa_botage_slider2*  wdg_OffsetRangeMax      = KODE_NULL;
  sa_botage_knob*     wdg_OffsetLoopProb      = KODE_NULL;
  sa_botage_slider2*  wdg_OffsetLoopMin       = KODE_NULL;
  sa_botage_slider2*  wdg_OffsetLoopMax       = KODE_NULL;

  sa_botage_knob*     wdg_ReverseRangeProb    = KODE_NULL;
  sa_botage_knob*     wdg_ReverseLoopProb     = KODE_NULL;

  sa_botage_switch*   wdg_FXMulti             = KODE_NULL;
  sa_botage_knob*     wdg_FXRangeProb         = KODE_NULL;
  sa_botage_slider*   wdg_FXRangeMin          = KODE_NULL;
  sa_botage_slider*   wdg_FXRangeMax          = KODE_NULL;
  sa_botage_knob*     wdg_FXLoopProb          = KODE_NULL;
  sa_botage_slider*   wdg_FXLoopMin           = KODE_NULL;
  sa_botage_slider*   wdg_FXLoopMax           = KODE_NULL;

  sa_botage_knob*       wdg_FXFilterProb      = KODE_NULL;
  KODE_SliderWidget*    wdg_FXFilterFreq      = KODE_NULL;
  KODE_SliderWidget*    wdg_FXFilterBW        = KODE_NULL;
  KODE_SelectorWidget*  wdg_FXFilterType      = KODE_NULL;

  KODE_MenuWidget*      wdg_FilterMenu        = KODE_NULL;

//------------------------------
public:
//------------------------------

  //myEditor(KODE_BaseInstance* AInstance, void* AParent=KODE_NULL)
  //: KODE_Editor(AInstance,AParent) {

  myEditorWidget(KODE_Instance* AInstance, KODE_FRect ARect)
  : KODE_Widget(ARect) {

    KODE_Descriptor* descriptor = AInstance->getDescriptor();

//    setFillBackground(true);
//    setBackgroundColor(0.5f);
//    setTitle("SA_BOTAGE v0.0.10");

    //-----

    wdg_FilterMenu = new KODE_MenuWidget( KODE_FRect() );
    wdg_FilterMenu->appendMenuItem("Off");
    wdg_FilterMenu->appendMenuItem("Lowpass");
    wdg_FilterMenu->appendMenuItem("Highpass");
    wdg_FilterMenu->appendMenuItem("Bandpass");
    wdg_FilterMenu->appendMenuItem("Notch");
    wdg_FilterMenu->setItemSize(150,20);
    wdg_FilterMenu->setItemLayout(1,5);
    wdg_FilterMenu->setMenuMirror(false,true);

    //-----

//    wdg_Header              = (sa_botage_header*)   appendWidget( new sa_botage_header(       KODE_FRect(   0,   0,  400, 60 ), this, KODE_Color(0.5) ));
//                                                    appendWidget( new sa_botage_text_small2(  KODE_FRect( 436,  10,  100, 15 ), descriptor->getVersionText()               ));

    wdg_Waveform            = (sa_botage_waveform*) appendWidget( new sa_botage_waveform(     KODE_FRect(  10,  70,  526, 70 )                        ));

    wdg_BufferNumBeats      = (sa_botage_qslider*)  appendWidget( new sa_botage_qslider(      KODE_FRect(  10, 155,  260, 20 ), 8                     ));
    wdg_BufferNumSlices     = (sa_botage_qslider*)  appendWidget( new sa_botage_qslider(      KODE_FRect( 276, 155,  260, 20 ), 8                     ));

                                                    appendWidget( new sa_botage_text_header1( KODE_FRect(  10, 200,  240, 20 ),"REPEAT"               ));

    wdg_RepeatProb          = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         KODE_FRect(  10, 250,   70, 70 )                        ));

                                                    appendWidget( new sa_botage_text_small(   KODE_FRect( 100, 230,  150, 15 ),"Slices"               ));
    wdg_RepeatSliceBits     = (sa_botage_buttons*)  appendWidget( new sa_botage_buttons(      KODE_FRect( 100, 250,  150, 20 ), 8                     ));
                                                    appendWidget( new sa_botage_text_small(   KODE_FRect( 100, 275,  150, 15 ),"Split"                ));
    wdg_RepeatSplitBits     = (sa_botage_buttons*)  appendWidget( new sa_botage_buttons(      KODE_FRect( 100, 295,  150, 20 ), 8                     ));

                                                    appendWidget( new sa_botage_text_header1( KODE_FRect( 270, 200,  126, 20 ),"Range"                ));
                                                    appendWidget( new sa_botage_text_header1( KODE_FRect( 410, 200,  126, 20 ),"Loop"                 ));

                                                    appendWidget( new sa_botage_text_header2( KODE_FRect( 270, 230,  264, 20 ),"Size"                 ));
    wdg_LoopsizeRangeProb   = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         KODE_FRect( 270, 260,   40, 40 )                        ));
    wdg_LoopsizeRangeMin    = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       KODE_FRect( 320, 260,   74, 16 )                        ));
    wdg_LoopsizeRangeMax    = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       KODE_FRect( 320, 280,   74, 16 )                        ));
    wdg_LoopsizeLoopProb    = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         KODE_FRect( 410, 260,   40, 40 )                        ));
    wdg_LoopsizeLoopMin     = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       KODE_FRect( 460, 260,   74, 16 )                        ));
    wdg_LoopsizeLoopMax     = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       KODE_FRect( 460, 280,   74, 16 )                        ));

                                                    appendWidget( new sa_botage_text_header2( KODE_FRect( 270, 310,  264, 20 ),"Speed"                ));
    wdg_LoopspeedRangeProb  = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         KODE_FRect( 270, 340,   40, 40 )                        ));
    wdg_LoopspeedRangeMin   = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       KODE_FRect( 320, 340,   74, 16 )                        ));
    wdg_LoopspeedRangeMax   = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       KODE_FRect( 320, 360,   74, 16 )                        ));
    wdg_LoopspeedLoopProb   = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         KODE_FRect( 410, 340,   40, 40 )                        ));
    wdg_LoopspeedLoopMin    = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       KODE_FRect( 460, 340,   74, 16 )                        ));
    wdg_LoopspeedLoopMax    = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       KODE_FRect( 460, 360,   74, 16 )                        ));

                                                    appendWidget( new sa_botage_text_header2( KODE_FRect( 270, 390,  264, 20 ),"Offset"               ));
    wdg_OffsetRangeProb     = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         KODE_FRect( 270, 420,   40, 40 )                        ));
    wdg_OffsetRangeMin      = (sa_botage_slider2*)  appendWidget( new sa_botage_slider(       KODE_FRect( 320, 420,   74, 16 )                        ));
    wdg_OffsetRangeMax      = (sa_botage_slider2*)  appendWidget( new sa_botage_slider(       KODE_FRect( 320, 440,   74, 16 )                        ));
    wdg_OffsetLoopProb      = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         KODE_FRect( 410, 420,   40, 40 )                        ));
    wdg_OffsetLoopMin       = (sa_botage_slider2*)  appendWidget( new sa_botage_slider(       KODE_FRect( 460, 420,   74, 16 )                        ));
    wdg_OffsetLoopMax       = (sa_botage_slider2*)  appendWidget( new sa_botage_slider(       KODE_FRect( 460, 440,   74, 16 )                        ));

                                                    appendWidget( new sa_botage_text_header2( KODE_FRect( 270, 470,  264, 20 ),"Reverse"              ));
    wdg_ReverseRangeProb    = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         KODE_FRect( 270, 500,   40, 40 )                        ));
    wdg_ReverseLoopProb     = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         KODE_FRect( 410, 500,   40, 40 )                        ));

                                                    appendWidget( new sa_botage_text_header2( KODE_FRect( 270, 550,  264, 20 ),"FX"                   ));
    wdg_FXMulti             = (sa_botage_switch*)   appendWidget( new sa_botage_switch(       KODE_FRect( 490, 550,   44, 17 )                        ));
    wdg_FXRangeProb         = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         KODE_FRect( 270, 580,   40, 40 )                        ));
    wdg_FXRangeMin          = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       KODE_FRect( 320, 580,   74, 16 )                        ));
    wdg_FXRangeMax          = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       KODE_FRect( 320, 600,   74, 16 )                        ));
    wdg_FXLoopProb          = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         KODE_FRect( 410, 580,   40, 40 )                        ));
    wdg_FXLoopMin           = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       KODE_FRect( 460, 580,   74, 16 )                        ));
    wdg_FXLoopMax           = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       KODE_FRect( 460, 600,   74, 16 )                        ));

                                                  //appendWidget( new sa_botage_text_header1( KODE_FRect(  10, 396,  240, 20 ),"ENVELOPES"            ));

                                                    appendWidget( new sa_botage_text_header2( KODE_FRect(  10, 336,  240, 20 ),"Loop Env"             ));
    wdg_EnvLoopAtt          = (sa_botage_slider*)   appendWidget( new sa_botage_slider2(      KODE_FRect(  10, 366,  115, 20 )                        ));
    wdg_EnvLoopDec          = (sa_botage_slider*)   appendWidget( new sa_botage_slider2(      KODE_FRect( 130, 366,  115, 20 )                        ));

                                                    appendWidget( new sa_botage_text_header2( KODE_FRect(  10, 396,  240, 20 ),"Slice Env"            ));
    wdg_EnvSliceAtt         = (sa_botage_slider*)   appendWidget( new sa_botage_slider2(      KODE_FRect(  10, 426,  115, 20 )                        ));
    wdg_EnvSliceDec         = (sa_botage_slider*)   appendWidget( new sa_botage_slider2(      KODE_FRect( 130, 426,  115, 20 )                        ));

                                                    appendWidget( new sa_botage_text_header2( KODE_FRect( 10, 520,  240, 20 ),"Filter"               ));

    wdg_FXFilterType        = (KODE_SelectorWidget*)appendWidget( new KODE_SelectorWidget(    KODE_FRect( 10, 550,  240, 16 )                        ));
    wdg_FXFilterProb        = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         KODE_FRect( 10, 570,   40, 40 )                        ));
    wdg_FXFilterFreq        = (KODE_SliderWidget*)  appendWidget( new KODE_SliderWidget(      KODE_FRect( 60, 570,  190, 16 )                        ));
    wdg_FXFilterBW          = (KODE_SliderWidget*)  appendWidget( new KODE_SliderWidget(      KODE_FRect( 60, 590,  190, 16 )                        ));

    //----------

    appendWidget(wdg_FilterMenu);

    wdg_FXFilterType->setMenu(wdg_FilterMenu);
    wdg_FXFilterType->setDrawTriangle(true);
    wdg_FXFilterType->setSelected(1);
    wdg_FXFilterType->setDrawParamText(false);

    connectParameter(wdg_BufferNumBeats,P_BUFFER_NUM_BEATS);
    connectParameter(wdg_BufferNumSlices,P_BUFFER_NUM_SLICES);
    connectParameter(wdg_EnvLoopAtt,P_ENV_LOOP_ATT);
    connectParameter(wdg_EnvLoopDec,P_ENV_LOOP_DEC);
    connectParameter(wdg_EnvSliceAtt,P_ENV_SLICE_ATT);
    connectParameter(wdg_EnvSliceDec,P_ENV_SLICE_DEC);
    connectParameter(wdg_RepeatProb,P_REPEAT_PROB);
    connectParameter(wdg_RepeatSliceBits,P_REPEAT_SLICE_BITS);
    connectParameter(wdg_RepeatSplitBits,P_REPEAT_SPLIT_BITS);
    connectParameter(wdg_LoopsizeRangeProb,P_LOOPSIZE_RANGE_PROB);
    connectParameter(wdg_LoopsizeRangeMin,P_LOOPSIZE_RANGE_MIN);
    connectParameter(wdg_LoopsizeRangeMax,P_LOOPSIZE_RANGE_MAX);
    connectParameter(wdg_LoopsizeLoopProb,P_LOOPSIZE_LOOP_PROB);
    connectParameter(wdg_LoopsizeLoopMin,P_LOOPSIZE_LOOP_MIN);
    connectParameter(wdg_LoopsizeLoopMax,P_LOOPSIZE_LOOP_MAX);
    connectParameter(wdg_LoopspeedRangeProb,P_LOOPSPEED_RANGE_PROB);
    connectParameter(wdg_LoopspeedRangeMin,P_LOOPSPEED_RANGE_MIN);
    connectParameter(wdg_LoopspeedRangeMax,P_LOOPSPEED_RANGE_MAX);
    connectParameter(wdg_LoopspeedLoopProb,P_LOOPSPEED_LOOP_PROB);
    connectParameter(wdg_LoopspeedLoopMin,P_LOOPSPEED_LOOP_MIN);
    connectParameter(wdg_LoopspeedLoopMax,P_LOOPSPEED_LOOP_MAX);
    connectParameter(wdg_OffsetRangeProb,P_OFFSET_RANGE_PROB);
    connectParameter(wdg_OffsetRangeMin,P_OFFSET_RANGE_MIN);
    connectParameter(wdg_OffsetRangeMax,P_OFFSET_RANGE_MAX);
    connectParameter(wdg_OffsetLoopProb,P_OFFSET_LOOP_PROB);
    connectParameter(wdg_OffsetLoopMin,P_OFFSET_LOOP_MIN);
    connectParameter(wdg_OffsetLoopMax,P_OFFSET_LOOP_MAX);
    connectParameter(wdg_ReverseRangeProb,P_REVERSE_RANGE_PROB);
    connectParameter(wdg_ReverseLoopProb,P_REVERSE_LOOP_PROB);
    connectParameter(wdg_FXMulti,P_FX_MULTI);
    connectParameter(wdg_FXRangeProb,P_FX_RANGE_PROB);
    connectParameter(wdg_FXRangeMin,P_FX_RANGE_MIN);
    connectParameter(wdg_FXRangeMax,P_FX_RANGE_MAX);
    connectParameter(wdg_FXLoopProb,P_FX_LOOP_PROB);
    connectParameter(wdg_FXLoopMin,P_FX_LOOP_MIN);
    connectParameter(wdg_FXLoopMax,P_FX_LOOP_MAX);
    connectParameter(wdg_FXFilterProb,P_FX_FILTER_PROB);
    connectParameter(wdg_FXFilterFreq,P_FX_FILTER_FREQ);
    connectParameter(wdg_FXFilterBW,P_FX_FILTER_BW);
    connectParameter(wdg_FXFilterType,P_FX_FILTER_TYPE);

  }

//------------------------------
public:
//------------------------------

  KODE_WaveformWidget* getWaveformWidget() {
    return wdg_Waveform;
  }

};

#endif

#endif // 0

//----------------------------------------------------------------------
#endif
