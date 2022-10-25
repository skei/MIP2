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
: public MIP_Editor
, public MIP_MenuListener {

  friend class sa_botage_plugin;

//------------------------------
private:
//------------------------------

  MIP_WaveformWidget* MWaveform     = nullptr;
  MIP_MenuWidget*     MFXFilterMenu = nullptr;

  const char* txt18[8] = { "1", "2", "3", "4", "5", "6", "7", "8" };
  const char* brow_text[3] = { "Prob", "sequence", "Perform" };

//------------------------------
public:
//------------------------------

  //#define HEADER_HEIGHT   240
  //#define FOOTER_HEIGHT   20
  //#define WAVEFORM_HEIGHT 20

  sa_botage_editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight, MIP_ParameterArray AParameters)
  : MIP_Editor(AListener,AWidth,AHeight) {

    MIP_Window* window = getWindow();
    window->setWindowFillBackground(false);

    //aspect

//    double initial_aspect = EDITOR_WIDTH / EDITOR_HEIGHT;
//    if (initial_aspect > 0) {
//      if (AHeight > 0) {
//        double aspect = AWidth / AHeight;
//        if (aspect >= initial_aspect) {
//          AWidth = AHeight * initial_aspect;
//        }
//        else {
//          AHeight = AWidth / initial_aspect;
//        }
//      } // h > 0
//    } // aspect > 0
//
//    window->setWidgetSize(AWidth,AHeight);
//    MEditorWidth = AWidth;
//    MEditorHeight = AHeight;

    //MIP_Print("AWidth %i AHeight %i\n",AWidth,AHeight);

    setCanResizeEditor(true);
    //setResizeProportional(true);
    setResizeProportional(false);
    setProportionalSize(AWidth,AHeight);

    // filter menu

    MFXFilterMenu = new MIP_MenuWidget( MIP_DRect( 0, 0, 160,80), this );
    MFXFilterMenu->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    MFXFilterMenu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 0,160,20), "Off" ));
    MFXFilterMenu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,20,160,20), "LowPass" ));
    MFXFilterMenu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,40,160,20), "HighPass" ));
    MFXFilterMenu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,60,160,20), "BandPass" ));
    //MFXFilterMenu->setDrawDropShadow(true);

    for (uint32_t i=0; i<MFXFilterMenu->getNumChildWidgets(); i++) {
      MIP_MenuItemWidget* w = (MIP_MenuItemWidget*)MFXFilterMenu->getChildWidget(i);
      w->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      w->setTextSize(-0.7);
    }

    // background

    //MIP_Widget* back_background = new MIP_Widget( MIP_DRect(EDITOR_WIDTH,EDITOR_HEIGHT) );
    //back_background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    //window->appendChildWidget(back_background);

    MIP_PanelWidget* background = new MIP_PanelWidget( MIP_DRect(EDITOR_WIDTH,EDITOR_HEIGHT) );
    background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    background->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
//    background->Layout.aspectRatio = (double)AWidth / (double)AHeight;
    background->Layout.aspectRatio = (double)EDITOR_WIDTH / (double)EDITOR_HEIGHT;
    background->setFillBackground(true);
    background->setBackgroundColor(0.5);
    //back_background->appendChildWidget(background);
    window->appendChildWidget(background);

    // header

    MIP_PanelWidget* header = new MIP_PanelWidget(MIP_DRect( 10,10, 530,40 ));
    header->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    background->appendChildWidget(header);

    // footer

    MIP_PanelWidget* footer = new MIP_PanelWidget(MIP_DRect( 0,EDITOR_HEIGHT-20, EDITOR_WIDTH,20 ));
    footer->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    background->appendChildWidget(footer);

    //{
    //  MIP_SizerWidget* window_sizer = new MIP_SizerWidget(MIP_DRect(EDITOR_WIDTH-20,0,20,20),MIP_WIDGET_SIZER_WINDOW,window);
    //  window_sizer->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    //  footer->appendChildWidget(window_sizer);
    //}

    // waveform

    MWaveform = new MIP_WaveformWidget(MIP_DRect( 10,60, 530,70 ));
    MWaveform->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    background->appendChildWidget(MWaveform);

    // beats/slices

    MIP_SliderWidget* w_beats = new MIP_SliderWidget(MIP_DRect( 10,140, 260,20 ), "Beats", 4);
    w_beats->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    w_beats->setTextSize( -0.7 );
    w_beats->setValueSize( -0.7 );
    background->appendChildWidget(w_beats);
    connectWidget( AParameters[PAR_NUM_BEATS], w_beats );

    MIP_SliderWidget* w_slices = new MIP_SliderWidget(MIP_DRect( 280,140, 260,20 ), "Slices", 2);
    w_slices->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    w_slices->setTextSize( -0.7 );
    w_slices->setValueSize( -0.7 );
    background->appendChildWidget(w_slices);
    connectWidget( AParameters[PAR_NUM_SLICES], w_slices );

#if 0

    // tabs

    MIP_TabsWidget* tabs = new MIP_TabsWidget( MIP_DRect(10,180, 530, EDITOR_HEIGHT-180-10-15), 3 );
    tabs->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    background->appendChildWidget(tabs);

      MIP_PanelWidget* prob_page = new MIP_PanelWidget(0);
      prob_page->setDrawBorder(true);
      MIP_PanelWidget* perf_page = new MIP_PanelWidget(0);
      perf_page->setDrawBorder(true);
      MIP_PanelWidget* seq_page = new MIP_PanelWidget(0);
      seq_page->setDrawBorder(true);

    tabs->appendPage("Prob",prob_page);
    tabs->appendPage("Perform",perf_page);
    tabs->appendPage("Sequence",seq_page);
    tabs->selectPage(0);

#endif // 0

    MIP_ButtonRowWidget* brow = new MIP_ButtonRowWidget( MIP_DRect(10,180, 530,25), 3, brow_text, MIP_BUTTON_ROW_SINGLE );
    brow->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    brow->setTextSize(-0.5);
    background->appendChildWidget(brow);

    double TH = 50.0;

//#if 0

    // repeat

    MIP_PanelWidget* repeat_section = new MIP_PanelWidget(MIP_DRect( 10,180 + TH, 260,110) );
    repeat_section->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    background->appendChildWidget(repeat_section);

    {
      MIP_TextWidget* t0 = new MIP_TextWidget( MIP_DRect(0,0,260,15), "Range");
      t0->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      t0->setFillBackground(true);
      t0->setBackgroundColor(0.2);
      t0->setDrawBorder(false);
      t0->setTextSize(-0.9);
      t0->setTextColor(0.8);
      repeat_section->appendChildWidget(t0);

      MIP_KnobWidget* k = new MIP_KnobWidget( MIP_DRect(5,40,65,65), "qwe", 0.5);
      k->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      k->setFillBackground(false);
      k->setDrawBorder(false);
      //k->setTextSize(-0.9);
      //k->setTextColor(0.8);
      repeat_section->appendChildWidget(k);
      connectWidget( AParameters[PAR_RANGE_PROB], k );


      MIP_TextWidget* t1 = new MIP_TextWidget( MIP_DRect(80,30,150,15), "Slices");
      t1->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      t1->setFillBackground(false);
      t1->setDrawBorder(false);
      t1->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
      t1->setTextSize(-0.9);
      t1->setTextColor(0.2);
      repeat_section->appendChildWidget(t1);

      MIP_ButtonRowWidget* b1 = new MIP_ButtonRowWidget( MIP_DRect(80,45,150,15), 8, txt18, MIP_BUTTON_ROW_MULTI);
      b1->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      b1->setFillBackground(false);
      b1->setDrawBorder(false);
      b1->setTextSize(-0.8);
      //b1->setTextColor(0.2);
      repeat_section->appendChildWidget(b1);
      connectWidget( AParameters[PAR_RANGE_SLICE_COUNT], b1 );

      MIP_TextWidget* t2 = new MIP_TextWidget( MIP_DRect(80,65,150,15), "Loops");
      t2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      t2->setFillBackground(false);
      t2->setDrawBorder(false);
      t2->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
      t2->setTextSize(-0.9);
      t2->setTextColor(0.2);
      repeat_section->appendChildWidget(t2);

      MIP_ButtonRowWidget* b2 = new MIP_ButtonRowWidget( MIP_DRect(80,80,150,15), 8, txt18, MIP_BUTTON_ROW_MULTI);
      b2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      b2->setFillBackground(false);
      b2->setDrawBorder(false);
      b2->setTextSize(-0.8);
      //b2->setTextColor(0.2);
      repeat_section->appendChildWidget(b2);
      connectWidget( AParameters[PAR_RANGE_LOOP_COUNT], b2 );
    }

    // env

    MIP_PanelWidget* env_section = new MIP_PanelWidget(MIP_DRect( 10,300 + TH, 260,110) );
    env_section->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    background->appendChildWidget(env_section);

    {
      MIP_TextWidget* t0 = new MIP_TextWidget( MIP_DRect(0,0,260,15), "Envelopes");
      t0->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      t0->setFillBackground(true);
      t0->setBackgroundColor(0.2);
      t0->setDrawBorder(false);
      t0->setTextSize(-0.9);
      t0->setTextColor(0.8);
      env_section->appendChildWidget(t0);

      MIP_SliderWidget* s1 = new MIP_SliderWidget( MIP_DRect(10,25,115,15), "Loop Attack", 0);
      s1->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s1->setTextSize(-0.7);
      s1->setValueSize(-0.7);
      env_section->appendChildWidget(s1);
      connectWidget( AParameters[PAR_LOOP_ENV_ATTACK], s1 );

      MIP_SliderWidget* s2 = new MIP_SliderWidget( MIP_DRect(135,25,115,15), "Loop Decay", 0);
      s2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s2->setTextSize(-0.7);
      s2->setValueSize(-0.7);
      env_section->appendChildWidget(s2);
      connectWidget( AParameters[PAR_LOOP_ENV_DECAY], s2 );

      MIP_SliderWidget* s3 = new MIP_SliderWidget( MIP_DRect(10,45,115,15), "Slice Attack", 0);
      s3->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s3->setTextSize(-0.7);
      s3->setValueSize(-0.7);
      env_section->appendChildWidget(s3);
      connectWidget( AParameters[PAR_SLICE_ENV_ATTACK], s3 );

      MIP_SliderWidget* s4 = new MIP_SliderWidget( MIP_DRect(135,45,115,15), "Slice Decay", 0);
      s4->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s4->setTextSize(-0.7);
      s4->setValueSize(-0.7);
      env_section->appendChildWidget(s4);
      connectWidget( AParameters[PAR_SLICE_ENV_DECAY], s4 );

    }

    // filter

    MIP_PanelWidget* filter_section = new MIP_PanelWidget(MIP_DRect( 10,420 + TH, 260,110) );
    filter_section->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    background->appendChildWidget(filter_section);

    {
      MIP_TextWidget* t0 = new MIP_TextWidget( MIP_DRect(0,0,260,15), "FX Filter");
      t0->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      t0->setFillBackground(true);
      t0->setBackgroundColor(0.2);
      t0->setDrawBorder(false);
      t0->setTextSize(-0.9);
      t0->setTextColor(0.8);
      filter_section->appendChildWidget(t0);

      MIP_SelectorWidget* sel1 = new MIP_SelectorWidget( MIP_DRect(10,25,240,15), "Type", MFXFilterMenu );
      sel1->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      sel1->setTextSize(-0.9);
      filter_section->appendChildWidget(sel1);
      connectWidget( AParameters[PAR_FX_FILTER_TYPE], sel1 );


      MIP_KnobWidget* k = new MIP_KnobWidget( MIP_DRect(10,45,40,44), "", 0.5);
      k->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      k->setFillBackground(false);
      k->setDrawBorder(false);
      filter_section->appendChildWidget(k);
      connectWidget( AParameters[PAR_FX_FILTER_PROB], k );

      MIP_SliderWidget* s1 = new MIP_SliderWidget( MIP_DRect(60,45,190,15), "Freq", 0);
      s1->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s1->setTextSize(-0.7);
      s1->setValueSize(-0.7);
      filter_section->appendChildWidget(s1);
      connectWidget( AParameters[PAR_FX_FILTER_FREQ], s1 );

      MIP_SliderWidget* s2 = new MIP_SliderWidget( MIP_DRect(60,70,190,15), "BW", 0);
      s2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s2->setTextSize(-0.7);
      s2->setValueSize(-0.7);
      filter_section->appendChildWidget(s2);
      connectWidget( AParameters[PAR_FX_FILTER_BW], s2 );

    }

    // loop size

    MIP_PanelWidget* size_section = new MIP_PanelWidget(MIP_DRect( 280,180 + TH, 260,65) );
    size_section->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    background->appendChildWidget(size_section);

    {
      MIP_TextWidget* t0 = new MIP_TextWidget( MIP_DRect(0,0,260,15), "Loop Size");
      t0->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      t0->setFillBackground(true);
      t0->setBackgroundColor(0.3);
      t0->setDrawBorder(false);
      t0->setTextSize(-0.8);
      t0->setTextColor(0.7);
      size_section->appendChildWidget(t0);

      MIP_KnobWidget* k = new MIP_KnobWidget( MIP_DRect(10,20,40,40), "", 0.5);
      k->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      k->setFillBackground(false);
      k->setDrawBorder(false);
      size_section->appendChildWidget(k);
      connectWidget( AParameters[PAR_PROB_SIZE_PROB_RANGE], k );

      MIP_SliderWidget* s1 = new MIP_SliderWidget( MIP_DRect(60,20,65,15), "Min", 0);
      s1->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s1->setTextSize(-0.7);
      s1->setValueSize(-0.7);
      size_section->appendChildWidget(s1);
      connectWidget( AParameters[PAR_PROB_SIZE_MIN_RANGE], s1 );

      MIP_SliderWidget* s2 = new MIP_SliderWidget( MIP_DRect(60,40,65,15), "Max", 0);
      s2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s2->setTextSize(-0.7);
      s2->setValueSize(-0.7);
      size_section->appendChildWidget(s2);
      connectWidget( AParameters[PAR_PROB_SIZE_MAX_RANGE], s2 );

      MIP_KnobWidget* k2 = new MIP_KnobWidget( MIP_DRect(135,20,40,40), "", 0.5);
      k2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      k2->setFillBackground(false);
      k2->setDrawBorder(false);
      size_section->appendChildWidget(k2);
      connectWidget( AParameters[PAR_PROB_SIZE_PROB_LOOP], k2 );

      MIP_SliderWidget* s3 = new MIP_SliderWidget( MIP_DRect(185,20,65,15), "Min", 0);
      s3->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s3->setTextSize(-0.7);
      s3->setValueSize(-0.7);
      size_section->appendChildWidget(s3);
      connectWidget( AParameters[PAR_PROB_SIZE_MIN_LOOP], s3 );

      MIP_SliderWidget* s4 = new MIP_SliderWidget( MIP_DRect(185,40,65,15), "Max", 0);
      s4->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s4->setTextSize(-0.7);
      s4->setValueSize(-0.7);
      size_section->appendChildWidget(s4);
      connectWidget( AParameters[PAR_PROB_SIZE_MAX_LOOP], s4 );

    }

    // loop speed

    MIP_PanelWidget* speed_section = new MIP_PanelWidget(MIP_DRect( 280,255 + TH, 260,65) );
    speed_section->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    background->appendChildWidget(speed_section);

    {
      MIP_TextWidget* t0 = new MIP_TextWidget( MIP_DRect(0,0,260,15), "Loop Speed");
      t0->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      t0->setFillBackground(true);
      t0->setBackgroundColor(0.3);
      t0->setDrawBorder(false);
      t0->setTextSize(-0.8);
      t0->setTextColor(0.7);
      speed_section->appendChildWidget(t0);

      MIP_KnobWidget* k = new MIP_KnobWidget( MIP_DRect(10,20,40,40), "", 0.5);
      k->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      k->setFillBackground(false);
      k->setDrawBorder(false);
      speed_section->appendChildWidget(k);
      connectWidget( AParameters[PAR_PROB_SPEED_PROB_RANGE], k );

      MIP_SliderWidget* s1 = new MIP_SliderWidget( MIP_DRect(60,20,65,15), "Min", 0);
      s1->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s1->setTextSize(-0.7);
      s1->setValueSize(-0.7);
      speed_section->appendChildWidget(s1);
      connectWidget( AParameters[PAR_PROB_SPEED_MIN_RANGE], s1 );

      MIP_SliderWidget* s2 = new MIP_SliderWidget( MIP_DRect(60,40,65,15), "Max", 0);
      s2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s2->setTextSize(-0.7);
      s2->setValueSize(-0.7);
      speed_section->appendChildWidget(s2);
      connectWidget( AParameters[PAR_PROB_SPEED_MAX_RANGE], s2 );

      MIP_KnobWidget* k2 = new MIP_KnobWidget( MIP_DRect(135,20,40,40), "", 0.5);
      k2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      k2->setFillBackground(false);
      k2->setDrawBorder(false);
      speed_section->appendChildWidget(k2);
      connectWidget( AParameters[PAR_PROB_SPEED_PROB_LOOP], k2 );

      MIP_SliderWidget* s3 = new MIP_SliderWidget( MIP_DRect(185,20,65,15), "Min", 0);
      s3->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s3->setTextSize(-0.7);
      s3->setValueSize(-0.7);
      speed_section->appendChildWidget(s3);
      connectWidget( AParameters[PAR_PROB_SPEED_MIN_LOOP], s3 );

      MIP_SliderWidget* s4 = new MIP_SliderWidget( MIP_DRect(185,40,65,15), "Max", 0);
      s4->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s4->setTextSize(-0.7);
      s4->setValueSize(-0.7);
      speed_section->appendChildWidget(s4);
      connectWidget( AParameters[PAR_PROB_SPEED_MAX_LOOP], s4 );

    }

    // loop offset

    MIP_PanelWidget* offset_section = new MIP_PanelWidget(MIP_DRect( 280,330 + TH, 260,65) );
    offset_section->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    background->appendChildWidget(offset_section);

    {
      MIP_TextWidget* t0 = new MIP_TextWidget( MIP_DRect(0,0,260,15), "Loop Offset");
      t0->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      t0->setFillBackground(true);
      t0->setBackgroundColor(0.3);
      t0->setDrawBorder(false);
      t0->setTextSize(-0.8);
      t0->setTextColor(0.7);
      offset_section->appendChildWidget(t0);

      MIP_KnobWidget* k = new MIP_KnobWidget( MIP_DRect(10,20,40,40), "", 0.5);
      k->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      k->setFillBackground(false);
      k->setDrawBorder(false);
      offset_section->appendChildWidget(k);
      connectWidget( AParameters[PAR_PROB_OFFSET_PROB_RANGE], k );

      MIP_SliderWidget* s1 = new MIP_SliderWidget( MIP_DRect(60,20,65,15), "Min", 0);
      s1->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s1->setTextSize(-0.7);
      s1->setValueSize(-0.7);
      offset_section->appendChildWidget(s1);
      connectWidget( AParameters[PAR_PROB_OFFSET_MIN_RANGE], s1 );

      MIP_SliderWidget* s2 = new MIP_SliderWidget( MIP_DRect(60,40,65,15), "Max", 0);
      s2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s2->setTextSize(-0.7);
      s2->setValueSize(-0.7);
      offset_section->appendChildWidget(s2);
      connectWidget( AParameters[PAR_PROB_OFFSET_MAX_RANGE], s2 );

      MIP_KnobWidget* k2 = new MIP_KnobWidget( MIP_DRect(135,20,40,40), "", 0.5);
      k2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      k2->setFillBackground(false);
      k2->setDrawBorder(false);
      offset_section->appendChildWidget(k2);
      connectWidget( AParameters[PAR_PROB_OFFSET_PROB_LOOP], k2 );

      MIP_SliderWidget* s3 = new MIP_SliderWidget( MIP_DRect(185,20,65,15), "Min", 0);
      s3->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s3->setTextSize(-0.7);
      s3->setValueSize(-0.7);
      offset_section->appendChildWidget(s3);
      connectWidget( AParameters[PAR_PROB_OFFSET_MIN_LOOP], s3 );

      MIP_SliderWidget* s4 = new MIP_SliderWidget( MIP_DRect(185,40,65,15), "Max", 0);
      s4->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s4->setTextSize(-0.7);
      s4->setValueSize(-0.7);
      offset_section->appendChildWidget(s4);
      connectWidget( AParameters[PAR_PROB_OFFSET_MAX_LOOP], s4 );

    }

    // loop reverse

    MIP_PanelWidget* reverse_section = new MIP_PanelWidget(MIP_DRect( 280,405 + TH, 260,65) );
    reverse_section->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    background->appendChildWidget(reverse_section);

    {
      MIP_TextWidget* t0 = new MIP_TextWidget( MIP_DRect(0,0,260,15), "Loop Reverse");
      t0->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      t0->setFillBackground(true);
      t0->setBackgroundColor(0.3);
      t0->setDrawBorder(false);
      t0->setTextSize(-0.8);
      t0->setTextColor(0.7);
      reverse_section->appendChildWidget(t0);

      MIP_KnobWidget* k = new MIP_KnobWidget( MIP_DRect(10,20,40,40), "", 0.5);
      k->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      k->setFillBackground(false);
      k->setDrawBorder(false);
      reverse_section->appendChildWidget(k);
      connectWidget( AParameters[PAR_PROB_REVERSE_PROB_RANGE], k );

      //MIP_SliderWidget* s1 = new MIP_SliderWidget( MIP_DRect(60,20,65,15), "Min", 0);
      //s1->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      //s1->setTextSize(-0.7);
      //s1->setValueSize(-0.7);
      //reverse_section->appendChildWidget(s1);
      //connectWidget( AParameters[PAR_PROB_REVERSE_MIN_RANGE], s1 );

      //MIP_SliderWidget* s2 = new MIP_SliderWidget( MIP_DRect(60,40,65,15), "Max", 0);
      //s2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      //s2->setTextSize(-0.7);
      //s2->setValueSize(-0.7);
      //reverse_section->appendChildWidget(s2);
      //connectWidget( AParameters[PAR_PROB_REVERSE_MAX_RANGE], s2 );

      MIP_KnobWidget* k2 = new MIP_KnobWidget( MIP_DRect(135,20,40,40), "", 0.5);
      k2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      k2->setFillBackground(false);
      k2->setDrawBorder(false);
      reverse_section->appendChildWidget(k2);
      connectWidget( AParameters[PAR_PROB_REVERSE_PROB_LOOP], k2 );

      //MIP_SliderWidget* s3 = new MIP_SliderWidget( MIP_DRect(185,20,65,15), "Min", 0);
      //s3->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      //s3->setTextSize(-0.7);
      //s3->setValueSize(-0.7);
      //reverse_section->appendChildWidget(s3);
      //connectWidget( AParameters[PAR_PROB_REVERE_MIN_LOOP], s3 );

      //MIP_SliderWidget* s4 = new MIP_SliderWidget( MIP_DRect(185,40,65,15), "Max", 0);
      //s4->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      //s4->setTextSize(-0.7);
      //s4->setValueSize(-0.7);
      //reverse_section->appendChildWidget(s4);
      //connectWidget( AParameters[PAR_PROB_REVERSE_MAX_LOOP], s4 );

    }

    // loop fx

    MIP_PanelWidget* fx_section = new MIP_PanelWidget(MIP_DRect( 280,480 + TH, 260,65) );
    fx_section->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    background->appendChildWidget(fx_section);

    {
      MIP_TextWidget* t0 = new MIP_TextWidget( MIP_DRect(0,0,260,15), "Loop FX");
      t0->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      t0->setFillBackground(true);
      t0->setBackgroundColor(0.3);
      t0->setDrawBorder(false);
      t0->setTextSize(-0.8);
      t0->setTextColor(0.7);
      fx_section->appendChildWidget(t0);

      MIP_KnobWidget* k = new MIP_KnobWidget( MIP_DRect(10,20,40,40), "", 0.5);
      k->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      k->setFillBackground(false);
      k->setDrawBorder(false);
      fx_section->appendChildWidget(k);
      connectWidget( AParameters[PAR_PROB_FX_PROB_RANGE], k );

      MIP_SliderWidget* s1 = new MIP_SliderWidget( MIP_DRect(60,20,65,15), "Min", 0);
      s1->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s1->setTextSize(-0.7);
      s1->setValueSize(-0.7);
      fx_section->appendChildWidget(s1);
      connectWidget( AParameters[PAR_PROB_FX_MIN_RANGE], s1 );

      MIP_SliderWidget* s2 = new MIP_SliderWidget( MIP_DRect(60,40,65,15), "Max", 0);
      s2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s2->setTextSize(-0.7);
      s2->setValueSize(-0.7);
      fx_section->appendChildWidget(s2);
      connectWidget( AParameters[PAR_PROB_FX_MAX_RANGE], s2 );

      MIP_KnobWidget* k2 = new MIP_KnobWidget( MIP_DRect(135,20,40,40), "", 0.5);
      k2->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      k2->setFillBackground(false);
      k2->setDrawBorder(false);
      fx_section->appendChildWidget(k2);
      connectWidget( AParameters[PAR_PROB_FX_PROB_LOOP], k2 );

      MIP_SliderWidget* s3 = new MIP_SliderWidget( MIP_DRect(185,20,65,15), "Min", 0);
      s3->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s3->setTextSize(-0.7);
      s3->setValueSize(-0.7);
      fx_section->appendChildWidget(s3);
      connectWidget( AParameters[PAR_PROB_FX_MIN_LOOP], s3 );

      MIP_SliderWidget* s4 = new MIP_SliderWidget( MIP_DRect(185,40,65,15), "Max", 0);
      s4->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      s4->setTextSize(-0.7);
      s4->setValueSize(-0.7);
      fx_section->appendChildWidget(s4);
      connectWidget( AParameters[PAR_PROB_FX_MAX_LOOP], s4 );

    }

//#endif // 0

    background->appendChildWidget(MFXFilterMenu);

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
      updateWaveformWidget(process);
      MWaveform->redraw(); // do_widget_redraw(MWaveform);
    }
  }

  //----------

  void updateWaveformWidget(sa_botage_process* process) {
    MWaveform->setBuffer(process->MLeftBuffer,process->MBufferLength);
    MWaveform->setNumGrid(process->par_num_beats);
    MWaveform->setNumSubGrid(process->par_num_slices);

    // marker 0 write pos
    MWaveform->setMarkerActive(0,true);
    MWaveform->setMarkerPos(0,process->MWritePos);
    MWaveform->setMarkerColor(0, MIP_Color(1,0,0,1) );                          // MWritePosColor
    MWaveform->setMarkerWidth(0,1);                                             // MWritePosWidth

    // marker 1 read pos
    MWaveform->setMarkerActive(1,true);
    MWaveform->setMarkerPos(1,process->MReadPos);
    MWaveform->setMarkerColor(1, MIP_Color(0,1,0,1) );                          // MReadPosColor
    MWaveform->setMarkerWidth(1,1);                                             // MReadPosWidth

    if (process->MRange) {
      // area 1 range
      MWaveform->setAreaActive(0,false);
      MWaveform->setAreaActive(1,true);
      MWaveform->setAreaStart(1,process->MRangeStart);
      MWaveform->setAreaLength(1,process->MRangeLength);
      MWaveform->setAreaColor(1, MIP_Color(0.0,0.5,0,0.3) );                      // MRangeColor[n]

      if (process->MLoop) {
        // area 2 loop
        MWaveform->setAreaActive(2,true);
        MWaveform->setAreaStart(2,process->MLoopStart);
        MWaveform->setAreaLength(2,process->MLoopLength);
        MWaveform->setAreaColor(2, MIP_Color(0,1.0,0,0.3) );
      }
      else {
        MWaveform->setAreaActive(2,false);
      }
    }
    else { // no range
      MWaveform->setAreaActive(1,false);
      MWaveform->setAreaActive(2,false);

      // area 0 current slice
      if (process->MIsPlaying) {
        MWaveform->setAreaActive(0,true);
        MWaveform->setAreaStart(0,process->MSliceStart);
        MWaveform->setAreaLength(0,process->MSliceLength);
        MWaveform->setAreaColor(0, MIP_Color(0,0,0,0.1) );                      // SCurrentliceColor
      }
      else {
        MWaveform->setAreaActive(0,false);
      }
    }
  }


};


//----------------------------------------------------------------------
#endif
