#ifndef sa_botage_editor_included
#define sa_botage_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_editor.h"

//#include "../data/img/knob4_60x60_131.h"
//#include "../data/img/sa_logo_40_trans_black.h"
//#include "../data/img/mip2_trans_129x34.h"

#include "sa_botage_widgets.h"


class sa_botage_editor
: public MIP_Editor {

private:

  MIP_Widget*   MEditorWidget = nullptr;

  //MIP_Bitmap*   MKnobBitmap   = nullptr;
  //MIP_Surface*  MKnobSurface  = nullptr;

  MIP_Bitmap*   MLogoBitmap   = nullptr;
  MIP_Surface*  MLogoSurface  = nullptr;

  MIP_Bitmap*   MMip2Bitmap   = nullptr;
  MIP_Surface*  MMip2Surface  = nullptr;

public:

  sa_botage_editor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded)
  : MIP_Editor(AListener,APlugin,AWidth,AHeight,AEmbedded) {

    //setCanResize();
    MIP_Window* window = getWindow();
    window->setFillWindowBackground(false);
    window->setWindowBackgroundColor(0.5f);
    window->setWindowTitle("SA_BOTAGE");

    // bitmaps

    MIP_Painter* painter;

    // knob
    //MKnobBitmap = new MIP_Bitmap(knob4_60x60_131,knob4_60x60_131_size);
    ////knob_bitmap->convertRgbaToBgra();
    //MKnobBitmap->premultAlpha(0x999999);
    //MKnobSurface = new MIP_Surface(window,MKnobBitmap->getWidth(),MKnobBitmap->getHeight());
    //painter = new MIP_Painter(MKnobSurface);
    //painter->drawBitmap(0,0,MKnobBitmap);
    //painter->flush();
    //delete painter;

    // logo
    MLogoBitmap = new MIP_Bitmap(sa_logo_40_trans_black,sa_logo_40_trans_black_size);
    //knob_bitmap->convertRgbaToBgra();
    MLogoBitmap->premultAlpha(0x808080);
    MLogoSurface = new MIP_Surface(window,MLogoBitmap->getWidth(),MLogoBitmap->getHeight());
    painter = new MIP_Painter(MLogoSurface);
    painter->drawBitmap(0,0,MLogoBitmap);
    painter->flush();
    delete painter;

    // mip2
    MMip2Bitmap = new MIP_Bitmap(mip2_trans_129x34,mip2_trans_129x34_size);
    //knob_bitmap->convertRgbaToBgra();
    MMip2Bitmap->scaleLayer(3,128);
    MMip2Bitmap->premultAlpha(0x808080);
    MMip2Surface = new MIP_Surface(window,MMip2Bitmap->getWidth(),MMip2Bitmap->getHeight());
    painter = new MIP_Painter(MMip2Surface);
    painter->drawBitmap(0,0,MMip2Bitmap);
    painter->flush();
    delete painter;

    // filter menu

    MIP_MenuWidget* filter_menu = new MIP_MenuWidget( MIP_FRect() );
    filter_menu->appendMenuItem("Off");
    filter_menu->appendMenuItem("Lowpass");
    filter_menu->appendMenuItem("Highpass");
    filter_menu->appendMenuItem("Bandpass");
    filter_menu->appendMenuItem("Notch");
    filter_menu->setItemSize(150,20);
    filter_menu->setItemLayout(1,5);
    filter_menu->setMenuMirror(false,true);

    //----------

    // editor widget

    MEditorWidget = new MIP_Widget(MIP_FRect());
    MEditorWidget->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

    // sa header


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //MIP_SAHeaderWidget* sa_header = new MIP_SAHeaderWidget(MIP_FRect(60),MLogoSurface,MMip2Surface);
    MIP_SAHeaderWidget* sa_header = new MIP_SAHeaderWidget(MIP_FRect(60),window);


    MEditorWidget->appendWidget(sa_header);
    sa_header->setPluginName("BOTAGE");
    sa_header->setPluginVersion("v0.0.11");
    sa_header->setPluginNameColor(0.2);

    //----------

    // footer

    MIP_PanelWidget* footer = new MIP_PanelWidget(MIP_FRect(20));
    MEditorWidget->appendWidget(footer);
    footer->layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
    footer->setDrawBorder(false);

      MIP_SizerWidget* window_sizer = new MIP_SizerWidget(MIP_FRect( 15,15),MIP_SIZER_WINDOW);
      footer->appendWidget(window_sizer);
      window_sizer->layout.alignment = MIP_WIDGET_ALIGN_BOTTOM_RIGHT;
      //window_sizer->setTarget(window);
      window_sizer->state.active = false;


    //----------

    MIP_PanelWidget* panel = new MIP_PanelWidget(MIP_FRect());
    MEditorWidget->appendWidget(panel);
    panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    panel->setDrawBorder(true);
    panel->setBorderEdges(MIP_EDGE_TOP | MIP_EDGE_BOTTOM);
    //panel->layout.outerBorder = MIP_FRect(10,0,10,0);

//-----

      MIP_WaveformWidget* waveform = new MIP_WaveformWidget(MIP_FRect(10,10,526,70));
      panel->appendWidget(waveform);
      waveform->setBackgroundColor(MIP_COLOR_DARK_GRAY);

      MIP_SliderWidget* num_beats  = new MIP_SliderWidget(MIP_FRect( 10, 95, 260,20),"num_beats");
      MIP_SliderWidget* num_slices = new MIP_SliderWidget(MIP_FRect(276, 95, 260,20),"num_slices");

      //num_beats->setRoundedCorners(MIP_CORNER_ALL);
      //num_beats->setRoundedRadius(5);

      panel->appendWidget(num_beats);
      panel->appendWidget(num_slices);

                                                                        panel->appendWidget( new sa_botage_text_header1( MIP_FRect(  10, 130,  240, 20 ),"REPEAT"               ));

    sa_botage_knob*     wdg_RepeatProb          = (sa_botage_knob*)     panel->appendWidget( new sa_botage_knob(         MIP_FRect(  10, 180,   70, 70 )                        ));

                                                                        panel->appendWidget( new sa_botage_text_small(   MIP_FRect( 100, 160,  150, 15 ),"Slices"               ));
    sa_botage_buttons*  wdg_RepeatSliceBits     = (sa_botage_buttons*)  panel->appendWidget( new sa_botage_buttons(      MIP_FRect( 100, 180,  150, 20 ), 8                     ));
                                                                        panel->appendWidget( new sa_botage_text_small(   MIP_FRect( 100, 205,  150, 15 ),"Split"                ));
    sa_botage_buttons*  wdg_RepeatSplitBits     = (sa_botage_buttons*)  panel->appendWidget( new sa_botage_buttons(      MIP_FRect( 100, 225,  150, 20 ), 8                     ));

                                                                        panel->appendWidget( new sa_botage_text_header1( MIP_FRect( 270, 130,  126, 20 ),"Range"                ));
                                                                        panel->appendWidget( new sa_botage_text_header1( MIP_FRect( 410, 130,  126, 20 ),"Loop"                 ));

                                                                        panel->appendWidget( new sa_botage_text_header2( MIP_FRect( 270, 160,  264, 20 ),"Size"                 ));
    sa_botage_knob*     wdg_LoopsizeRangeProb   = (sa_botage_knob*)     panel->appendWidget( new sa_botage_knob(         MIP_FRect( 270, 190,   40, 40 )                        ));
    sa_botage_slider*   wdg_LoopsizeRangeMin    = (sa_botage_slider*)   panel->appendWidget( new sa_botage_slider(       MIP_FRect( 320, 190,   74, 16 )                        ));
    sa_botage_slider*   wdg_LoopsizeRangeMax    = (sa_botage_slider*)   panel->appendWidget( new sa_botage_slider(       MIP_FRect( 320, 210,   74, 16 )                        ));
    sa_botage_knob*     wdg_LoopsizeLoopProb    = (sa_botage_knob*)     panel->appendWidget( new sa_botage_knob(         MIP_FRect( 410, 190,   40, 40 )                        ));
    sa_botage_slider*   wdg_LoopsizeLoopMin     = (sa_botage_slider*)   panel->appendWidget( new sa_botage_slider(       MIP_FRect( 460, 190,   74, 16 )                        ));
    sa_botage_slider*   wdg_LoopsizeLoopMax     = (sa_botage_slider*)   panel->appendWidget( new sa_botage_slider(       MIP_FRect( 460, 210,   74, 16 )                        ));

                                                                        panel->appendWidget( new sa_botage_text_header2( MIP_FRect( 270, 240,  264, 20 ),"Speed"                ));
    sa_botage_knob*     wdg_LoopspeedRangeProb  = (sa_botage_knob*)     panel->appendWidget( new sa_botage_knob(         MIP_FRect( 270, 270,   40, 40 )                        ));
    sa_botage_slider*   wdg_LoopspeedRangeMin   = (sa_botage_slider*)   panel->appendWidget( new sa_botage_slider(       MIP_FRect( 320, 270,   74, 16 )                        ));
    sa_botage_slider*   wdg_LoopspeedRangeMax   = (sa_botage_slider*)   panel->appendWidget( new sa_botage_slider(       MIP_FRect( 320, 290,   74, 16 )                        ));
    sa_botage_knob*     wdg_LoopspeedLoopProb   = (sa_botage_knob*)     panel->appendWidget( new sa_botage_knob(         MIP_FRect( 410, 270,   40, 40 )                        ));
    sa_botage_slider*   wdg_LoopspeedLoopMin    = (sa_botage_slider*)   panel->appendWidget( new sa_botage_slider(       MIP_FRect( 460, 270,   74, 16 )                        ));
    sa_botage_slider*   wdg_LoopspeedLoopMax    = (sa_botage_slider*)   panel->appendWidget( new sa_botage_slider(       MIP_FRect( 460, 290,   74, 16 )                        ));

                                                                        panel->appendWidget( new sa_botage_text_header2( MIP_FRect( 270, 320,  264, 20 ),"Offset"               ));
    sa_botage_knob*     wdg_OffsetRangeProb     = (sa_botage_knob*)     panel->appendWidget( new sa_botage_knob(         MIP_FRect( 270, 350,   40, 40 )                        ));
    sa_botage_slider2*  wdg_OffsetRangeMin      = (sa_botage_slider2*)  panel->appendWidget( new sa_botage_slider(       MIP_FRect( 320, 350,   74, 16 )                        ));
    sa_botage_slider2*  wdg_OffsetRangeMax      = (sa_botage_slider2*)  panel->appendWidget( new sa_botage_slider(       MIP_FRect( 320, 370,   74, 16 )                        ));
    sa_botage_knob*     wdg_OffsetLoopProb      = (sa_botage_knob*)     panel->appendWidget( new sa_botage_knob(         MIP_FRect( 410, 350,   40, 40 )                        ));
    sa_botage_slider2*  wdg_OffsetLoopMin       = (sa_botage_slider2*)  panel->appendWidget( new sa_botage_slider(       MIP_FRect( 460, 350,   74, 16 )                        ));
    sa_botage_slider2*  wdg_OffsetLoopMax       = (sa_botage_slider2*)  panel->appendWidget( new sa_botage_slider(       MIP_FRect( 460, 370,   74, 16 )                        ));

                                                                        panel->appendWidget( new sa_botage_text_header2( MIP_FRect( 270, 400,  264, 20 ),"Reverse"              ));
    sa_botage_knob*     wdg_ReverseRangeProb    = (sa_botage_knob*)     panel->appendWidget( new sa_botage_knob(         MIP_FRect( 270, 430,   40, 40 )                        ));
    sa_botage_knob*     wdg_ReverseLoopProb     = (sa_botage_knob*)     panel->appendWidget( new sa_botage_knob(         MIP_FRect( 410, 430,   40, 40 )                        ));

                                                                        panel->appendWidget( new sa_botage_text_header2( MIP_FRect( 270, 480,  264, 20 ),"FX"                   ));
    sa_botage_switch*   wdg_FXMulti             = (sa_botage_switch*)   panel->appendWidget( new sa_botage_switch(       MIP_FRect( 490, 480,   44, 17 )                        ));
    sa_botage_knob*     wdg_FXRangeProb         = (sa_botage_knob*)     panel->appendWidget( new sa_botage_knob(         MIP_FRect( 270, 510,   40, 40 )                        ));
    sa_botage_slider*   wdg_FXRangeMin          = (sa_botage_slider*)   panel->appendWidget( new sa_botage_slider(       MIP_FRect( 320, 510,   74, 16 )                        ));
    sa_botage_slider*   wdg_FXRangeMax          = (sa_botage_slider*)   panel->appendWidget( new sa_botage_slider(       MIP_FRect( 320, 530,   74, 16 )                        ));
    sa_botage_knob*     wdg_FXLoopProb          = (sa_botage_knob*)     panel->appendWidget( new sa_botage_knob(         MIP_FRect( 410, 510,   40, 40 )                        ));
    sa_botage_slider*   wdg_FXLoopMin           = (sa_botage_slider*)   panel->appendWidget( new sa_botage_slider(       MIP_FRect( 460, 510,   74, 16 )                        ));
    sa_botage_slider*   wdg_FXLoopMax           = (sa_botage_slider*)   panel->appendWidget( new sa_botage_slider(       MIP_FRect( 460, 530,   74, 16 )                        ));

                                                                      //panel->appendWidget( new sa_botage_text_header1( MIP_FRect(  10, 326,  240, 20 ),"ENVELOPES"            ));

                                                                        panel->appendWidget( new sa_botage_text_header2( MIP_FRect(  10, 266,  240, 20 ),"Loop Env"             ));
    sa_botage_slider*   wdg_EnvLoopAtt        = (sa_botage_slider*)     panel->appendWidget( new sa_botage_slider2(      MIP_FRect(  10, 296,  115, 20 )                        ));
    sa_botage_slider*   wdg_EnvLoopDec        = (sa_botage_slider*)     panel->appendWidget( new sa_botage_slider2(      MIP_FRect( 130, 296,  115, 20 )                        ));

                                                                        panel->appendWidget( new sa_botage_text_header2( MIP_FRect(  10, 326,  240, 20 ),"Slice Env"            ));
    sa_botage_slider*   wdg_EnvSliceAtt       = (sa_botage_slider*)     panel->appendWidget( new sa_botage_slider2(      MIP_FRect(  10, 356,  115, 20 )                        ));
    sa_botage_slider*   wdg_EnvSliceDec       = (sa_botage_slider*)     panel->appendWidget( new sa_botage_slider2(      MIP_FRect( 130, 356,  115, 20 )                        ));

                                                                        panel->appendWidget( new sa_botage_text_header2( MIP_FRect(  10, 450,  240, 20 ),"Filter"               ));

    MIP_SelectorWidget* wdg_FXFilterType        = (MIP_SelectorWidget*) panel->appendWidget( new MIP_SelectorWidget(    MIP_FRect(   10, 480,  240, 16 )                        ));
    sa_botage_knob*     wdg_FXFilterProb        = (sa_botage_knob*)     panel->appendWidget( new sa_botage_knob(         MIP_FRect(  10, 500,   40, 40 )                        ));
    MIP_SliderWidget*   wdg_FXFilterFreq        = (MIP_SliderWidget*)   panel->appendWidget( new MIP_SliderWidget(      MIP_FRect(   60, 500,  190, 16 )                        ));
    MIP_SliderWidget*   wdg_FXFilterBW          = (MIP_SliderWidget*)   panel->appendWidget( new MIP_SliderWidget(      MIP_FRect(   60, 520,  190, 16 )                        ));


//-----

    //----------

    MEditorWidget->appendWidget(filter_menu);
    window->appendWidget(MEditorWidget);

  }


    //----------

//      // left_panel
//
//      MIP_PanelWidget* left_panel = new MIP_PanelWidget(MIP_FRect(250));
//      MEditorWidget->appendWidget(left_panel);
//      left_panel->setBackgroundColor(0.6);
//      left_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
//      left_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
//      left_panel->layout.spacing = 5;
//
//        //
//
//
//
//        //
//
//        MIP_ButtonWidget* button = new MIP_ButtonWidget(MIP_FRect(20));
//        left_panel->appendWidget(button);
//        button->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//        button->setIsToggle(false);
//
//        MIP_ButtonWidget* switch1 = new MIP_ButtonWidget(MIP_FRect(20));
//        left_panel->appendWidget(switch1);
//        switch1->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//        switch1->setIsToggle(true);
//
//        MIP_SliderWidget* slider = new MIP_SliderWidget(MIP_FRect(110,20), "Slider", 0.5 );
//        left_panel->appendWidget(slider);
//        slider->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;
//
//        MIP_RangeSliderWidget* rangeslider = new MIP_RangeSliderWidget(MIP_FRect(20));
//        left_panel->appendWidget(rangeslider);
//        rangeslider->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//        rangeslider->setValue(0.2);
//        rangeslider->setValue2(0.7);
//
//        MIP_ScrollBarWidget* scrollbar = new MIP_ScrollBarWidget(MIP_FRect(20));
//        left_panel->appendWidget(scrollbar);
//        scrollbar->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//
//        MIP_TextEditWidget* textedit = new MIP_TextEditWidget(MIP_FRect(20));
//        left_panel->appendWidget(textedit);
//        textedit->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//
//        MIP_GroupBoxWidget* groupbox = new MIP_GroupBoxWidget(MIP_FRect(100),true);
//        left_panel->appendWidget(groupbox);
//        groupbox->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;
//        groupbox->getTitleBar()->setDrawTriangle(true);
//        groupbox->close();
//
//          MIP_PanelWidget* group1 = new MIP_PanelWidget(MIP_FRect());
//          groupbox->appendWidget(group1);
//          group1->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
//          group1->setFillBackground(true);
//          //group1->setDrawBorder(true);
//          //group1->setRoundedCorners(MIP_CORNER_LEFT_BOTTOM+MIP_CORNER_RIGHT_BOTTOM);
//          //group1->setRoundedRadius(8);
//          group1->setBackgroundColor(MIP_COLOR_LIGHT_CYAN2);
//
//        MIP_SelectorWidget* selector = new MIP_SelectorWidget(MIP_FRect(110,20));
//        left_panel->appendWidget(selector);
//        selector->setMenu(selector_menu);
//        selector->layout.alignment  = MIP_WIDGET_ALIGN_FILL_TOP;
//
//        MIP_GridWidget* grid = new MIP_GridWidget(MIP_FRect(80),5,5);
//        left_panel->appendWidget(grid);
//        grid->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//
//        MIP_ButtonRowWidget* button_row = new MIP_ButtonRowWidget(MIP_FRect(20), 6, buttonrow_text, MIP_BUTTON_ROW_MULTI );
//        left_panel->appendWidget(button_row);
//        button_row->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//        button_row->setButtonState(0,true);
//        button_row->setButtonState(2,true);
//
//        MIP_ButtonRowWidget* button_row2 = new MIP_ButtonRowWidget(MIP_FRect(20), 6, buttonrow_text, MIP_BUTTON_ROW_SINGLE );
//        left_panel->appendWidget(button_row2);
//        button_row2->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//        button_row2->setButtonState(0,true);
//
//        MIP_KeyboardWidget* keyboard = new MIP_KeyboardWidget(MIP_FRect(40));
//        left_panel->appendWidget(keyboard);
//        keyboard->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//
//        MIP_SliderBankWidget* sliderbank = new MIP_SliderBankWidget(MIP_FRect(40),16);
//        left_panel->appendWidget(sliderbank);
//        sliderbank->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//
//        MIP_ValueGraphWidget* valuegraph = new MIP_ValueGraphWidget(MIP_FRect(40),16);
//        left_panel->appendWidget(valuegraph);
//        valuegraph->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//
//        /*
//        MIP_CircularWaveformWidget* circular = new MIP_CircularWaveformWidget(MIP_FRect(150));
//        left_panel->appendWidget(circular);
//        circular->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//        circular->setInnerRadius(0.5);
//        circular->setOuterRadius(1);
//        circular->setBipolar(true);
//        circular->createBuffer(1024);
//        for (uint32_t i=0; i<1024; i++) circular->setBuffer(i,MIP_RandomRange(-1,1));
//        */
//
//      //----------
//
//      // left_sizer
//
//      MIP_SizerWidget* left_sizer = new MIP_SizerWidget(MIP_FRect(5),MIP_SIZER_LEFT);
//      MEditorWidget->appendWidget(left_sizer);
//      left_sizer->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
//      left_sizer->setTarget(left_panel);
//
//      //----------
//
//      // right top panel
//
//      MIP_PanelWidget* right_top_panel = new MIP_PanelWidget(MIP_FRect(100));
//      MEditorWidget->appendWidget(right_top_panel);
//      right_top_panel->setBackgroundColor(0.6);
//      right_top_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//      right_top_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
//      right_top_panel->layout.spacing = 5;
//
//        MIP_KnobWidget* knob1 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
//        right_top_panel->appendWidget(knob1);
//        knob1->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
//
//        MIP_KnobWidget* knob2 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
//        right_top_panel->appendWidget(knob2);
//        knob2->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
//
//        MIP_KnobWidget* knob3 = new MIP_KnobWidget( MIP_FRect( 50,50 ));
//        right_top_panel->appendWidget(knob3);
//        knob3->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
//
//      MEditor->connect(knob1,0);
//      MEditor->connect(knob2,1);
//      MEditor->connect(knob3,2);
//
//      MIP_CurveWidget* curve1 = new MIP_CurveWidget( MIP_FRect( 50,50 ),false);
//      right_top_panel->appendWidget(curve1);
//      curve1->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
//      curve1->setValue(0.5);
//
//      MIP_CurveWidget* curve2 = new MIP_CurveWidget( MIP_FRect( 50,50 ),true);
//      right_top_panel->appendWidget(curve2);
//      curve2->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
//      curve2->setValue(0.5);
//
//      MIP_ImageStripWidget* imagestrip = new MIP_ImageStripWidget(MIP_FRect(60));
//      right_top_panel->appendWidget(imagestrip);
//      imagestrip->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
//      imagestrip->setup(1,131,MKnobSurface);
//
//      //----------
//
//      // top_sizer
//
//      MIP_SizerWidget* top_sizer = new MIP_SizerWidget(MIP_FRect(5),MIP_SIZER_TOP);
//      MEditorWidget->appendWidget(top_sizer);
//      top_sizer->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
//      top_sizer->setTarget(right_top_panel);
//
//      //----------
//
//      // right bottom panel
//
//      MIP_PanelWidget* right_bottom_panel = new MIP_PanelWidget(MIP_FRect(100));
//      MEditorWidget->appendWidget(right_bottom_panel);
//      right_bottom_panel->setBackgroundColor(0.6);
//      right_bottom_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
//      right_bottom_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
//      right_bottom_panel->layout.spacing = 5;
//
//          MIP_PanelWidget* p;
//
//          p = new MIP_PanelWidget(MIP_FRect(100,25));
//          right_bottom_panel->appendWidget(p);
//          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
//          p->setDrawBorder();
//          p->setFillBackground(false);
//
//          p = new MIP_PanelWidget(MIP_FRect(100,25));
//          right_bottom_panel->appendWidget(p);
//          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
//          p->setDrawBorder();
//          p->setBorderEdges(MIP_EDGE_LEFT + MIP_EDGE_RIGHT);
//          p->setRoundedCorners(MIP_CORNER_LEFT_TOP + MIP_CORNER_RIGHT_TOP + MIP_CORNER_RIGHT_BOTTOM);
//          p->setFillBackground(false);
//
//          p = new MIP_PanelWidget(MIP_FRect(100,25));
//          right_bottom_panel->appendWidget(p);
//          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
//          p->setFillBackground();
//
//          p = new MIP_PanelWidget(MIP_FRect(100,25));
//          right_bottom_panel->appendWidget(p);
//          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
//          p->setFillGradient();
//
//          p = new MIP_PanelWidget(MIP_FRect(100,25));
//          right_bottom_panel->appendWidget(p);
//          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
//          p->setFillGradient();
//          p->setDrawBorder();
//
//          p = new MIP_PanelWidget(MIP_FRect(100,25));
//          right_bottom_panel->appendWidget(p);
//          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
//          p->setFillGradient();
//          p->setDrawBorder();
//          p->setRoundedCorners(MIP_CORNER_ALL);
//          p->setRoundedRadius(6);
//
//          p = new MIP_PanelWidget(MIP_FRect(100,25));
//          right_bottom_panel->appendWidget(p);
//          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
//          p->setFillGradient();
//          p->setDrawBorder();
//          p->setRoundedCorners(MIP_CORNER_LEFT_TOP + MIP_CORNER_RIGHT_TOP);
//          p->setRoundedRadius(6);
//
//          p = new MIP_PanelWidget(MIP_FRect(100,25));
//          right_bottom_panel->appendWidget(p);
//          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
//          //p->setFillBackground();
//          p->setFillGradient();
//          p->setDrawBorder();
//          p->setBorderThickness(2);
//          p->setBorderEdges(MIP_EDGE_LEFT + MIP_EDGE_TOP + MIP_EDGE_RIGHT);
//          p->setRoundedCorners(MIP_CORNER_LEFT_TOP);
//          p->setRoundedRadius(12);
//
//          p = new MIP_PanelWidget(MIP_FRect(100,25));
//          right_bottom_panel->appendWidget(p);
//          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
//          p->setDrawBorder();
//          p->setRoundedCorners(MIP_CORNER_ALL);
//          p->setRoundedRadius(9);
//
//          p = new MIP_PanelWidget(MIP_FRect(100,25));
//          right_bottom_panel->appendWidget(p);
//          p->layout.alignment = MIP_WIDGET_ALIGN_STACK_VERT;
//          p->setFillGradient();
//          p->setGradientColor1(MIP_COLOR_LIGHT_GREEN);
//          p->setGradientColor2(MIP_COLOR_DARK_GREEN);
//          p->setDrawBorder();
//          p->setRoundedCorners(MIP_CORNER_ALL);
//          p->setRoundedRadius(11);
//
//      //----------
//
//      // bottom_sizer
//
//      MIP_SizerWidget* bottom_sizer = new MIP_SizerWidget(MIP_FRect(5),MIP_SIZER_BOTTOM);
//      MEditorWidget->appendWidget(bottom_sizer);
//      bottom_sizer->layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;
//      bottom_sizer->setTarget(right_bottom_panel);
//
//      //----------
//
//      // right center panel
//
//      MIP_PanelWidget* right_center_panel = new MIP_PanelWidget(MIP_FRect());
//      MEditorWidget->appendWidget(right_center_panel);
//      right_center_panel->setBackgroundColor(0.6);
//      right_center_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
//      right_center_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
//      right_center_panel->layout.spacing = 5;
//
//        MIP_TabsWidget* tabs = new MIP_TabsWidget(MIP_FRect());
//        right_center_panel->appendWidget(tabs);
//        tabs->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
//
//          MIP_PanelWidget* page1 = new MIP_PanelWidget(MIP_FRect());
//          page1->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
//          page1->setFillBackground(true);
//          page1->setBackgroundColor(MIP_Color(0.65,0.6,0.6));
//          page1->layout.innerBorder = MIP_FRect(10,10,10,10);
//          page1->layout.spacing = 5;
//
//          MIP_PanelWidget* page2 = new MIP_PanelWidget(MIP_FRect());
//          page2->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
//          page2->setFillBackground(false);
//          page2->setBackgroundColor(MIP_Color(0.6,0.65,0.6));
//
//            MIP_GraphWidget* graph = new MIP_GraphWidget(MIP_FRect());
//            page2->appendWidget(graph);
//            graph->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
//            add_modules(graph);
//
//          MIP_PanelWidget* page3 = new MIP_PanelWidget(MIP_FRect());
//          page3->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
//          page3->setFillBackground(true);
//          page3->setBackgroundColor(MIP_Color(0.6,0.6,0.65));
//
//            MIP_ScrollBoxWidget* scrollbox1 = new MIP_ScrollBoxWidget(MIP_FRect(),true,false);
//            page3->appendWidget(scrollbox1);
//            scrollbox1->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
//
//            scrollbox1->getContentWidget()->layout.innerBorder = MIP_FRect(5,5,5,5);
//            scrollbox1->getContentWidget()->layout.spacing = 5;
//
//            for (uint32_t i=0; i<100; i++) {
//              MIP_KnobWidget* knob = new MIP_KnobWidget( MIP_FRect( 32,32 ));
//              scrollbox1->appendWidget(knob);
//              knob->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
//            }
//
//        tabs->appendPage("Page1",page1);
//        tabs->appendPage("Page2",page2);
//        tabs->appendPage("Page3",page3);
//        tabs->selectPage(0);

  //----------

  virtual ~sa_botage_editor() {
    delete MLogoBitmap;
    delete MLogoSurface;
    delete MMip2Bitmap;
    delete MMip2Surface;
  }

};

//----------------------------------------------------------------------
#endif


























#if 0


    //-----


    //-----

//    wdg_Header              = (sa_botage_header*)   appendWidget( new sa_botage_header(       MIP_FRect(   0,   0,  400, 60 ), this, MIP_Color(0.5) ));
//                                                    appendWidget( new sa_botage_text_small2(  MIP_FRect( 436,  10,  100, 15 ), descriptor->getVersionText()               ));

    wdg_Waveform            = (sa_botage_waveform*) appendWidget( new sa_botage_waveform(     MIP_FRect(  10,  70,  526, 70 )                        ));

    wdg_BufferNumBeats      = (sa_botage_qslider*)  appendWidget( new sa_botage_qslider(      MIP_FRect(  10, 155,  260, 20 ), 8                     ));
    wdg_BufferNumSlices     = (sa_botage_qslider*)  appendWidget( new sa_botage_qslider(      MIP_FRect( 276, 155,  260, 20 ), 8                     ));

                                                    appendWidget( new sa_botage_text_header1( MIP_FRect(  10, 200,  240, 20 ),"REPEAT"               ));

    wdg_RepeatProb          = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         MIP_FRect(  10, 250,   70, 70 )                        ));

                                                    appendWidget( new sa_botage_text_small(   MIP_FRect( 100, 230,  150, 15 ),"Slices"               ));
    wdg_RepeatSliceBits     = (sa_botage_buttons*)  appendWidget( new sa_botage_buttons(      MIP_FRect( 100, 250,  150, 20 ), 8                     ));
                                                    appendWidget( new sa_botage_text_small(   MIP_FRect( 100, 275,  150, 15 ),"Split"                ));
    wdg_RepeatSplitBits     = (sa_botage_buttons*)  appendWidget( new sa_botage_buttons(      MIP_FRect( 100, 295,  150, 20 ), 8                     ));

                                                    appendWidget( new sa_botage_text_header1( MIP_FRect( 270, 200,  126, 20 ),"Range"                ));
                                                    appendWidget( new sa_botage_text_header1( MIP_FRect( 410, 200,  126, 20 ),"Loop"                 ));

                                                    appendWidget( new sa_botage_text_header2( MIP_FRect( 270, 230,  264, 20 ),"Size"                 ));
    wdg_LoopsizeRangeProb   = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         MIP_FRect( 270, 260,   40, 40 )                        ));
    wdg_LoopsizeRangeMin    = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       MIP_FRect( 320, 260,   74, 16 )                        ));
    wdg_LoopsizeRangeMax    = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       MIP_FRect( 320, 280,   74, 16 )                        ));
    wdg_LoopsizeLoopProb    = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         MIP_FRect( 410, 260,   40, 40 )                        ));
    wdg_LoopsizeLoopMin     = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       MIP_FRect( 460, 260,   74, 16 )                        ));
    wdg_LoopsizeLoopMax     = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       MIP_FRect( 460, 280,   74, 16 )                        ));

                                                    appendWidget( new sa_botage_text_header2( MIP_FRect( 270, 310,  264, 20 ),"Speed"                ));
    wdg_LoopspeedRangeProb  = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         MIP_FRect( 270, 340,   40, 40 )                        ));
    wdg_LoopspeedRangeMin   = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       MIP_FRect( 320, 340,   74, 16 )                        ));
    wdg_LoopspeedRangeMax   = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       MIP_FRect( 320, 360,   74, 16 )                        ));
    wdg_LoopspeedLoopProb   = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         MIP_FRect( 410, 340,   40, 40 )                        ));
    wdg_LoopspeedLoopMin    = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       MIP_FRect( 460, 340,   74, 16 )                        ));
    wdg_LoopspeedLoopMax    = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       MIP_FRect( 460, 360,   74, 16 )                        ));

                                                    appendWidget( new sa_botage_text_header2( MIP_FRect( 270, 390,  264, 20 ),"Offset"               ));
    wdg_OffsetRangeProb     = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         MIP_FRect( 270, 420,   40, 40 )                        ));
    wdg_OffsetRangeMin      = (sa_botage_slider2*)  appendWidget( new sa_botage_slider(       MIP_FRect( 320, 420,   74, 16 )                        ));
    wdg_OffsetRangeMax      = (sa_botage_slider2*)  appendWidget( new sa_botage_slider(       MIP_FRect( 320, 440,   74, 16 )                        ));
    wdg_OffsetLoopProb      = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         MIP_FRect( 410, 420,   40, 40 )                        ));
    wdg_OffsetLoopMin       = (sa_botage_slider2*)  appendWidget( new sa_botage_slider(       MIP_FRect( 460, 420,   74, 16 )                        ));
    wdg_OffsetLoopMax       = (sa_botage_slider2*)  appendWidget( new sa_botage_slider(       MIP_FRect( 460, 440,   74, 16 )                        ));

                                                    appendWidget( new sa_botage_text_header2( MIP_FRect( 270, 470,  264, 20 ),"Reverse"              ));
    wdg_ReverseRangeProb    = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         MIP_FRect( 270, 500,   40, 40 )                        ));
    wdg_ReverseLoopProb     = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         MIP_FRect( 410, 500,   40, 40 )                        ));

                                                    appendWidget( new sa_botage_text_header2( MIP_FRect( 270, 550,  264, 20 ),"FX"                   ));
    wdg_FXMulti             = (sa_botage_switch*)   appendWidget( new sa_botage_switch(       MIP_FRect( 490, 550,   44, 17 )                        ));
    wdg_FXRangeProb         = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         MIP_FRect( 270, 580,   40, 40 )                        ));
    wdg_FXRangeMin          = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       MIP_FRect( 320, 580,   74, 16 )                        ));
    wdg_FXRangeMax          = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       MIP_FRect( 320, 600,   74, 16 )                        ));
    wdg_FXLoopProb          = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         MIP_FRect( 410, 580,   40, 40 )                        ));
    wdg_FXLoopMin           = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       MIP_FRect( 460, 580,   74, 16 )                        ));
    wdg_FXLoopMax           = (sa_botage_slider*)   appendWidget( new sa_botage_slider(       MIP_FRect( 460, 600,   74, 16 )                        ));

                                                  //appendWidget( new sa_botage_text_header1( MIP_FRect(  10, 396,  240, 20 ),"ENVELOPES"            ));

                                                    appendWidget( new sa_botage_text_header2( MIP_FRect(  10, 336,  240, 20 ),"Loop Env"             ));
    wdg_EnvLoopAtt          = (sa_botage_slider*)   appendWidget( new sa_botage_slider2(      MIP_FRect(  10, 366,  115, 20 )                        ));
    wdg_EnvLoopDec          = (sa_botage_slider*)   appendWidget( new sa_botage_slider2(      MIP_FRect( 130, 366,  115, 20 )                        ));

                                                    appendWidget( new sa_botage_text_header2( MIP_FRect(  10, 396,  240, 20 ),"Slice Env"            ));
    wdg_EnvSliceAtt         = (sa_botage_slider*)   appendWidget( new sa_botage_slider2(      MIP_FRect(  10, 426,  115, 20 )                        ));
    wdg_EnvSliceDec         = (sa_botage_slider*)   appendWidget( new sa_botage_slider2(      MIP_FRect( 130, 426,  115, 20 )                        ));

                                                    appendWidget( new sa_botage_text_header2( MIP_FRect( 10, 520,  240, 20 ),"Filter"               ));

    wdg_FXFilterType        = (MIP_SelectorWidget*)appendWidget( new MIP_SelectorWidget(    MIP_FRect( 10, 550,  240, 16 )                        ));
    wdg_FXFilterProb        = (sa_botage_knob*)     appendWidget( new sa_botage_knob(         MIP_FRect( 10, 570,   40, 40 )                        ));
    wdg_FXFilterFreq        = (MIP_SliderWidget*)  appendWidget( new MIP_SliderWidget(      MIP_FRect( 60, 570,  190, 16 )                        ));
    wdg_FXFilterBW          = (MIP_SliderWidget*)  appendWidget( new MIP_SliderWidget(      MIP_FRect( 60, 590,  190, 16 )                        ));

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

#endif // 0
