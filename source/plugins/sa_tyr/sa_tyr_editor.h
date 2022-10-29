#ifndef sa_tyr_editor_included
#define sa_tyr_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/widgets/mip_widgets.h"
#include "plugin/mip_editor.h"

#include "sa_tyr_widgets.h"

//----------------------------------------------------------------------

class sa_tyr_Editor
: public MIP_Editor
, public MIP_MenuListener {

//------------------------------
public:
//------------------------------

  sa_tyr_VoiceWidget* MVoiceWidget          = nullptr;
  MIP_ValueWidget*    MPlayingVoicesWidget  = nullptr;
  MIP_ValueWidget*    MReleasedVoicesWidget = nullptr;
  MIP_ValueWidget*    MTotalVoicesWidget    = nullptr;

//------------------------------
public:
//------------------------------

  //sa_tyr_Editor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded, const clap_plugin_descriptor_t* descriptor)
  //: MIP_Editor(AListener,APlugin,AWidth,AHeight,AEmbedded) {

  //sa_tyr_Editor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded, const clap_plugin_descriptor_t* descriptor)
  sa_tyr_Editor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, MIP_ParameterArray AParameters)
  : MIP_Editor(AListener,AWidth,AHeight) {


    MIP_Window* window = getWindow();

    //----------

    // osc menu

    MIP_MenuWidget* osc_type_menu = new MIP_MenuWidget(MIP_DRect(0,0,160,60),this);
    //osc_type_menu->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    osc_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 0,160,20), "Morph" ));
    osc_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,20,160,20), "DSF" ));
    osc_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,40,160,20), "Wavetable" ));
    //osc_type_menu->setItemSize(100,20);
    //osc_type_menu->setItemLayout(1,3);
    //osc_type_menu->setMenuMirror(false,false);
    //MEditorPanel->appendChildWidget(osc_type_menu);

    for (uint32_t i=0; i<osc_type_menu->getNumChildWidgets(); i++) {
      MIP_MenuItemWidget* w = (MIP_MenuItemWidget*)osc_type_menu->getChildWidget(i);
      w->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      w->setTextSize(-0.7);
    }

    // res menu

    MIP_MenuWidget* res_type_menu = new MIP_MenuWidget(MIP_DRect(0,0,160,60),this);
    //res_type_menu->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    res_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 0,160,20), "Pluck" ));
    res_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,20,160,20), "Rep. Pluck" ));
    res_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,40,160,20), "(not yet)" ));
    //res_type_menu->setItemSize(120,20);
    //res_type_menu->setItemLayout(1,3);
    //res_type_menu->setMenuMirror(false,false);
    //MEditorPanel->appendChildWidget(res_type_menu);

    for (uint32_t i=0; i<res_type_menu->getNumChildWidgets(); i++) {
      MIP_MenuItemWidget* w = (MIP_MenuItemWidget*)res_type_menu->getChildWidget(i);
      w->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      w->setTextSize(-0.7);
    }

    // flt menu

    MIP_MenuWidget* flt_type_menu = new MIP_MenuWidget(MIP_DRect(0,0,160,100),this);
    //flt_type_menu->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    flt_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 0,160,20), "Off" ));
    flt_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,20,160,20), "Lowpass" ));
    flt_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,40,160,20), "Highpass" ));
    flt_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,60,160,20), "Bandpass" ));
    flt_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,80,160,20), "Notch" ));
    //flt_type_menu->setItemSize(100,20);
    //flt_type_menu->setItemLayout(1,5);
    //flt_type_menu->setMenuMirror(false,false);
    //MEditorPanel->appendChildWidget(flt_type_menu);

    for (uint32_t i=0; i<flt_type_menu->getNumChildWidgets(); i++) {
      MIP_MenuItemWidget* w = (MIP_MenuItemWidget*)flt_type_menu->getChildWidget(i);
      w->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      w->setTextSize(-0.7);
    }

    // ws

    MIP_MenuWidget* wave_shaper_type_menu = new MIP_MenuWidget(MIP_DRect(0,0,160,180),this);
    //wave_shaper_type_menu->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    wave_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,  0,160,20), "Off" ));
    wave_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 20,160,20), "Curve" ));
    wave_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 40,160,20), "Ampl Mod" ));
    wave_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 60,160,20), "Ring Mod" ));
    wave_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 80,160,20), "Replace" ));
    wave_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,100,160,20), "Negate" ));
    wave_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,120,160,20), "Sign" ));
    wave_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,140,160,20), "Max" ));
    wave_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,160,160,20), "Fold" ));
    //wave_shaper_type_menu->setItemSize(100,20);
    //wave_shaper_type_menu->setItemLayout(1,9);
    //wave_shaper_type_menu->setMenuMirror(false,false);
    //MEditorPanel->appendChildWidget(wave_shaper_type_menu);

    for (uint32_t i=0; i<wave_shaper_type_menu->getNumChildWidgets(); i++) {
      MIP_MenuItemWidget* w = (MIP_MenuItemWidget*)wave_shaper_type_menu->getChildWidget(i);
      w->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      w->setTextSize(-0.7);
    }

    // ps

    MIP_MenuWidget* phase_shaper_type_menu = new MIP_MenuWidget(MIP_DRect(0,0,160,140),this);
    //phase_shaper_type_menu->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    phase_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,  0,160,20), "Off" ));
    phase_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 20,160,20), "Curve" ));
    phase_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 40,160,20), "Sync" ));
    phase_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 60,160,20), "Sync / Clamp" ));
    phase_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0, 80,160,20), "Phase Mod" ));
    phase_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,100,160,20), "Freq Mod" ));
    phase_shaper_type_menu->appendChildWidget( new MIP_MenuItemWidget(MIP_DRect(0,120,160,20), "Flip" ));
    //phase_shaper_type_menu->setItemSize(100,20);
    //phase_shaper_type_menu->setItemLayout(1,7);
    //phase_shaper_type_menu->setMenuMirror(false,false);
    //MEditorPanel->appendChildWidget(phase_shaper_type_menu);

    for (uint32_t i=0; i<phase_shaper_type_menu->getNumChildWidgets(); i++) {
      MIP_MenuItemWidget* w = (MIP_MenuItemWidget*)phase_shaper_type_menu->getChildWidget(i);
      w->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
      w->setTextSize(-0.7);
    }

    //----------

    // panel

    MIP_PanelWidget* MEditorPanel = new MIP_PanelWidget(MIP_DRect(0,0,EDITOR_WIDTH,EDITOR_HEIGHT));
    MEditorPanel->setDrawBorder(false);
    MEditorPanel->setFillBackground(false);
    //MEditorPanel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

    // header

    MIP_SAHeaderWidget* header = new MIP_SAHeaderWidget(MIP_DRect(0,0,EDITOR_WIDTH,80));
    //header->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    //header->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    header->setPluginName(" tyr");
    header->setPluginVersion("0.0.8");
    MEditorPanel->appendChildWidget(header);

    // controls

    MIP_PanelWidget* controls = new MIP_PanelWidget(MIP_DRect(0,80,EDITOR_WIDTH,EDITOR_HEIGHT-80));
    //controls->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    //controls->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    controls->setDrawBorder(false);
    controls->setFillBackground(true);
    controls->setBackgroundColor(0.6);
    //controls->Layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    MEditorPanel->appendChildWidget(controls);

//
//
// left --------------------------------------------------
//
//

// ----- osc1

      MIP_PanelWidget* osc1_panel = new MIP_PanelWidget(MIP_DRect(10,10,250,300) );
      controls->appendChildWidget(osc1_panel);
      osc1_panel->setFillBackground(true);
      osc1_panel->setBackgroundColor(0.55);

        // text

        MIP_TextWidget* osc1_text = new MIP_TextWidget(MIP_DRect(0,0,250,20),"OSC1");
        osc1_panel->appendChildWidget(osc1_text);
        osc1_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        osc1_text->setDrawBorder(true);
        osc1_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        osc1_text->setDrawBorder(false);
        osc1_text->setFillBackground(true);
        osc1_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // type

        MIP_SelectorWidget* osc1_type_selector = new MIP_SelectorWidget( MIP_DRect(10,30,230,20), "Osc Type", osc_type_menu );
        osc1_type_selector->setTextSize(-0.7);

        osc1_panel->appendChildWidget(osc1_type_selector);
        //osc1_type_selector->setMenu(osc_type_menu);
        //osc1_type_selector->setSelected(1);
        //connectWidget(AParameters[],osc1_type_selector,PAR_OSC1_TYPE);
        connectWidget(AParameters[PAR_OSC1_TYPE], osc1_type_selector );

        // shape

        MIP_Knob2Widget* osc1_shape_knob = new MIP_Knob2Widget( MIP_DRect(10,60,50,82),"Shape");
        osc1_panel->appendChildWidget(osc1_shape_knob);
        connectWidget(AParameters[PAR_OSC1_SHAPE],osc1_shape_knob);

        // width

        MIP_Knob2Widget* osc1_width_knob = new MIP_Knob2Widget( MIP_DRect(70,60,50,82),"Width");
        osc1_panel->appendChildWidget(osc1_width_knob);
//        osc1_width_knob->getKnobWidget()->setSnap(true);
//        osc1_width_knob->getKnobWidget()->setSnapPos(0.5);
//        osc1_width_knob->getKnobWidget()->setSnapDist(0.05);
//        osc1_width_knob->getKnobWidget()->setBipolar(true);
        connectWidget(AParameters[PAR_OSC1_WIDTH],osc1_width_knob);

        // oct

        MIP_DragValueWidget* osc1_oct_slider = new MIP_DragValueWidget( MIP_DRect(10,150,70,20),"Oct");
        osc1_panel->appendChildWidget(osc1_oct_slider);
        //osc1_oct_slider->setQuantize(true);
//        osc1_oct_slider->setSnap(true);
//        osc1_oct_slider->setSnapPos(0.5);
//        osc1_oct_slider->setSnapDist(0.05);
//        osc1_oct_slider->setDrawParamName(false);
        osc1_oct_slider->setDrawBorder(true);
        osc1_oct_slider->setBorderColor(0.4);
        osc1_oct_slider->setFillBackground(true);
        osc1_oct_slider->setBackgroundColor(0.6);
        connectWidget(AParameters[PAR_OSC1_OCT],osc1_oct_slider);

        // semi

        MIP_DragValueWidget* osc1_semi_slider = new MIP_DragValueWidget( MIP_DRect(90,150,70,20),"Semi");
        osc1_panel->appendChildWidget(osc1_semi_slider);
//        osc1_semi_slider->setSnap(true);
//        osc1_semi_slider->setSnapPos(0.5);
//        osc1_semi_slider->setSnapDist(0.05);
//        osc1_semi_slider->setDrawParamName(false);
        osc1_semi_slider->setDrawBorder(true);
        osc1_semi_slider->setBorderColor(0.4);
        osc1_semi_slider->setFillBackground(true);
        osc1_semi_slider->setBackgroundColor(0.6);
        connectWidget(AParameters[PAR_OSC1_SEMI],osc1_semi_slider);

        // cent

        MIP_DragValueWidget* osc1_cent_slider = new MIP_DragValueWidget( MIP_DRect(170,150,70,20),"Cent");
        osc1_panel->appendChildWidget(osc1_cent_slider);
        //osc1_cent_slider->setSnap(true);
        //osc1_cent_slider->setSnapPos(0.5);
        //osc1_cent_slider->setSnapDist(0.05);
        //osc1_cent_slider->setDrawParamName(false);
        osc1_cent_slider->setDrawBorder(true);
        osc1_cent_slider->setBorderColor(0.4);
        osc1_cent_slider->setFillBackground(true);
        osc1_cent_slider->setBackgroundColor(0.6);
        connectWidget(AParameters[PAR_OSC1_CENT],osc1_cent_slider);

        // inputs

        MIP_Knob2Widget* osc1_in_o1 = new MIP_Knob2Widget( MIP_DRect(10,180,30,45),"O1");
        osc1_panel->appendChildWidget(osc1_in_o1);
        //osc1_in_o1->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_OSC1_IN_O1],osc1_in_o1);

        MIP_Knob2Widget* osc1_in_o2 = new MIP_Knob2Widget( MIP_DRect(45,180,30,45),"O2");
        osc1_panel->appendChildWidget(osc1_in_o2);
        //osc1_in_o2->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_OSC1_IN_O2],osc1_in_o2);

        MIP_Knob2Widget* osc1_in_r1 = new MIP_Knob2Widget( MIP_DRect(80,180,30,45),"R1");
        osc1_panel->appendChildWidget(osc1_in_r1);
        //osc1_in_r1->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_OSC1_IN_R1],osc1_in_r1);

        MIP_Knob2Widget* osc1_in_r2 = new MIP_Knob2Widget( MIP_DRect(115,180,30,45),"R2");
        osc1_panel->appendChildWidget(osc1_in_r2);
        //osc1_in_r2->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_OSC1_IN_R2],osc1_in_r2);

        MIP_Knob2Widget* osc1_in_n = new MIP_Knob2Widget( MIP_DRect(150,180,30,45),"N");
        osc1_panel->appendChildWidget(osc1_in_n);
        //osc1_in_n->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_OSC1_IN_N],osc1_in_n);

        // ps

        MIP_SliderWidget* osc1_phase_shaper_slider = new MIP_SliderWidget(MIP_DRect(10,240,110,20),"Phase");
        osc1_panel->appendChildWidget(osc1_phase_shaper_slider);
        connectWidget(AParameters[PAR_OSC1_PM_AMOUNT],osc1_phase_shaper_slider);

        MIP_SelectorWidget* osc1_phase_shaper_selector = new MIP_SelectorWidget( MIP_DRect(130,240,110,20),"Phase Shp",phase_shaper_type_menu );
        osc1_phase_shaper_selector->setTextSize(-0.7);

        osc1_panel->appendChildWidget(osc1_phase_shaper_selector);
        //osc1_phase_shaper_selector->setMenu(phase_shaper_type_menu);
        //osc1_phase_shaper_selector->setSelected(0);
        connectWidget(AParameters[PAR_OSC1_PM_TYPE],osc1_phase_shaper_selector);

        // ws

        MIP_SliderWidget* osc1_wave_shaper_slider = new MIP_SliderWidget(MIP_DRect(10,270,110,20),"Wave");
        osc1_panel->appendChildWidget(osc1_wave_shaper_slider);
        connectWidget(AParameters[PAR_OSC1_WM_AMOUNT],osc1_wave_shaper_slider);

        MIP_SelectorWidget* osc1_wave_shaper_selector = new MIP_SelectorWidget( MIP_DRect(130,270,110,20),"Wave Shp",wave_shaper_type_menu );
        osc1_wave_shaper_selector->setTextSize(-0.7);

        osc1_panel->appendChildWidget(osc1_wave_shaper_selector);
        //osc1_wave_shaper_selector->setMenu(wave_shaper_type_menu);
        //osc1_wave_shaper_selector->setSelected(0);
        connectWidget(AParameters[PAR_OSC1_WM_TYPE],osc1_wave_shaper_selector);

// ----- res1 -----

      MIP_PanelWidget* res1_panel = new MIP_PanelWidget(MIP_DRect(10,320,250,260) );
      //res1_panel->setBackgroundColor(0.55);
      controls->appendChildWidget(res1_panel);

        // text

        MIP_TextWidget* res1_text = new MIP_TextWidget(MIP_DRect(0,0,250,20),"RES1");
        res1_panel->appendChildWidget(res1_text);
        res1_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        res1_text->setDrawBorder(true);
        res1_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        res1_text->setDrawBorder(false);
        res1_text->setFillBackground(true);
        res1_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // type

        MIP_SelectorWidget* res1_type_selector = new MIP_SelectorWidget( MIP_DRect(10,30,230,20),"Res Type",res_type_menu );
        res1_type_selector->setTextSize(-0.7);
        res1_panel->appendChildWidget(res1_type_selector);
//        res1_type_selector->setText("");
//        res1_type_selector->setMenu(res_type_menu);
        connectWidget(AParameters[PAR_RES1_TYPE],res1_type_selector);

        // inputs

        MIP_Knob2Widget* res1_in_o1 = new MIP_Knob2Widget( MIP_DRect(10,60,30,45),"O1");
        res1_panel->appendChildWidget(res1_in_o1);
//        res1_in_o1->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_RES1_IN_O1],res1_in_o1);

        MIP_Knob2Widget* res1_in_o2 = new MIP_Knob2Widget( MIP_DRect(45,60,30,45),"O2");
        res1_panel->appendChildWidget(res1_in_o2);
//        res1_in_o2->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_RES1_IN_O2],res1_in_o2);

        MIP_Knob2Widget* res1_in_r1 = new MIP_Knob2Widget( MIP_DRect(80,60,30,45),"R1");
        res1_panel->appendChildWidget(res1_in_r1);
//        res1_in_r1->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_RES1_IN_R1],res1_in_r1);

        MIP_Knob2Widget* res1_in_r2 = new MIP_Knob2Widget( MIP_DRect(115,60,30,45),"R2");
        res1_panel->appendChildWidget(res1_in_r2);
//        res1_in_r2->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_RES1_IN_R2],res1_in_r2);

        MIP_Knob2Widget* res1_in_n = new MIP_Knob2Widget( MIP_DRect(150,60,30,45),"N");
        res1_panel->appendChildWidget(res1_in_n);
//        res1_in_n->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_RES1_IN_N],res1_in_n);

        // shape

        MIP_Knob2Widget* res1_nshape_knob = new MIP_Knob2Widget( MIP_DRect(10,110,50,82),"Shape");
        res1_panel->appendChildWidget(res1_nshape_knob);
        connectWidget(AParameters[PAR_RES1_SHAPE],res1_nshape_knob);

        // fb

        MIP_Knob2Widget* res1_fb_knob = new MIP_Knob2Widget( MIP_DRect(70,110,50,82),"FB");
        res1_panel->appendChildWidget(res1_fb_knob);
        connectWidget(AParameters[PAR_RES1_FB],res1_fb_knob);

        // damp

        MIP_Knob2Widget* res1_damp_knob = new MIP_Knob2Widget( MIP_DRect(130,110,50,82),"Damp");
        res1_panel->appendChildWidget(res1_damp_knob);
        connectWidget(AParameters[PAR_RES1_DAMP],res1_damp_knob);

        // rough

        MIP_Knob2Widget* res1_rough_knob = new MIP_Knob2Widget( MIP_DRect(190,110,50,82),"Rough");
        res1_panel->appendChildWidget(res1_rough_knob);
//        res1_rough_knob->getKnobWidget()->setSnap(true);
//        res1_rough_knob->getKnobWidget()->setSnapPos(0.5);
//        res1_rough_knob->getKnobWidget()->setSnapDist(0.05);
        connectWidget(AParameters[PAR_RES1_ROUGH],res1_rough_knob);

        // speed

        MIP_SliderWidget* res1_speed_slider = new MIP_SliderWidget( MIP_DRect(10,200,230,20),"speed" );
        res1_panel->appendChildWidget(res1_speed_slider);
        connectWidget(AParameters[PAR_RES1_SPEED],res1_speed_slider);

        // oct

        MIP_DragValueWidget* res1_oct_slider = new MIP_DragValueWidget( MIP_DRect(10,230,70,20),"Oct");
        res1_panel->appendChildWidget(res1_oct_slider);
//        res1_oct_slider->setSnap(true);
//        res1_oct_slider->setSnapPos(0.5);
//        res1_oct_slider->setSnapDist(0.05);
//        res1_oct_slider->setDrawParamName(false);
        res1_oct_slider->setDrawBorder(true);
        res1_oct_slider->setBorderColor(0.4);
        res1_oct_slider->setFillBackground(true);
        res1_oct_slider->setBackgroundColor(0.6);
        connectWidget(AParameters[PAR_RES1_OCT],res1_oct_slider);

        // semi

        MIP_DragValueWidget* res1_semi_slider = new MIP_DragValueWidget( MIP_DRect(90,230,70,20),"Semi");
        res1_panel->appendChildWidget(res1_semi_slider);
//        res1_semi_slider->setSnap(true);
//        res1_semi_slider->setSnapPos(0.5);
//        res1_semi_slider->setSnapDist(0.05);
//        res1_semi_slider->setDrawParamName(false);
        res1_semi_slider->setDrawBorder(true);
        res1_semi_slider->setBorderColor(0.4);
        res1_semi_slider->setFillBackground(true);
        res1_semi_slider->setBackgroundColor(0.6);
        connectWidget(AParameters[PAR_RES1_SEMI],res1_semi_slider);

        // cent

        MIP_DragValueWidget* res1_cent_slider = new MIP_DragValueWidget( MIP_DRect(170,230,70,20),"Cent");
        res1_panel->appendChildWidget(res1_cent_slider);
//        res1_cent_slider->setSnap(true);
//        res1_cent_slider->setSnapPos(0.5);
//        res1_cent_slider->setSnapDist(0.05);
//        res1_cent_slider->setDrawParamName(false);
        res1_cent_slider->setDrawBorder(true);
        res1_cent_slider->setBorderColor(0.4);
        res1_cent_slider->setFillBackground(true);
        res1_cent_slider->setBackgroundColor(0.6);
        connectWidget(AParameters[PAR_RES1_CENT],res1_cent_slider);


//
//
// mid --------------------------------------------------
//
//

// ----- osc2

      MIP_PanelWidget* osc2_panel = new MIP_PanelWidget(MIP_DRect(10+260,10,250,300) );
      controls->appendChildWidget(osc2_panel);
      osc2_panel->setFillBackground(true);
      osc2_panel->setBackgroundColor(0.55);

        // text

        MIP_TextWidget* osc2_text = new MIP_TextWidget(MIP_DRect(0,0,250,20),"OSC2");
        osc2_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        osc2_text->setDrawBorder(true);
        osc2_panel->appendChildWidget(osc2_text);
        osc2_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        osc2_text->setDrawBorder(false);
        osc2_text->setFillBackground(true);
        osc2_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // type

        MIP_SelectorWidget* osc2_type_selector = new MIP_SelectorWidget( MIP_DRect(10,30,230,20),"Osc Type",osc_type_menu );
        osc2_type_selector->setTextSize(-0.7);
        osc2_panel->appendChildWidget(osc2_type_selector);
//        osc2_type_selector->setMenu(osc_type_menu);
//        osc2_type_selector->setSelected(1);
        connectWidget(AParameters[PAR_OSC2_TYPE],osc2_type_selector);

        // shape

        MIP_Knob2Widget* osc2_shape_knob = new MIP_Knob2Widget( MIP_DRect(10,60,50,82),"Shape");
        osc2_panel->appendChildWidget(osc2_shape_knob);
        connectWidget(AParameters[PAR_OSC2_SHAPE],osc2_shape_knob);

        // width

        MIP_Knob2Widget* osc2_width_knob = new MIP_Knob2Widget( MIP_DRect(70,60,50,82),"Width");
        osc2_panel->appendChildWidget(osc2_width_knob);
//        osc2_width_knob->getKnobWidget()->setSnap(true);
//        osc2_width_knob->getKnobWidget()->setSnapPos(0.5);
//        osc2_width_knob->getKnobWidget()->setSnapDist(0.05);
//        osc2_width_knob->getKnobWidget()->setBipolar(true);
        connectWidget(AParameters[PAR_OSC2_WIDTH],osc2_width_knob);

        // oct

        MIP_DragValueWidget* osc2_oct_slider = new MIP_DragValueWidget( MIP_DRect(10,150,70,20),"Oct");
        osc2_panel->appendChildWidget(osc2_oct_slider);
//        osc2_oct_slider->setSnap(true);
//        osc2_oct_slider->setSnapPos(0.5);
//        osc2_oct_slider->setSnapDist(0.05);
//        osc2_oct_slider->setDrawParamName(false);
        osc2_oct_slider->setDrawBorder(true);
        osc2_oct_slider->setBorderColor(0.4);
        osc2_oct_slider->setFillBackground(true);
        osc2_oct_slider->setBackgroundColor(0.6);
        connectWidget(AParameters[PAR_OSC2_OCT],osc2_oct_slider);

        // semi

        MIP_DragValueWidget* osc2_semi_slider = new MIP_DragValueWidget( MIP_DRect(90,150,70,20),"Semi");
        osc2_panel->appendChildWidget(osc2_semi_slider);
//        osc2_semi_slider->setSnap(true);
//        osc2_semi_slider->setSnapPos(0.5);
//        osc2_semi_slider->setSnapDist(0.05);
//        osc2_semi_slider->setDrawParamName(false);
        osc2_semi_slider->setDrawBorder(true);
        osc2_semi_slider->setBorderColor(0.4);
        osc2_semi_slider->setFillBackground(true);
        osc2_semi_slider->setBackgroundColor(0.6);
        connectWidget(AParameters[PAR_OSC2_SEMI],osc2_semi_slider);

        // cent

        MIP_DragValueWidget* osc2_cent_slider = new MIP_DragValueWidget( MIP_DRect(170,150,70,20),"Cent");
        osc2_panel->appendChildWidget(osc2_cent_slider);
//        osc2_cent_slider->setSnap(true);
//        osc2_cent_slider->setSnapPos(0.5);
//        osc2_cent_slider->setSnapDist(0.05);
//        osc2_cent_slider->setDrawParamName(false);
        osc2_cent_slider->setDrawBorder(true);
        osc2_cent_slider->setBorderColor(0.4);
        osc2_cent_slider->setFillBackground(true);
        osc2_cent_slider->setBackgroundColor(0.6);
        connectWidget(AParameters[PAR_OSC2_CENT],osc2_cent_slider);

        // inputs

        MIP_Knob2Widget* osc2_in_o1 = new MIP_Knob2Widget( MIP_DRect(10,180,30,45),"O1");
        osc2_panel->appendChildWidget(osc2_in_o1);
//        osc2_in_o1->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_OSC2_IN_O1],osc2_in_o1);

        MIP_Knob2Widget* osc2_in_o2 = new MIP_Knob2Widget( MIP_DRect(45,180,30,45),"O2");
        osc2_panel->appendChildWidget(osc2_in_o2);
//        osc2_in_o1->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_OSC2_IN_O2],osc2_in_o2);

        MIP_Knob2Widget* osc2_in_r1 = new MIP_Knob2Widget( MIP_DRect(80,180,30,45),"R1");
        osc2_panel->appendChildWidget(osc2_in_r1);
//        osc2_in_o2->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_OSC2_IN_R1],osc2_in_r1);

        MIP_Knob2Widget* osc2_in_r2 = new MIP_Knob2Widget( MIP_DRect(115,180,30,45),"R2");
        osc2_panel->appendChildWidget(osc2_in_r2);
//        osc2_in_r1->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_OSC2_IN_R2],osc2_in_r2);

        MIP_Knob2Widget* osc2_in_n = new MIP_Knob2Widget( MIP_DRect(150,180,30,45),"N");
        osc2_panel->appendChildWidget(osc2_in_n);
//        osc2_in_r2->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_OSC2_IN_N],osc2_in_n);

        // ps

        MIP_SliderWidget* osc2_phase_shaper_slider = new MIP_SliderWidget(MIP_DRect(10,240,110,20),"Phase");
        osc2_panel->appendChildWidget(osc2_phase_shaper_slider);
//        osc2_in_n->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_OSC2_PM_AMOUNT],osc2_phase_shaper_slider);

        MIP_SelectorWidget* osc2_phase_shaper_selector = new MIP_SelectorWidget( MIP_DRect(130,240,110,20),"Phase Shp",phase_shaper_type_menu );
        osc2_phase_shaper_selector->setTextSize(-0.7);
        osc2_panel->appendChildWidget(osc2_phase_shaper_selector);
//        osc2_phase_shaper_selector->setMenu(phase_shaper_type_menu);
//        osc2_phase_shaper_selector->setSelected(0);
        connectWidget(AParameters[PAR_OSC2_PM_TYPE],osc2_phase_shaper_selector);

        // ws

        MIP_SliderWidget* osc2_wave_shaper_slider = new MIP_SliderWidget(MIP_DRect(10,270,110,20),"Wave");
        osc2_panel->appendChildWidget(osc2_wave_shaper_slider);
        connectWidget(AParameters[PAR_OSC2_WM_AMOUNT],osc2_wave_shaper_slider);

        MIP_SelectorWidget* osc2_wave_shaper_selector = new MIP_SelectorWidget( MIP_DRect(130,270,110,20),"Wave Shp",wave_shaper_type_menu );
        osc2_wave_shaper_selector->setTextSize(-0.7);
        osc2_panel->appendChildWidget(osc2_wave_shaper_selector);
//        osc2_wave_shaper_selector->setMenu(wave_shaper_type_menu);
//        osc2_wave_shaper_selector->setSelected(0);
        connectWidget(AParameters[PAR_OSC2_WM_TYPE],osc2_wave_shaper_selector);

// ----- res2 -----

      MIP_PanelWidget* res2_panel = new MIP_PanelWidget(MIP_DRect(10+260,320,250,260) );
//      res2_panel->setBackgroundColor(0.55);
      controls->appendChildWidget(res2_panel);

        // text

        MIP_TextWidget* res2_text = new MIP_TextWidget(MIP_DRect(0,0,250,20),"RES2");
        res2_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        res2_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        res2_text->setDrawBorder(false);
        res2_text->setFillBackground(true);
        res2_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);
        res2_panel->appendChildWidget(res2_text);

        // type

        MIP_SelectorWidget* res2_type_selector = new MIP_SelectorWidget( MIP_DRect(10,30,230,20),"Res Type",res_type_menu );
        res2_type_selector->setTextSize(-0.7);
        res2_panel->appendChildWidget(res2_type_selector);
//        res2_type_selector->setText("---");
//        res2_type_selector->setMenu(res_type_menu);
        connectWidget(AParameters[PAR_RES2_TYPE],res2_type_selector);

        // inputs

        MIP_Knob2Widget* res2_in_o1 = new MIP_Knob2Widget( MIP_DRect(10,60,30,45),"O1");
        res2_panel->appendChildWidget(res2_in_o1);
//        res2_in_o1->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_RES2_IN_O1],res2_in_o1);

        MIP_Knob2Widget* res2_in_o2 = new MIP_Knob2Widget( MIP_DRect(45,60,30,45),"O2");
        res2_panel->appendChildWidget(res2_in_o2);
//        res2_in_o2->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_RES2_IN_O2],res2_in_o2);

        MIP_Knob2Widget* res2_in_r1 = new MIP_Knob2Widget( MIP_DRect(80,60,30,45),"R1");
        res2_panel->appendChildWidget(res2_in_r1);
//        res2_in_r1->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_RES2_IN_R1],res2_in_r1);

        MIP_Knob2Widget* res2_in_r2 = new MIP_Knob2Widget( MIP_DRect(115,60,30,45),"R2");
        res2_panel->appendChildWidget(res2_in_r2);
//        res2_in_r2->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_RES2_IN_R2],res2_in_r2);

        MIP_Knob2Widget* res2_in_n = new MIP_Knob2Widget( MIP_DRect(150,60,30,45),"N");
        res2_panel->appendChildWidget(res2_in_n);
//        res2_in_n->getKnobWidget()->setArcThickness(4);
        connectWidget(AParameters[PAR_RES2_IN_N],res2_in_n);

        // shape

        MIP_Knob2Widget* res2_nshape_knob = new MIP_Knob2Widget( MIP_DRect(10,110,50,82),"Shape");
        res2_panel->appendChildWidget(res2_nshape_knob);
        connectWidget(AParameters[PAR_RES2_SHAPE],res2_nshape_knob);

        // fb

        MIP_Knob2Widget* res2_fb_knob = new MIP_Knob2Widget( MIP_DRect(70,110,50,82),"FB");
        res2_panel->appendChildWidget(res2_fb_knob);
        connectWidget(AParameters[PAR_RES2_FB],res2_fb_knob);

        // damp

        MIP_Knob2Widget* res2_damp_knob = new MIP_Knob2Widget( MIP_DRect(130,110,50,82),"Damp");
        res2_panel->appendChildWidget(res2_damp_knob);
        connectWidget(AParameters[PAR_RES2_DAMP],res2_damp_knob);

        // rough

        MIP_Knob2Widget* res2_rough_knob = new MIP_Knob2Widget( MIP_DRect(190,110,50,82),"Rough");
        res2_panel->appendChildWidget(res2_rough_knob);
//        res2_rough_knob->getKnobWidget()->setSnap(true);
//        res2_rough_knob->getKnobWidget()->setSnapPos(0.5);
//        res2_rough_knob->getKnobWidget()->setSnapDist(0.05);
        connectWidget(AParameters[PAR_RES2_ROUGH],res2_rough_knob);

        // speed

        MIP_SliderWidget* res2_speed_slider = new MIP_SliderWidget( MIP_DRect(10,200,230,20),"speed" );
        res2_panel->appendChildWidget(res2_speed_slider);
        connectWidget(AParameters[PAR_RES2_SPEED],res2_speed_slider);

        // oct

        MIP_DragValueWidget* res2_oct_slider = new MIP_DragValueWidget( MIP_DRect(10,230,70,20),"Oct");
        res2_panel->appendChildWidget(res2_oct_slider);
//        res2_oct_slider->setSnap(true);
//        res2_oct_slider->setSnapPos(0.5);
//        res2_oct_slider->setSnapDist(0.05);
//        res2_oct_slider->setDrawParamName(false);
        res2_oct_slider->setDrawBorder(true);
        res2_oct_slider->setBorderColor(0.4);
        res2_oct_slider->setFillBackground(true);
        res2_oct_slider->setBackgroundColor(0.6);
        connectWidget(AParameters[PAR_RES2_OCT],res2_oct_slider);

        // semi

        MIP_DragValueWidget* res2_semi_slider = new MIP_DragValueWidget( MIP_DRect(90,230,70,20),"Semi");
        res2_panel->appendChildWidget(res2_semi_slider);
//        res2_semi_slider->setSnap(true);
//        res2_semi_slider->setSnapPos(0.5);
//        res2_semi_slider->setSnapDist(0.05);
//        res2_semi_slider->setDrawParamName(false);
        res2_semi_slider->setDrawBorder(true);
        res2_semi_slider->setBorderColor(0.4);
        res2_semi_slider->setFillBackground(true);
        res2_semi_slider->setBackgroundColor(0.6);
        connectWidget(AParameters[PAR_RES2_SEMI],res2_semi_slider);

        // cent

        MIP_DragValueWidget* res2_cent_slider = new MIP_DragValueWidget( MIP_DRect(170,230,70,20),"Cent");
        res2_panel->appendChildWidget(res2_cent_slider);
//        res2_cent_slider->setSnap(true);
//        res2_cent_slider->setSnapPos(0.5);
//        res2_cent_slider->setSnapDist(0.05);
//        res2_cent_slider->setDrawParamName(false);
        res2_cent_slider->setDrawBorder(true);
        res2_cent_slider->setBorderColor(0.4);
        res2_cent_slider->setFillBackground(true);
        res2_cent_slider->setBackgroundColor(0.6);
        connectWidget(AParameters[PAR_RES2_CENT],res2_cent_slider);

//
//
// right --------------------------------------------------
//
//

      // voices

      MIP_PanelWidget* voice2_panel = new MIP_PanelWidget(MIP_DRect(530,10,276,75+30) );
//      voice2_panel->setBackgroundColor(0.55);
      controls->appendChildWidget(voice2_panel);

        // text

        MIP_TextWidget* voices_text = new MIP_TextWidget(MIP_DRect(0,0,276,20),"Voices");
        voice2_panel->appendChildWidget(voices_text);
        voices_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        voices_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        voices_text->setDrawBorder(false);
        voices_text->setFillBackground(true);
        voices_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // voices

        MVoiceWidget = new sa_tyr_VoiceWidget( MIP_DRect(10,30,256,8) );
        voice2_panel->appendChildWidget(MVoiceWidget);

        MPlayingVoicesWidget = new MIP_ValueWidget( MIP_DRect(10,45,150,15), "Playing:",0 );
//        MPlayingVoicesWidget->setDigits(0);
        MPlayingVoicesWidget->setDrawBorder(false);
        MPlayingVoicesWidget->setFillBackground(false);
        voice2_panel->appendChildWidget(MPlayingVoicesWidget);

        MReleasedVoicesWidget = new MIP_ValueWidget( MIP_DRect(10,60,150,15), "Released:",0 );
//        MReleasedVoicesWidget->setDigits(0);
        MReleasedVoicesWidget->setDrawBorder(false);
        MReleasedVoicesWidget->setFillBackground(false);
        voice2_panel->appendChildWidget(MReleasedVoicesWidget);

        MTotalVoicesWidget = new MIP_ValueWidget( MIP_DRect(10,75,150,15), "Total:",0 );
//        MTotalVoicesWidget->setDigits(0);
        MTotalVoicesWidget->setDrawBorder(false);
        MTotalVoicesWidget->setFillBackground(false);
        voice2_panel->appendChildWidget(MTotalVoicesWidget);

      // ----- global -----

      MIP_PanelWidget* global_panel = new MIP_PanelWidget(MIP_DRect(530,125,276,210) );
//      global_panel->setBackgroundColor(0.57);
      controls->appendChildWidget(global_panel);

        // text

        MIP_TextWidget* global_text = new MIP_TextWidget(MIP_DRect(0,0,276,20),"Global");
        global_panel->appendChildWidget(global_text);
        global_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        global_text->setDrawBorder(true);
        global_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        global_text->setDrawBorder(false);
        global_text->setFillBackground(true);
        global_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // vol

        MIP_Knob2Widget* global_vol_knob = new MIP_Knob2Widget( MIP_DRect(10,30,50,82),"Vol");
        global_panel->appendChildWidget(global_vol_knob);
        connectWidget(AParameters[PAR_MASTER_VOL],global_vol_knob);

        // pan

        MIP_Knob2Widget* global_pan_knob = new MIP_Knob2Widget( MIP_DRect(70,30,50,82),"Pan");
        global_panel->appendChildWidget(global_pan_knob);
//        global_pan_knob->getKnobWidget()->setSnap(true);
//        global_pan_knob->getKnobWidget()->setSnapPos(0.5);
//        global_pan_knob->getKnobWidget()->setSnapDist(0.05);
//        global_pan_knob->getKnobWidget()->setBipolar(true);
        connectWidget(AParameters[PAR_MASTER_PAN],global_pan_knob);

        // osc1 out

        MIP_Knob2Widget* global_osc1_out_knob = new MIP_Knob2Widget( MIP_DRect(10,120,50,82),"O1");
        global_panel->appendChildWidget(global_osc1_out_knob);
        connectWidget(AParameters[PAR_MASTER_OSC1_OUT],global_osc1_out_knob);

        // osc2 out

        MIP_Knob2Widget* global_osc2_out_knob = new MIP_Knob2Widget( MIP_DRect(70,120,50,82),"O2");
        global_panel->appendChildWidget(global_osc2_out_knob);
        connectWidget(AParameters[PAR_MASTER_OSC2_OUT],global_osc2_out_knob);

        // res1 out

        MIP_Knob2Widget* global_res1_out_knob = new MIP_Knob2Widget( MIP_DRect(130,120,50,82),"R1");
        global_panel->appendChildWidget(global_res1_out_knob);
        connectWidget(AParameters[PAR_MASTER_RES1_OUT],global_res1_out_knob);

        // res2 out

        MIP_Knob2Widget* global_res2_out_knob = new MIP_Knob2Widget( MIP_DRect(190,120,50,82),"R2");
        global_panel->appendChildWidget(global_res2_out_knob);
        connectWidget(AParameters[PAR_MASTER_RES2_OUT],global_res2_out_knob);

        /*

        // osc1/2 mix

        MIP_SelectorWidget* osc_mix_selector = new MIP_SelectorWidget( MIP_DRect(0,180,230,20) );
        ->setTextSize(-0.7);
        global_panel->appendChildWidget(osc_mix_selector);
        osc_mix_selector->setMenu(osc_mix_type_menu);
        osc_mix_selector->setSelected(0);
        //connectWidget(AParameters[],osc_mix_selector,PAR_OSC_MIX_TYPE);

        //

        MIP_SliderWidget* osc_mix_slider = new MIP_SliderWidget(MIP_DRect(0,210,230,20),"O1:O2");
        global_panel->appendChildWidget(osc_mix_slider);
        //connectWidget(AParameters[],osc_mix_slider,PAR_OSC_MIX_AMOUNT);

        */

      // ----- flt1 -----

      MIP_PanelWidget* flt1_panel = new MIP_PanelWidget(MIP_DRect(530,345,276,145) );
      flt1_panel->setBackgroundColor(0.55);
      controls->appendChildWidget(flt1_panel);

        // text

        MIP_TextWidget* flt_text = new MIP_TextWidget(MIP_DRect(0,0,276,20),"Filter");
        flt1_panel->appendChildWidget(flt_text);
        flt_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        flt_text->setDrawBorder(true);
        flt_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        flt_text->setDrawBorder(false);
        flt_text->setFillBackground(true);
        flt_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // type

        MIP_SelectorWidget* flt1_type_selector = new MIP_SelectorWidget( MIP_DRect(10,30,256,20),"Flt Type",flt_type_menu );
        flt1_type_selector->setTextSize(-0.7);
        flt1_panel->appendChildWidget(flt1_type_selector);
//        flt1_type_selector->setText("---");
//        flt1_type_selector->setMenu(flt_type_menu);
        connectWidget(AParameters[PAR_FLT1_TYPE],flt1_type_selector);

        // freq

        MIP_Knob2Widget* flt1_freq_knob = new MIP_Knob2Widget( MIP_DRect(10,60,50,82),"Freq");
        flt1_panel->appendChildWidget(flt1_freq_knob);
        connectWidget(AParameters[PAR_FLT1_FREQ],flt1_freq_knob);

        // res

        MIP_Knob2Widget* flt1_res_knob = new MIP_Knob2Widget( MIP_DRect(70,60,50,82),"Res");
        flt1_panel->appendChildWidget(flt1_res_knob);
        connectWidget(AParameters[PAR_FLT1_RES],flt1_res_knob);

      // ----- env1

      MIP_PanelWidget* env1_panel = new MIP_PanelWidget(MIP_DRect(530,500,276,112) );
      env1_panel->setFillBackground(true);
      env1_panel->setBackgroundColor(0.55);
      controls->appendChildWidget(env1_panel);

        // text

        MIP_TextWidget* env_text = new MIP_TextWidget(MIP_DRect(0,0,276,20),"Envelope");
        env1_panel->appendChildWidget(env_text);
        env_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        env_text->setDrawBorder(true);
        env_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        env_text->setDrawBorder(false);
        env_text->setFillBackground(true);
        env_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // att

        MIP_Knob2Widget* env1_att_knob = new MIP_Knob2Widget( MIP_DRect(10,30,50,82),"Att");
        env1_panel->appendChildWidget(env1_att_knob);
        connectWidget(AParameters[PAR_ENV1_ATT],env1_att_knob);

        // dec

        MIP_Knob2Widget* env1_dec_knob = new MIP_Knob2Widget( MIP_DRect(70,30,50,82),"Dec");
        env1_panel->appendChildWidget(env1_dec_knob);
        connectWidget(AParameters[PAR_ENV1_DEC],env1_dec_knob);

        // sus

        MIP_Knob2Widget* env1_sus_knob = new MIP_Knob2Widget( MIP_DRect(130,30,50,82),"Sus");
        env1_panel->appendChildWidget(env1_sus_knob);
        connectWidget(AParameters[PAR_ENV1_SUS],env1_sus_knob);

        // rel

        MIP_Knob2Widget* env1_rel_knob = new MIP_Knob2Widget( MIP_DRect(190,30,50,82),"Rel");
        env1_panel->appendChildWidget(env1_rel_knob);
        connectWidget(AParameters[PAR_ENV1_REL],env1_rel_knob);

    // menus

    MEditorPanel->appendChildWidget(osc_type_menu);
    MEditorPanel->appendChildWidget(res_type_menu);
    MEditorPanel->appendChildWidget(flt_type_menu);
    MEditorPanel->appendChildWidget(wave_shaper_type_menu);
    MEditorPanel->appendChildWidget(phase_shaper_type_menu);
    //MEditorPanel->appendChildWidget(osc_mix_type_menu);

    //

    MEditorPanel->Layout.aspectRatio = EDITOR_WIDTH / EDITOR_HEIGHT;
    MEditorPanel->setRectMode(MIP_WIDGET_RECT_MODE_INITIAL_RATIO);

    window->appendChildWidget(MEditorPanel);


  }

  //----------

  /*
    redraw the voice info..
  */

  //void on_timerCallback(void) final {
  void on_gui_timer() final {
    MIP_PRINT;
    //flushGuiParams();
    MVoiceWidget->redraw();
    MPlayingVoicesWidget->redraw();
    MReleasedVoicesWidget->redraw();
    MTotalVoicesWidget->redraw();
  }

};

//----------------------------------------------------------------------
#endif
