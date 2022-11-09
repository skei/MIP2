#ifndef sa_tyr_editor_included
#define sa_tyr_editor_included
//----------------------------------------------------------------------

// we need some visual tool to do all of this..

#include "mip.h"
#include "gui/widgets/mip_widgets.h"
#include "plugin/mip_editor.h"

#include "sa_tyr_widgets.h"

//#define _PLUG_NAME  "tyr"
//#define _PLUG_VER   "0.0.9"

//----------------------------------------------------------------------

class sa_tyr_editor
: public MIP_Editor
/*, public MIP_MenuListener*/ {

//------------------------------
public:
//------------------------------

  sa_tyr_VoiceWidget* MVoiceWidget          = nullptr;
  MIP_ValueWidget*    MPlayingVoicesWidget  = nullptr;
  MIP_ValueWidget*    MReleasedVoicesWidget = nullptr;
  MIP_ValueWidget*    MTotalVoicesWidget    = nullptr;
  //MIP_KeyboardWidget* MKeyboards[11]        = {0};

//------------------------------
public:
//------------------------------

  sa_tyr_editor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, MIP_ParameterArray AParameters)
  : MIP_Editor(AListener,AWidth,AHeight) {


    MIP_Window* window = getWindow();

    //------------------------------
    // menus
    //------------------------------

    sa_tyr_MenuWidget* osc_type_menu           = new sa_tyr_MenuWidget(160,20,sa_tyr_osc_type_text,SA_TYR_OSC_TYPE_COUNT);
    sa_tyr_MenuWidget* res_type_menu           = new sa_tyr_MenuWidget(160,20,sa_tyr_res_type_text,SA_TYR_RES_TYPE_COUNT);
    sa_tyr_MenuWidget* flt_type_menu           = new sa_tyr_MenuWidget(160,20,sa_tyr_flt_type_text,SA_TYR_FLT_TYPE_COUNT);
    sa_tyr_MenuWidget* phase_shaper_type_menu  = new sa_tyr_MenuWidget(160,20,sa_tyr_pm_type_text,SA_TYR_PM_TYPE_COUNT);
    sa_tyr_MenuWidget* wave_shaper_type_menu   = new sa_tyr_MenuWidget(160,20,sa_tyr_wm_type_text,SA_TYR_WM_TYPE_COUNT);

    //------------------------------
    //
    //------------------------------

    #define HEADER_HEIGHT 80

    // panel

    MIP_PanelWidget* MEditorPanel = new MIP_PanelWidget(MIP_DRect(0,0,SA_TYR_EDITOR_WIDTH,SA_TYR_EDITOR_HEIGHT));
    MEditorPanel->setDrawBorder(false);
    MEditorPanel->setFillBackground(false);

    // header

    MIP_SAHeaderWidget* header = new MIP_SAHeaderWidget(MIP_DRect(0,0,SA_TYR_EDITOR_WIDTH,HEADER_HEIGHT));
    MEditorPanel->appendChildWidget(header);
    header->setPluginName(SA_TYR_PLUGIN_NAME);
    header->setPluginVersion(SA_TYR_PLUGIN_VERSION);

    // controls

    MIP_PanelWidget* controls = new MIP_PanelWidget(MIP_DRect(0,HEADER_HEIGHT,SA_TYR_EDITOR_WIDTH,SA_TYR_EDITOR_HEIGHT - HEADER_HEIGHT));
    MEditorPanel->appendChildWidget(controls);
    controls->setDrawBorder(false);
    controls->setFillBackground(true);
    controls->setBackgroundColor(0.6);

    #undef HEADER_HEAIGHT

//
//
// left --------------------------------------------------
//
//

// ----- osc1

      sa_tyr_SectionPanelWidget* osc1_panel = new sa_tyr_SectionPanelWidget(MIP_DRect(10,10,260,300), "OSC1", 20 );
      controls->appendChildWidget(osc1_panel);

        // type

        sa_tyr_SelectorWidget* osc1_type_selector = new sa_tyr_SelectorWidget( MIP_DRect(10,30,240,20), "Osc Type", osc_type_menu );
        osc1_panel->appendChildWidget(osc1_type_selector);
        connectWidget(AParameters[PAR_OSC1_TYPE], osc1_type_selector );

        // shape

        sa_tyr_BigKnobWidget* osc1_shape_knob = new sa_tyr_BigKnobWidget( MIP_DRect(10,60,50,82),"Shape");
        //osc1_shape_knob->getLabelWidget()->setTextSize(-0.5);
        osc1_panel->appendChildWidget(osc1_shape_knob);
        connectWidget(AParameters[PAR_OSC1_SHAPE],osc1_shape_knob);

        // width

        sa_tyr_BigKnobWidget* osc1_width_knob = new sa_tyr_BigKnobWidget( MIP_DRect(70,60,50,82),"Width", 0.0, true);
        osc1_panel->appendChildWidget(osc1_width_knob);
        connectWidget(AParameters[PAR_OSC1_WIDTH],osc1_width_knob);

        // oct

        sa_tyr_DragValueWidget* osc1_oct_slider = new sa_tyr_DragValueWidget( MIP_DRect(10,150,72,20),"Oct");
        osc1_panel->appendChildWidget(osc1_oct_slider);
        connectWidget(AParameters[PAR_OSC1_OCT],osc1_oct_slider);

        // semi

        sa_tyr_DragValueWidget* osc1_semi_slider = new sa_tyr_DragValueWidget( MIP_DRect(94,150,72,20),"Semi");
        osc1_panel->appendChildWidget(osc1_semi_slider);
        connectWidget(AParameters[PAR_OSC1_SEMI],osc1_semi_slider);

        // cent

        sa_tyr_DragValueWidget* osc1_cent_slider = new sa_tyr_DragValueWidget( MIP_DRect(178,150,72,20), "Cent", 0.0, true, 0.0, 0.1 );
        osc1_panel->appendChildWidget(osc1_cent_slider);
        connectWidget(AParameters[PAR_OSC1_CENT],osc1_cent_slider);

        // inputs

        sa_tyr_SmallKnobWidget* osc1_in_o1 = new sa_tyr_SmallKnobWidget( MIP_DRect(10,180,30,45),"O1");
        osc1_panel->appendChildWidget(osc1_in_o1);
        connectWidget(AParameters[PAR_OSC1_IN_O1],osc1_in_o1);

        sa_tyr_SmallKnobWidget* osc1_in_o2 = new sa_tyr_SmallKnobWidget( MIP_DRect(40,180,30,45),"O2");
        osc1_panel->appendChildWidget(osc1_in_o2);
        connectWidget(AParameters[PAR_OSC1_IN_O2],osc1_in_o2);

        sa_tyr_SmallKnobWidget* osc1_in_r1 = new sa_tyr_SmallKnobWidget( MIP_DRect(70,180,30,45),"R1");
        osc1_panel->appendChildWidget(osc1_in_r1);
        connectWidget(AParameters[PAR_OSC1_IN_R1],osc1_in_r1);

        sa_tyr_SmallKnobWidget* osc1_in_r2 = new sa_tyr_SmallKnobWidget( MIP_DRect(100,180,30,45),"R2");
        osc1_panel->appendChildWidget(osc1_in_r2);
        connectWidget(AParameters[PAR_OSC1_IN_R2],osc1_in_r2);

        sa_tyr_SmallKnobWidget* osc1_in_n = new sa_tyr_SmallKnobWidget( MIP_DRect(130,180,30,45),"N");
        osc1_panel->appendChildWidget(osc1_in_n);
        connectWidget(AParameters[PAR_OSC1_IN_N],osc1_in_n);

        sa_tyr_SmallKnobWidget* osc1_in_s = new sa_tyr_SmallKnobWidget( MIP_DRect(160,180,30,45),"S");
        osc1_panel->appendChildWidget(osc1_in_s);
        connectWidget(AParameters[PAR_OSC1_IN_S],osc1_in_s);

        sa_tyr_SmallKnobWidget* osc1_in_i = new sa_tyr_SmallKnobWidget( MIP_DRect(190,180,30,45),"I");
        osc1_panel->appendChildWidget(osc1_in_i);
        connectWidget(AParameters[PAR_OSC1_IN_I],osc1_in_i);

        sa_tyr_SmallKnobWidget* osc1_in_a = new sa_tyr_SmallKnobWidget( MIP_DRect(220,180,30,45),"A");
        osc1_panel->appendChildWidget(osc1_in_a);
        connectWidget(AParameters[PAR_OSC1_IN_A],osc1_in_a);

        // ps

        sa_tyr_SliderWidget* osc1_phase_shaper_slider = new sa_tyr_SliderWidget(MIP_DRect(10,240,115,20),"Phase",0.0,true);
        osc1_panel->appendChildWidget(osc1_phase_shaper_slider);
        connectWidget(AParameters[PAR_OSC1_PM_AMOUNT],osc1_phase_shaper_slider);

        sa_tyr_SelectorWidget* osc1_phase_shaper_selector = new sa_tyr_SelectorWidget( MIP_DRect(135,240,115,20),"Phase Shp",phase_shaper_type_menu );
        osc1_panel->appendChildWidget(osc1_phase_shaper_selector);
        connectWidget(AParameters[PAR_OSC1_PM_TYPE],osc1_phase_shaper_selector);

        // ws

        sa_tyr_SliderWidget* osc1_wave_shaper_slider = new sa_tyr_SliderWidget(MIP_DRect(10,270,115,20),"Wave",0.0,true);
        osc1_panel->appendChildWidget(osc1_wave_shaper_slider);
        connectWidget(AParameters[PAR_OSC1_WM_AMOUNT],osc1_wave_shaper_slider);

        sa_tyr_SelectorWidget* osc1_wave_shaper_selector = new sa_tyr_SelectorWidget( MIP_DRect(135,270,115,20),"Wave Shp",wave_shaper_type_menu );
        osc1_panel->appendChildWidget(osc1_wave_shaper_selector);
        connectWidget(AParameters[PAR_OSC1_WM_TYPE],osc1_wave_shaper_selector);

// ----- res1 -----

      sa_tyr_SectionPanelWidget* res1_panel = new sa_tyr_SectionPanelWidget(MIP_DRect(10,320,260,260), "RES1", 20 );
      controls->appendChildWidget(res1_panel);

        // type

        sa_tyr_SelectorWidget* res1_type_selector = new sa_tyr_SelectorWidget( MIP_DRect(10,30,240,20),"Res Type",res_type_menu );
        res1_panel->appendChildWidget(res1_type_selector);
        connectWidget(AParameters[PAR_RES1_TYPE],res1_type_selector);

        // inputs

        sa_tyr_SmallKnobWidget* res1_in_o1 = new sa_tyr_SmallKnobWidget( MIP_DRect(10,60,30,45),"O1");
        res1_panel->appendChildWidget(res1_in_o1);
        connectWidget(AParameters[PAR_RES1_IN_O1],res1_in_o1);

        sa_tyr_SmallKnobWidget* res1_in_o2 = new sa_tyr_SmallKnobWidget( MIP_DRect(40,60,30,45),"O2");
        res1_panel->appendChildWidget(res1_in_o2);
        connectWidget(AParameters[PAR_RES1_IN_O2],res1_in_o2);

        sa_tyr_SmallKnobWidget* res1_in_r1 = new sa_tyr_SmallKnobWidget( MIP_DRect(70,60,30,45),"R1");
        res1_panel->appendChildWidget(res1_in_r1);
        connectWidget(AParameters[PAR_RES1_IN_R1],res1_in_r1);

        sa_tyr_SmallKnobWidget* res1_in_r2 = new sa_tyr_SmallKnobWidget( MIP_DRect(100,60,30,45),"R2");
        res1_panel->appendChildWidget(res1_in_r2);
        connectWidget(AParameters[PAR_RES1_IN_R2],res1_in_r2);

        sa_tyr_SmallKnobWidget* res1_in_n = new sa_tyr_SmallKnobWidget( MIP_DRect(130,60,30,45),"N");
        res1_panel->appendChildWidget(res1_in_n);
        connectWidget(AParameters[PAR_RES1_IN_N],res1_in_n);

        sa_tyr_SmallKnobWidget* res1_in_s = new sa_tyr_SmallKnobWidget( MIP_DRect(160,60,30,45),"S");
        res1_panel->appendChildWidget(res1_in_s);
        connectWidget(AParameters[PAR_RES1_IN_S],res1_in_s);

        sa_tyr_SmallKnobWidget* res1_in_i = new sa_tyr_SmallKnobWidget( MIP_DRect(190,60,30,45),"I");
        res1_panel->appendChildWidget(res1_in_i);
        connectWidget(AParameters[PAR_RES1_IN_I],res1_in_i);

        sa_tyr_SmallKnobWidget* res1_in_a = new sa_tyr_SmallKnobWidget( MIP_DRect(220,60,30,45),"A");
        res1_panel->appendChildWidget(res1_in_a);
        connectWidget(AParameters[PAR_RES1_IN_A],res1_in_a);

        // shape

        sa_tyr_BigKnobWidget* res1_nshape_knob = new sa_tyr_BigKnobWidget( MIP_DRect(10,110,50,82),"Shape");
        res1_panel->appendChildWidget(res1_nshape_knob);
        connectWidget(AParameters[PAR_RES1_SHAPE],res1_nshape_knob);

        // fb

        sa_tyr_BigKnobWidget* res1_fb_knob = new sa_tyr_BigKnobWidget( MIP_DRect(70,110,50,82),"FB");
        res1_panel->appendChildWidget(res1_fb_knob);
        connectWidget(AParameters[PAR_RES1_FB],res1_fb_knob);

        // damp

        sa_tyr_BigKnobWidget* res1_damp_knob = new sa_tyr_BigKnobWidget( MIP_DRect(130,110,50,82),"Damp");
        res1_panel->appendChildWidget(res1_damp_knob);
        connectWidget(AParameters[PAR_RES1_DAMP],res1_damp_knob);

        // rough

        sa_tyr_BigKnobWidget* res1_rough_knob = new sa_tyr_BigKnobWidget( MIP_DRect(190,110,50,82),"Rough",0.0,true);
        res1_rough_knob->getKnobWidget()->setBipolar(false);
        res1_panel->appendChildWidget(res1_rough_knob);
        connectWidget(AParameters[PAR_RES1_ROUGH],res1_rough_knob);

        // speed

        MIP_SliderWidget* res1_speed_slider = new MIP_SliderWidget( MIP_DRect(10,200,240,20),"speed" );
        res1_panel->appendChildWidget(res1_speed_slider);
        connectWidget(AParameters[PAR_RES1_SPEED],res1_speed_slider);

        // oct

        sa_tyr_DragValueWidget* res1_oct_slider = new sa_tyr_DragValueWidget( MIP_DRect(10,230,72,20),"Oct");
        res1_panel->appendChildWidget(res1_oct_slider);
        connectWidget(AParameters[PAR_RES1_OCT],res1_oct_slider);

        // semi

        sa_tyr_DragValueWidget* res1_semi_slider = new sa_tyr_DragValueWidget( MIP_DRect(94,230,72,20),"Semi");
        res1_panel->appendChildWidget(res1_semi_slider);
        connectWidget(AParameters[PAR_RES1_SEMI],res1_semi_slider);

        // cent

        sa_tyr_DragValueWidget* res1_cent_slider = new sa_tyr_DragValueWidget( MIP_DRect(178,230,72,20),"Cent",0.0,true,0.0,0.1);
        res1_panel->appendChildWidget(res1_cent_slider);
        connectWidget(AParameters[PAR_RES1_CENT],res1_cent_slider);

//
//
// mid --------------------------------------------------
//
//

// ----- osc2

      sa_tyr_SectionPanelWidget* osc2_panel = new sa_tyr_SectionPanelWidget(MIP_DRect(280,10,260,300), "OSC2", 20 );
      controls->appendChildWidget(osc2_panel);

        // type

        sa_tyr_SelectorWidget* osc2_type_selector = new sa_tyr_SelectorWidget( MIP_DRect(10,30,240,20),"Osc Type",osc_type_menu );
        osc2_panel->appendChildWidget(osc2_type_selector);
        connectWidget(AParameters[PAR_OSC2_TYPE],osc2_type_selector);

        // shape

        sa_tyr_BigKnobWidget* osc2_shape_knob = new sa_tyr_BigKnobWidget( MIP_DRect(10,60,50,82),"Shape");
        osc2_panel->appendChildWidget(osc2_shape_knob);
        connectWidget(AParameters[PAR_OSC2_SHAPE],osc2_shape_knob);

        // width

        sa_tyr_BigKnobWidget* osc2_width_knob = new sa_tyr_BigKnobWidget( MIP_DRect(70,60,50,82),"Width",0.0,true);
        osc2_panel->appendChildWidget(osc2_width_knob);
        connectWidget(AParameters[PAR_OSC2_WIDTH],osc2_width_knob);

        // oct

        sa_tyr_DragValueWidget* osc2_oct_slider = new sa_tyr_DragValueWidget( MIP_DRect(10,150,72,20),"Oct");
        osc2_panel->appendChildWidget(osc2_oct_slider);
        connectWidget(AParameters[PAR_OSC2_OCT],osc2_oct_slider);

        // semi

        sa_tyr_DragValueWidget* osc2_semi_slider = new sa_tyr_DragValueWidget( MIP_DRect(94,150,72,20),"Semi");
        osc2_panel->appendChildWidget(osc2_semi_slider);
        connectWidget(AParameters[PAR_OSC2_SEMI],osc2_semi_slider);

        // cent

        sa_tyr_DragValueWidget* osc2_cent_slider = new sa_tyr_DragValueWidget( MIP_DRect(178,150,72,20),"Cent",0.0,true,0.0,0.1);
        osc2_panel->appendChildWidget(osc2_cent_slider);
        connectWidget(AParameters[PAR_OSC2_CENT],osc2_cent_slider);

        // inputs

        sa_tyr_SmallKnobWidget* osc2_in_o1 = new sa_tyr_SmallKnobWidget( MIP_DRect(10,180,30,45),"O1");
        osc2_panel->appendChildWidget(osc2_in_o1);
        connectWidget(AParameters[PAR_OSC2_IN_O1],osc2_in_o1);

        sa_tyr_SmallKnobWidget* osc2_in_o2 = new sa_tyr_SmallKnobWidget( MIP_DRect(40,180,30,45),"O2");
        osc2_panel->appendChildWidget(osc2_in_o2);
        connectWidget(AParameters[PAR_OSC2_IN_O2],osc2_in_o2);

        sa_tyr_SmallKnobWidget* osc2_in_r1 = new sa_tyr_SmallKnobWidget( MIP_DRect(70,180,30,45),"R1");
        osc2_panel->appendChildWidget(osc2_in_r1);
        connectWidget(AParameters[PAR_OSC2_IN_R1],osc2_in_r1);

        sa_tyr_SmallKnobWidget* osc2_in_r2 = new sa_tyr_SmallKnobWidget( MIP_DRect(100,180,30,45),"R2");
        osc2_panel->appendChildWidget(osc2_in_r2);
        connectWidget(AParameters[PAR_OSC2_IN_R2],osc2_in_r2);

        sa_tyr_SmallKnobWidget* osc2_in_n = new sa_tyr_SmallKnobWidget( MIP_DRect(130,180,30,45),"N");
        osc2_panel->appendChildWidget(osc2_in_n);
        connectWidget(AParameters[PAR_OSC2_IN_N],osc2_in_n);

        sa_tyr_SmallKnobWidget* osc2_in_s = new sa_tyr_SmallKnobWidget( MIP_DRect(160,180,30,45),"S");
        osc2_panel->appendChildWidget(osc2_in_s);
        connectWidget(AParameters[PAR_OSC2_IN_S],osc2_in_s);

        sa_tyr_SmallKnobWidget* osc2_in_i = new sa_tyr_SmallKnobWidget( MIP_DRect(190,180,30,45),"I");
        osc2_panel->appendChildWidget(osc2_in_i);
        connectWidget(AParameters[PAR_OSC2_IN_I],osc2_in_i);

        sa_tyr_SmallKnobWidget* osc2_in_a = new sa_tyr_SmallKnobWidget( MIP_DRect(220,180,30,45),"A");
        osc2_panel->appendChildWidget(osc2_in_a);
        connectWidget(AParameters[PAR_OSC2_IN_A],osc2_in_a);

        // ps

        sa_tyr_SliderWidget* osc2_phase_shaper_slider = new sa_tyr_SliderWidget(MIP_DRect(10,240,115,20),"Phase",0.0,true);
        osc2_panel->appendChildWidget(osc2_phase_shaper_slider);
        connectWidget(AParameters[PAR_OSC2_PM_AMOUNT],osc2_phase_shaper_slider);

        sa_tyr_SelectorWidget* osc2_phase_shaper_selector = new sa_tyr_SelectorWidget( MIP_DRect(135,240,115,20),"Phase Shp",phase_shaper_type_menu );
        osc2_panel->appendChildWidget(osc2_phase_shaper_selector);
        connectWidget(AParameters[PAR_OSC2_PM_TYPE],osc2_phase_shaper_selector);

        // ws

        sa_tyr_SliderWidget* osc2_wave_shaper_slider = new sa_tyr_SliderWidget(MIP_DRect(10,270,115,20),"Wave",0.0,true);
        osc2_panel->appendChildWidget(osc2_wave_shaper_slider);
        connectWidget(AParameters[PAR_OSC2_WM_AMOUNT],osc2_wave_shaper_slider);

        sa_tyr_SelectorWidget* osc2_wave_shaper_selector = new sa_tyr_SelectorWidget( MIP_DRect(135,270,115,20),"Wave Shp",wave_shaper_type_menu );
        osc2_panel->appendChildWidget(osc2_wave_shaper_selector);
        connectWidget(AParameters[PAR_OSC2_WM_TYPE],osc2_wave_shaper_selector);

// ----- res2 -----

      sa_tyr_SectionPanelWidget* res2_panel = new sa_tyr_SectionPanelWidget(MIP_DRect(280,320,260,260), "RES2", 20 );
      controls->appendChildWidget(res2_panel);

        // type

        sa_tyr_SelectorWidget* res2_type_selector = new sa_tyr_SelectorWidget( MIP_DRect(10,30,230,20),"Res Type",res_type_menu );
        res2_panel->appendChildWidget(res2_type_selector);
        connectWidget(AParameters[PAR_RES2_TYPE],res2_type_selector);

        // inputs

        sa_tyr_SmallKnobWidget* res2_in_o1 = new sa_tyr_SmallKnobWidget( MIP_DRect(10,60,30,45),"O1");
        res2_panel->appendChildWidget(res2_in_o1);
        connectWidget(AParameters[PAR_RES2_IN_O1],res2_in_o1);

        sa_tyr_SmallKnobWidget* res2_in_o2 = new sa_tyr_SmallKnobWidget( MIP_DRect(40,60,30,45),"O2");
        res2_panel->appendChildWidget(res2_in_o2);
        connectWidget(AParameters[PAR_RES2_IN_O2],res2_in_o2);

        sa_tyr_SmallKnobWidget* res2_in_r1 = new sa_tyr_SmallKnobWidget( MIP_DRect(70,60,30,45),"R1");
        res2_panel->appendChildWidget(res2_in_r1);
        connectWidget(AParameters[PAR_RES2_IN_R1],res2_in_r1);

        sa_tyr_SmallKnobWidget* res2_in_r2 = new sa_tyr_SmallKnobWidget( MIP_DRect(100,60,30,45),"R2");
        res2_panel->appendChildWidget(res2_in_r2);
        connectWidget(AParameters[PAR_RES2_IN_R2],res2_in_r2);

        sa_tyr_SmallKnobWidget* res2_in_n = new sa_tyr_SmallKnobWidget( MIP_DRect(130,60,30,45),"N");
        res2_panel->appendChildWidget(res2_in_n);
        connectWidget(AParameters[PAR_RES2_IN_N],res2_in_n);

        sa_tyr_SmallKnobWidget* res2_in_s = new sa_tyr_SmallKnobWidget( MIP_DRect(160,60,30,45),"S");
        res2_panel->appendChildWidget(res2_in_s);
        connectWidget(AParameters[PAR_RES2_IN_S],res2_in_s);

        sa_tyr_SmallKnobWidget* res2_in_i = new sa_tyr_SmallKnobWidget( MIP_DRect(190,60,30,45),"I");
        res2_panel->appendChildWidget(res2_in_i);
        connectWidget(AParameters[PAR_RES2_IN_I],res2_in_i);

        sa_tyr_SmallKnobWidget* res2_in_a = new sa_tyr_SmallKnobWidget( MIP_DRect(220,60,30,45),"A");
        res2_panel->appendChildWidget(res2_in_a);
        connectWidget(AParameters[PAR_RES2_IN_A],res2_in_a);

        // shape

        sa_tyr_BigKnobWidget* res2_nshape_knob = new sa_tyr_BigKnobWidget( MIP_DRect(10,110,50,82),"Shape");
        res2_panel->appendChildWidget(res2_nshape_knob);
        connectWidget(AParameters[PAR_RES2_SHAPE],res2_nshape_knob);

        // fb

        sa_tyr_BigKnobWidget* res2_fb_knob = new sa_tyr_BigKnobWidget( MIP_DRect(70,110,50,82),"FB");
        res2_panel->appendChildWidget(res2_fb_knob);
        connectWidget(AParameters[PAR_RES2_FB],res2_fb_knob);

        // damp

        sa_tyr_BigKnobWidget* res2_damp_knob = new sa_tyr_BigKnobWidget( MIP_DRect(130,110,50,82),"Damp");
        res2_panel->appendChildWidget(res2_damp_knob);
        connectWidget(AParameters[PAR_RES2_DAMP],res2_damp_knob);

        // rough

        sa_tyr_BigKnobWidget* res2_rough_knob = new sa_tyr_BigKnobWidget( MIP_DRect(190,110,50,82),"Rough",0.0,true);
        res2_rough_knob->getKnobWidget()->setBipolar(false);
        res2_panel->appendChildWidget(res2_rough_knob);
        connectWidget(AParameters[PAR_RES2_ROUGH],res2_rough_knob);

        // speed

        sa_tyr_SliderWidget* res2_speed_slider = new sa_tyr_SliderWidget( MIP_DRect(10,200,240,20),"speed" );
        res2_panel->appendChildWidget(res2_speed_slider);
        connectWidget(AParameters[PAR_RES2_SPEED],res2_speed_slider);

        // oct

        sa_tyr_DragValueWidget* res2_oct_slider = new sa_tyr_DragValueWidget( MIP_DRect(10,230,72,20),"Oct");
        res2_panel->appendChildWidget(res2_oct_slider);
        connectWidget(AParameters[PAR_RES2_OCT],res2_oct_slider);

        // semi

        sa_tyr_DragValueWidget* res2_semi_slider = new sa_tyr_DragValueWidget( MIP_DRect(94,230,72,20),"Semi");
        res2_panel->appendChildWidget(res2_semi_slider);
        connectWidget(AParameters[PAR_RES2_SEMI],res2_semi_slider);

        // cent

        sa_tyr_DragValueWidget* res2_cent_slider = new sa_tyr_DragValueWidget( MIP_DRect(178,230,72,20),"Cent",0.0,true,0.0,0.1);
        res2_panel->appendChildWidget(res2_cent_slider);
        connectWidget(AParameters[PAR_RES2_CENT],res2_cent_slider);

//
//
// right --------------------------------------------------
//
//

      // voices

      sa_tyr_SectionPanelWidget* voice2_panel = new sa_tyr_SectionPanelWidget(MIP_DRect(550,10,276,75+30), "Voices", 20 );
      controls->appendChildWidget(voice2_panel);

        // voices

        MVoiceWidget = new sa_tyr_VoiceWidget( MIP_DRect(10,30,256,8) );
        voice2_panel->appendChildWidget(MVoiceWidget);

        MPlayingVoicesWidget = new MIP_ValueWidget( MIP_DRect(10,45,150,15), "Playing:",0 );
        MPlayingVoicesWidget->setNumDigits(0);
        MPlayingVoicesWidget->setDrawBorder(false);
        MPlayingVoicesWidget->setFillBackground(false);
        voice2_panel->appendChildWidget(MPlayingVoicesWidget);

        MReleasedVoicesWidget = new MIP_ValueWidget( MIP_DRect(10,60,150,15), "Released:",0 );
        MReleasedVoicesWidget->setNumDigits(0);
        MReleasedVoicesWidget->setDrawBorder(false);
        MReleasedVoicesWidget->setFillBackground(false);
        voice2_panel->appendChildWidget(MReleasedVoicesWidget);

        MTotalVoicesWidget = new MIP_ValueWidget( MIP_DRect(10,75,150,15), "Total:",0 );
        MTotalVoicesWidget->setNumDigits(0);
        MTotalVoicesWidget->setDrawBorder(false);
        MTotalVoicesWidget->setFillBackground(false);
        voice2_panel->appendChildWidget(MTotalVoicesWidget);

      // ----- flt1 -----

      sa_tyr_SectionPanelWidget* flt1_panel = new sa_tyr_SectionPanelWidget(MIP_DRect(550,125,276,145), "Filter", 20 );
      controls->appendChildWidget(flt1_panel);

        // type

        sa_tyr_SelectorWidget* flt1_type_selector = new sa_tyr_SelectorWidget( MIP_DRect(10,30,256,20),"Flt Type",flt_type_menu );
        flt1_panel->appendChildWidget(flt1_type_selector);
        connectWidget(AParameters[PAR_FLT1_TYPE],flt1_type_selector);

        // freq

        sa_tyr_BigKnobWidget* flt1_freq_knob = new sa_tyr_BigKnobWidget( MIP_DRect(10,60,50,82),"Freq");
        flt1_panel->appendChildWidget(flt1_freq_knob);
        connectWidget(AParameters[PAR_FLT1_FREQ],flt1_freq_knob);

        // res

        sa_tyr_BigKnobWidget* flt1_res_knob = new sa_tyr_BigKnobWidget( MIP_DRect(70,60,50,82),"Res");
        flt1_panel->appendChildWidget(flt1_res_knob);
        connectWidget(AParameters[PAR_FLT1_RES],flt1_res_knob);

      // ----- env1 -----

      sa_tyr_SectionPanelWidget* env1_panel = new sa_tyr_SectionPanelWidget(MIP_DRect(550,280,276,115), "Env", 20 );
      controls->appendChildWidget(env1_panel);

        // att

        sa_tyr_BigKnobWidget* env1_att_knob = new sa_tyr_BigKnobWidget( MIP_DRect(10,30,50,82),"Att");
        env1_panel->appendChildWidget(env1_att_knob);
        connectWidget(AParameters[PAR_ENV1_ATT],env1_att_knob);

        // dec

        sa_tyr_BigKnobWidget* env1_dec_knob = new sa_tyr_BigKnobWidget( MIP_DRect(70,30,50,82),"Dec");
        env1_panel->appendChildWidget(env1_dec_knob);
        connectWidget(AParameters[PAR_ENV1_DEC],env1_dec_knob);

        // sus

        sa_tyr_BigKnobWidget* env1_sus_knob = new sa_tyr_BigKnobWidget( MIP_DRect(130,30,50,82),"Sus");
        env1_panel->appendChildWidget(env1_sus_knob);
        connectWidget(AParameters[PAR_ENV1_SUS],env1_sus_knob);

        // rel

        sa_tyr_BigKnobWidget* env1_rel_knob = new sa_tyr_BigKnobWidget( MIP_DRect(190,30,50,82),"Rel");
        env1_panel->appendChildWidget(env1_rel_knob);
        connectWidget(AParameters[PAR_ENV1_REL],env1_rel_knob);

      // ----- global -----

      sa_tyr_SectionPanelWidget* global_panel = new sa_tyr_SectionPanelWidget(MIP_DRect(550,405,276,210), "Mix", 20 );
      controls->appendChildWidget(global_panel);

        // vol

        sa_tyr_BigKnobWidget* global_vol_knob = new sa_tyr_BigKnobWidget( MIP_DRect(10,30,50,82),"Vol");
        global_panel->appendChildWidget(global_vol_knob);
        connectWidget(AParameters[PAR_MASTER_VOL],global_vol_knob);

        // pan

        sa_tyr_BigKnobWidget* global_pan_knob = new sa_tyr_BigKnobWidget( MIP_DRect(70,30,50,82),"Pan",0.0,true);
        global_panel->appendChildWidget(global_pan_knob);
        connectWidget(AParameters[PAR_MASTER_PAN],global_pan_knob);

        // osc1 out

        sa_tyr_BigKnobWidget* global_osc1_out_knob = new sa_tyr_BigKnobWidget( MIP_DRect(10,120,50,82),"O1");
        global_panel->appendChildWidget(global_osc1_out_knob);
        connectWidget(AParameters[PAR_MASTER_OSC1_OUT],global_osc1_out_knob);

        // osc2 out

        sa_tyr_BigKnobWidget* global_osc2_out_knob = new sa_tyr_BigKnobWidget( MIP_DRect(70,120,50,82),"O2");
        global_panel->appendChildWidget(global_osc2_out_knob);
        connectWidget(AParameters[PAR_MASTER_OSC2_OUT],global_osc2_out_knob);

        // res1 out

        sa_tyr_BigKnobWidget* global_res1_out_knob = new sa_tyr_BigKnobWidget( MIP_DRect(130,120,50,82),"R1");
        global_panel->appendChildWidget(global_res1_out_knob);
        connectWidget(AParameters[PAR_MASTER_RES1_OUT],global_res1_out_knob);

        // res2 out

        sa_tyr_BigKnobWidget* global_res2_out_knob = new sa_tyr_BigKnobWidget( MIP_DRect(190,120,50,82),"R2");
        global_panel->appendChildWidget(global_res2_out_knob);
        connectWidget(AParameters[PAR_MASTER_RES2_OUT],global_res2_out_knob);

//
//
// keyboard --------------------------------------------------
//
//

        //MIP_ScrollBoxWidget* scrollbox = new MIP_ScrollBoxWidget( MIP_DRect(0,SA_TYR_EDITOR_HEIGHT-60,SA_TYR_EDITOR_WIDTH,40),false,true);
        //MEditorPanel->appendChildWidget(scrollbox);
        //for (uint32_t i=0; i<11; i++) {
        //  MKeyboards[i] = new MIP_KeyboardWidget(MIP_DRect( i*80, 0, 80, 40 ));
        //  MKeyboards[i]->Layout.alignment = MIP_WIDGET_ALIGN_LEFT_TOP;
        //  scrollbox->appendChildWidget( MKeyboards[i] );
        //}

//
//
// --------------------------------------------------
//
//
    // menus

    MEditorPanel->appendChildWidget(osc_type_menu);
    MEditorPanel->appendChildWidget(res_type_menu);
    MEditorPanel->appendChildWidget(flt_type_menu);
    MEditorPanel->appendChildWidget(wave_shaper_type_menu);
    MEditorPanel->appendChildWidget(phase_shaper_type_menu);
    //MEditorPanel->appendChildWidget(osc_mix_type_menu);

    //

    //MEditorPanel->Layout.aspectRatio = EDITOR_WIDTH / EDITOR_HEIGHT;
    MEditorPanel->setRectMode(MIP_WIDGET_SCALE_MODE_INITIAL_RATIO);

    window->appendChildWidget(MEditorPanel);


  }

  //----------

  void timer_update(sa_tyr_voice_manager* voices) {
    if (MIsEditorOpen) { // && not resizing

      uint32_t playing = 0;
      uint32_t released = 0;
      for (uint32_t v=0; v<SA_TYR_NUM_VOICES; v++) {
        uint32_t state = voices->getVoiceState(v);
        MVoiceWidget->voice_state[v] = state;
        if (state == MIP_VOICE_PLAYING) playing += 1;
        if (state == MIP_VOICE_RELEASED) released += 1;
      }

      MPlayingVoicesWidget->setValue(playing);
      MReleasedVoicesWidget->setValue(released);
      MTotalVoicesWidget->setValue(playing + released);

      MVoiceWidget->redraw();
      MPlayingVoicesWidget->redraw();
      MReleasedVoicesWidget->redraw();
      MTotalVoicesWidget->redraw();

    }
  }

  //----------

  //void updateWaveformWidget() {
  //}


};

//----------------------------------------------------------------------
#endif
