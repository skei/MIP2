#ifndef sa_ts1_editor_included
#define sa_ts1_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_editor.h"

#include "sa_ts1_widgets.h"

//----------------------------------------------------------------------

#define EDITOR_WIDTH          816
#define EDITOR_HEIGHT         683


class sa_ts1_Editor
: public MIP_Editor {

//------------------------------
public:
//------------------------------

  sa_ts1_VoiceWidget* MVoiceWidget          = nullptr;
  MIP_ValueWidget*    MPlayingVoicesWidget  = nullptr;
  MIP_ValueWidget*    MReleasedVoicesWidget = nullptr;
  MIP_ValueWidget*    MTotalVoicesWidget    = nullptr;

//------------------------------
public:
//------------------------------

  sa_ts1_Editor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded, const clap_plugin_descriptor_t* descriptor)
  : MIP_Editor(AListener,APlugin,AWidth,AHeight,AEmbedded) {
    MIP_Window* window = getWindow();

// sa_ts1_module_menus

    // osc menu

    MIP_MenuWidget* osc_type_menu = new MIP_MenuWidget(MIP_FRect());
    osc_type_menu->appendMenuItem("Morph");
    osc_type_menu->appendMenuItem("DSF");
    osc_type_menu->appendMenuItem("Wavetable");
    osc_type_menu->setItemSize(100,20);
    osc_type_menu->setItemLayout(1,3);
    osc_type_menu->setMenuMirror(false,false);
    //MEditorPanel->appendWidget(osc_type_menu);

    // res menu

    MIP_MenuWidget* res_type_menu = new MIP_MenuWidget(MIP_FRect());
    res_type_menu->appendMenuItem("Pluck");
    res_type_menu->appendMenuItem("Repeated Plucks");
    res_type_menu->appendMenuItem("(not implemented)");
    res_type_menu->setItemSize(120,20);
    res_type_menu->setItemLayout(1,3);
    res_type_menu->setMenuMirror(false,false);
    //MEditorPanel->appendWidget(res_type_menu);

    // flt menu

    MIP_MenuWidget* flt_type_menu = new MIP_MenuWidget(MIP_FRect());
    flt_type_menu->appendMenuItem("Off");
    flt_type_menu->appendMenuItem("Lowpass");
    flt_type_menu->appendMenuItem("Highpass");
    flt_type_menu->appendMenuItem("Bandpass");
    flt_type_menu->appendMenuItem("Notch");
    flt_type_menu->setItemSize(100,20);
    flt_type_menu->setItemLayout(1,5);
    flt_type_menu->setMenuMirror(false,false);
    //MEditorPanel->appendWidget(flt_type_menu);

    // ws

    MIP_MenuWidget* wave_shaper_type_menu = new MIP_MenuWidget(MIP_FRect());
    wave_shaper_type_menu->appendMenuItem("Off");
    wave_shaper_type_menu->appendMenuItem("Curve");
    wave_shaper_type_menu->appendMenuItem("AmplMod");
    wave_shaper_type_menu->appendMenuItem("RingMod");
    wave_shaper_type_menu->appendMenuItem("Replace");
    wave_shaper_type_menu->appendMenuItem("Negate");
    wave_shaper_type_menu->appendMenuItem("Sign");
    wave_shaper_type_menu->appendMenuItem("Max");
    wave_shaper_type_menu->appendMenuItem("Fold");
    wave_shaper_type_menu->setItemSize(100,20);
    wave_shaper_type_menu->setItemLayout(1,9);
    wave_shaper_type_menu->setMenuMirror(false,false);
    //MEditorPanel->appendWidget(wave_shaper_type_menu);

    // ps

    MIP_MenuWidget* phase_shaper_type_menu = new MIP_MenuWidget(MIP_FRect());
    phase_shaper_type_menu->appendMenuItem("Off");
    phase_shaper_type_menu->appendMenuItem("Curve");
    phase_shaper_type_menu->appendMenuItem("Sync");
    phase_shaper_type_menu->appendMenuItem("Sync / Clamp");
    phase_shaper_type_menu->appendMenuItem("PhaseMod");
    phase_shaper_type_menu->appendMenuItem("FreqMod");
    phase_shaper_type_menu->appendMenuItem("Flip");
    phase_shaper_type_menu->setItemSize(100,20);
    phase_shaper_type_menu->setItemLayout(1,7);
    phase_shaper_type_menu->setMenuMirror(false,false);
    //MEditorPanel->appendWidget(phase_shaper_type_menu);

    // panel

    MIP_PanelWidget* MEditorPanel = new MIP_PanelWidget(MIP_FRect());
    MEditorPanel->setDrawBorder(false);
    MEditorPanel->setFillBackground(false);
    MEditorPanel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

    // header

    MIP_SAHeaderWidget* sa_header = new MIP_SAHeaderWidget(60,window);
    sa_header->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    sa_header->setPluginName(descriptor->name);
    sa_header->setPluginVersion(descriptor->version);
    MEditorPanel->appendWidget(sa_header);

    // controls

    MIP_PanelWidget* controls = new MIP_PanelWidget(MIP_FRect());
    controls->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    controls->setDrawBorder(false);
    controls->setFillBackground(true);
    controls->setBackgroundColor(0.6);
    controls->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    MEditorPanel->appendWidget(controls);

//
//
// left --------------------------------------------------
//
//

// ----- osc1

      MIP_PanelWidget* osc1_panel = new MIP_PanelWidget(MIP_FRect(10,10,250,300) );
      controls->appendWidget(osc1_panel);
      osc1_panel->setFillBackground(true);
      osc1_panel->setBackgroundColor(0.55);

        // text

        MIP_TextWidget* osc1_text = new MIP_TextWidget(MIP_FRect(0,0,250,20),"OSC1");
        osc1_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        osc1_text->setDrawBorder(true);
        osc1_panel->appendWidget(osc1_text);
        osc1_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        osc1_text->setDrawBorder(false);
        osc1_text->setFillBackground(true);
        osc1_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // type

        MIP_SelectorWidget* osc1_type_selector = new MIP_SelectorWidget( MIP_FRect(10,30,230,20) );
        osc1_panel->appendWidget(osc1_type_selector);
        osc1_type_selector->setMenu(osc_type_menu);
        osc1_type_selector->setSelected(1);
        connect(osc1_type_selector,PAR_OSC1_TYPE);

        // shape

        MIP_Knob2Widget* osc1_shape_knob = new MIP_Knob2Widget( MIP_FRect(10,60,50,82),"Shape");
        osc1_panel->appendWidget(osc1_shape_knob);
        connect(osc1_shape_knob,PAR_OSC1_SHAPE);

        // width

        MIP_Knob2Widget* osc1_width_knob = new MIP_Knob2Widget( MIP_FRect(70,60,50,82),"Width");
        osc1_panel->appendWidget(osc1_width_knob);
        osc1_width_knob->getKnobWidget()->setSnap(true);
        osc1_width_knob->getKnobWidget()->setSnapPos(0.5);
        osc1_width_knob->getKnobWidget()->setSnapDist(0.05);
        osc1_width_knob->getKnobWidget()->setBipolar(true);
        connect(osc1_width_knob,PAR_OSC1_WIDTH);

        // oct

        MIP_DragValueWidget* osc1_oct_slider = new MIP_DragValueWidget( MIP_FRect(10,150,70,20),"Oct");
        osc1_panel->appendWidget(osc1_oct_slider);
        //osc1_oct_slider->setQuantize(true);
        osc1_oct_slider->setSnap(true);
        osc1_oct_slider->setSnapPos(0.5);
        osc1_oct_slider->setSnapDist(0.05);
        osc1_oct_slider->setDrawParamName(false);
        osc1_oct_slider->setDrawBorder(true);
        osc1_oct_slider->setBorderColor(0.4);
        osc1_oct_slider->setFillBackground(true);
        osc1_oct_slider->setBackgroundColor(0.6);
        connect(osc1_oct_slider,PAR_OSC1_OCT);

        // semi

        MIP_DragValueWidget* osc1_semi_slider = new MIP_DragValueWidget( MIP_FRect(90,150,70,20),"Semi");
        osc1_panel->appendWidget(osc1_semi_slider);
        osc1_semi_slider->setSnap(true);
        osc1_semi_slider->setSnapPos(0.5);
        osc1_semi_slider->setSnapDist(0.05);
        osc1_semi_slider->setDrawParamName(false);
        osc1_semi_slider->setDrawBorder(true);
        osc1_semi_slider->setBorderColor(0.4);
        osc1_semi_slider->setFillBackground(true);
        osc1_semi_slider->setBackgroundColor(0.6);
        connect(osc1_semi_slider,PAR_OSC1_SEMI);

        // cent

        MIP_DragValueWidget* osc1_cent_slider = new MIP_DragValueWidget( MIP_FRect(170,150,70,20),"Cent");
        osc1_panel->appendWidget(osc1_cent_slider);
        osc1_cent_slider->setSnap(true);
        osc1_cent_slider->setSnapPos(0.5);
        osc1_cent_slider->setSnapDist(0.05);
        osc1_cent_slider->setDrawParamName(false);
        osc1_cent_slider->setDrawBorder(true);
        osc1_cent_slider->setBorderColor(0.4);
        osc1_cent_slider->setFillBackground(true);
        osc1_cent_slider->setBackgroundColor(0.6);
        connect(osc1_cent_slider,PAR_OSC1_CENT);

        // inputs

        MIP_Knob2Widget* osc1_in_o1 = new MIP_Knob2Widget( MIP_FRect(10,180,30,45),"O1",true,false);
        osc1_panel->appendWidget(osc1_in_o1);
        osc1_in_o1->getKnobWidget()->setArcThickness(4);
        connect(osc1_in_o1,PAR_OSC1_IN_O1);

        MIP_Knob2Widget* osc1_in_o2 = new MIP_Knob2Widget( MIP_FRect(45,180,30,45),"O2",true,false);
        osc1_panel->appendWidget(osc1_in_o2);
        osc1_in_o2->getKnobWidget()->setArcThickness(4);
        connect(osc1_in_o2,PAR_OSC1_IN_O2);

        MIP_Knob2Widget* osc1_in_r1 = new MIP_Knob2Widget( MIP_FRect(80,180,30,45),"R1",true,false);
        osc1_panel->appendWidget(osc1_in_r1);
        osc1_in_r1->getKnobWidget()->setArcThickness(4);
        connect(osc1_in_r1,PAR_OSC1_IN_R1);

        MIP_Knob2Widget* osc1_in_r2 = new MIP_Knob2Widget( MIP_FRect(115,180,30,45),"R2",true,false);
        osc1_panel->appendWidget(osc1_in_r2);
        osc1_in_r2->getKnobWidget()->setArcThickness(4);
        connect(osc1_in_r2,PAR_OSC1_IN_R2);

        MIP_Knob2Widget* osc1_in_n = new MIP_Knob2Widget( MIP_FRect(150,180,30,45),"N",true,false);
        osc1_panel->appendWidget(osc1_in_n);
        osc1_in_n->getKnobWidget()->setArcThickness(4);
        connect(osc1_in_n,PAR_OSC1_IN_N);

        // ps

        MIP_SliderWidget* osc1_phase_shaper_slider = new MIP_SliderWidget(MIP_FRect(10,240,110,20),"Phase");
        osc1_panel->appendWidget(osc1_phase_shaper_slider);
        connect(osc1_phase_shaper_slider,PAR_OSC1_PM_AMOUNT);

        MIP_SelectorWidget* osc1_phase_shaper_selector = new MIP_SelectorWidget( MIP_FRect(130,240,110,20) );
        osc1_panel->appendWidget(osc1_phase_shaper_selector);
        osc1_phase_shaper_selector->setMenu(phase_shaper_type_menu);
        osc1_phase_shaper_selector->setSelected(0);
        connect(osc1_phase_shaper_selector,PAR_OSC1_PM_TYPE);

        // ws

        MIP_SliderWidget* osc1_wave_shaper_slider = new MIP_SliderWidget(MIP_FRect(10,270,110,20),"Wave");
        osc1_panel->appendWidget(osc1_wave_shaper_slider);
        connect(osc1_wave_shaper_slider,PAR_OSC1_WM_AMOUNT);

        MIP_SelectorWidget* osc1_wave_shaper_selector = new MIP_SelectorWidget( MIP_FRect(130,270,110,20) );
        osc1_panel->appendWidget(osc1_wave_shaper_selector);
        osc1_wave_shaper_selector->setMenu(wave_shaper_type_menu);
        osc1_wave_shaper_selector->setSelected(0);
        connect(osc1_wave_shaper_selector,PAR_OSC1_WM_TYPE);

// ----- res1 -----

      MIP_PanelWidget* res1_panel = new MIP_PanelWidget(MIP_FRect(10,320,250,260) );
      res1_panel->setBackgroundColor(0.55);
      controls->appendWidget(res1_panel);

        // text

        MIP_TextWidget* res1_text = new MIP_TextWidget(MIP_FRect(0,0,250,20),"RES1");
        res1_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        res1_text->setDrawBorder(true);
        res1_panel->appendWidget(res1_text);
        res1_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        res1_text->setDrawBorder(false);
        res1_text->setFillBackground(true);
        res1_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // type

        MIP_SelectorWidget* res1_type_selector = new MIP_SelectorWidget( MIP_FRect(10,30,230,20) );
        res1_panel->appendWidget(res1_type_selector);
        res1_type_selector->setText("");
        res1_type_selector->setMenu(res_type_menu);
        connect(res1_type_selector,PAR_RES1_TYPE);

        // inputs

        MIP_Knob2Widget* res1_in_o1 = new MIP_Knob2Widget( MIP_FRect(10,60,30,45),"O1",true,false);
        res1_panel->appendWidget(res1_in_o1);
        res1_in_o1->getKnobWidget()->setArcThickness(4);
        connect(res1_in_o1,PAR_RES1_IN_O1);

        MIP_Knob2Widget* res1_in_o2 = new MIP_Knob2Widget( MIP_FRect(45,60,30,45),"O2",true,false);
        res1_panel->appendWidget(res1_in_o2);
        res1_in_o2->getKnobWidget()->setArcThickness(4);
        connect(res1_in_o2,PAR_RES1_IN_O2);

        MIP_Knob2Widget* res1_in_r1 = new MIP_Knob2Widget( MIP_FRect(80,60,30,45),"R1",true,false);
        res1_panel->appendWidget(res1_in_r1);
        res1_in_r1->getKnobWidget()->setArcThickness(4);
        connect(res1_in_r1,PAR_RES1_IN_R1);

        MIP_Knob2Widget* res1_in_r2 = new MIP_Knob2Widget( MIP_FRect(115,60,30,45),"R2",true,false);
        res1_panel->appendWidget(res1_in_r2);
        res1_in_r2->getKnobWidget()->setArcThickness(4);
        connect(res1_in_r2,PAR_RES1_IN_R2);

        MIP_Knob2Widget* res1_in_n = new MIP_Knob2Widget( MIP_FRect(150,60,30,45),"N",true,false);
        res1_panel->appendWidget(res1_in_n);
        res1_in_n->getKnobWidget()->setArcThickness(4);
        connect(res1_in_n,PAR_RES1_IN_N);

        // shape

        MIP_Knob2Widget* res1_nshape_knob = new MIP_Knob2Widget( MIP_FRect(10,110,50,82),"Shape");
        res1_panel->appendWidget(res1_nshape_knob);
        connect(res1_nshape_knob,PAR_RES1_SHAPE);

        // fb

        MIP_Knob2Widget* res1_fb_knob = new MIP_Knob2Widget( MIP_FRect(70,110,50,82),"FB");
        res1_panel->appendWidget(res1_fb_knob);
        connect(res1_fb_knob,PAR_RES1_FB);

        // damp

        MIP_Knob2Widget* res1_damp_knob = new MIP_Knob2Widget( MIP_FRect(130,110,50,82),"Damp");
        res1_panel->appendWidget(res1_damp_knob);
        connect(res1_damp_knob,PAR_RES1_DAMP);

        // rough

        MIP_Knob2Widget* res1_rough_knob = new MIP_Knob2Widget( MIP_FRect(190,110,50,82),"Rough");
        res1_panel->appendWidget(res1_rough_knob);
        res1_rough_knob->getKnobWidget()->setSnap(true);
        res1_rough_knob->getKnobWidget()->setSnapPos(0.5);
        res1_rough_knob->getKnobWidget()->setSnapDist(0.05);
        connect(res1_rough_knob,PAR_RES1_ROUGH);

        // speed

        MIP_SliderWidget* res1_speed_slider = new MIP_SliderWidget( MIP_FRect(10,200,230,20) );
        res1_panel->appendWidget(res1_speed_slider);
        connect(res1_speed_slider,PAR_RES1_SPEED);

        // oct

        MIP_DragValueWidget* res1_oct_slider = new MIP_DragValueWidget( MIP_FRect(10,230,70,20),"Oct");
        res1_panel->appendWidget(res1_oct_slider);
        res1_oct_slider->setSnap(true);
        res1_oct_slider->setSnapPos(0.5);
        res1_oct_slider->setSnapDist(0.05);
        res1_oct_slider->setDrawParamName(false);
        res1_oct_slider->setDrawBorder(true);
        res1_oct_slider->setBorderColor(0.4);
        res1_oct_slider->setFillBackground(true);
        res1_oct_slider->setBackgroundColor(0.6);
        connect(res1_oct_slider,PAR_RES1_OCT);

        // semi

        MIP_DragValueWidget* res1_semi_slider = new MIP_DragValueWidget( MIP_FRect(90,230,70,20),"Semi");
        res1_panel->appendWidget(res1_semi_slider);
        res1_semi_slider->setSnap(true);
        res1_semi_slider->setSnapPos(0.5);
        res1_semi_slider->setSnapDist(0.05);
        res1_semi_slider->setDrawParamName(false);
        res1_semi_slider->setDrawBorder(true);
        res1_semi_slider->setBorderColor(0.4);
        res1_semi_slider->setFillBackground(true);
        res1_semi_slider->setBackgroundColor(0.6);
        connect(res1_semi_slider,PAR_RES1_SEMI);

        // cent

        MIP_DragValueWidget* res1_cent_slider = new MIP_DragValueWidget( MIP_FRect(170,230,70,20),"Cent");
        res1_panel->appendWidget(res1_cent_slider);
        res1_cent_slider->setSnap(true);
        res1_cent_slider->setSnapPos(0.5);
        res1_cent_slider->setSnapDist(0.05);
        res1_cent_slider->setDrawParamName(false);
        res1_cent_slider->setDrawBorder(true);
        res1_cent_slider->setBorderColor(0.4);
        res1_cent_slider->setFillBackground(true);
        res1_cent_slider->setBackgroundColor(0.6);
        connect(res1_cent_slider,PAR_RES1_CENT);


//
//
// mid --------------------------------------------------
//
//

// ----- osc2

      MIP_PanelWidget* osc2_panel = new MIP_PanelWidget(MIP_FRect(10+260,10,250,300) );
      controls->appendWidget(osc2_panel);
      osc2_panel->setFillBackground(true);
      osc2_panel->setBackgroundColor(0.55);

        // text

        MIP_TextWidget* osc2_text = new MIP_TextWidget(MIP_FRect(0,0,250,20),"OSC2");
        osc2_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        osc2_text->setDrawBorder(true);
        osc2_panel->appendWidget(osc2_text);
        osc2_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        osc2_text->setDrawBorder(false);
        osc2_text->setFillBackground(true);
        osc2_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // type

        MIP_SelectorWidget* osc2_type_selector = new MIP_SelectorWidget( MIP_FRect(10,30,230,20) );
        osc2_panel->appendWidget(osc2_type_selector);
        osc2_type_selector->setMenu(osc_type_menu);
        osc2_type_selector->setSelected(1);
        connect(osc2_type_selector,PAR_OSC2_TYPE);

        // shape

        MIP_Knob2Widget* osc2_shape_knob = new MIP_Knob2Widget( MIP_FRect(10,60,50,82),"Shape");
        osc2_panel->appendWidget(osc2_shape_knob);
        connect(osc2_shape_knob,PAR_OSC2_SHAPE);

        // width

        MIP_Knob2Widget* osc2_width_knob = new MIP_Knob2Widget( MIP_FRect(70,60,50,82),"Width");
        osc2_panel->appendWidget(osc2_width_knob);
        osc2_width_knob->getKnobWidget()->setSnap(true);
        osc2_width_knob->getKnobWidget()->setSnapPos(0.5);
        osc2_width_knob->getKnobWidget()->setSnapDist(0.05);
        osc2_width_knob->getKnobWidget()->setBipolar(true);
        connect(osc2_width_knob,PAR_OSC2_WIDTH);

        // oct

        MIP_DragValueWidget* osc2_oct_slider = new MIP_DragValueWidget( MIP_FRect(10,150,70,20),"Oct");
        osc2_panel->appendWidget(osc2_oct_slider);
        osc2_oct_slider->setSnap(true);
        osc2_oct_slider->setSnapPos(0.5);
        osc2_oct_slider->setSnapDist(0.05);
        osc2_oct_slider->setDrawParamName(false);
        osc2_oct_slider->setDrawBorder(true);
        osc2_oct_slider->setBorderColor(0.4);
        osc2_oct_slider->setFillBackground(true);
        osc2_oct_slider->setBackgroundColor(0.6);
        connect(osc2_oct_slider,PAR_OSC2_OCT);

        // semi

        MIP_DragValueWidget* osc2_semi_slider = new MIP_DragValueWidget( MIP_FRect(90,150,70,20),"Semi");
        osc2_panel->appendWidget(osc2_semi_slider);
        osc2_semi_slider->setSnap(true);
        osc2_semi_slider->setSnapPos(0.5);
        osc2_semi_slider->setSnapDist(0.05);
        osc2_semi_slider->setDrawParamName(false);
        osc2_semi_slider->setDrawBorder(true);
        osc2_semi_slider->setBorderColor(0.4);
        osc2_semi_slider->setFillBackground(true);
        osc2_semi_slider->setBackgroundColor(0.6);
        connect(osc2_semi_slider,PAR_OSC2_SEMI);

        // cent

        MIP_DragValueWidget* osc2_cent_slider = new MIP_DragValueWidget( MIP_FRect(170,150,70,20),"Cent");
        osc2_panel->appendWidget(osc2_cent_slider);
        osc2_cent_slider->setSnap(true);
        osc2_cent_slider->setSnapPos(0.5);
        osc2_cent_slider->setSnapDist(0.05);
        osc2_cent_slider->setDrawParamName(false);
        osc2_cent_slider->setDrawBorder(true);
        osc2_cent_slider->setBorderColor(0.4);
        osc2_cent_slider->setFillBackground(true);
        osc2_cent_slider->setBackgroundColor(0.6);
        connect(osc2_cent_slider,PAR_OSC2_CENT);

        // inputs

        MIP_Knob2Widget* osc2_in_o1 = new MIP_Knob2Widget( MIP_FRect(10,180,30,45),"O1",true,false);
        osc2_panel->appendWidget(osc2_in_o1);
        osc2_in_o1->getKnobWidget()->setArcThickness(4);
        connect(osc2_in_o1,PAR_OSC2_IN_O1);

        MIP_Knob2Widget* osc2_in_o2 = new MIP_Knob2Widget( MIP_FRect(45,180,30,45),"O2",true,false);
        osc2_panel->appendWidget(osc2_in_o2);
        osc2_in_o1->getKnobWidget()->setArcThickness(4);
        connect(osc2_in_o2,PAR_OSC2_IN_O2);

        MIP_Knob2Widget* osc2_in_r1 = new MIP_Knob2Widget( MIP_FRect(80,180,30,45),"R1",true,false);
        osc2_panel->appendWidget(osc2_in_r1);
        osc2_in_o2->getKnobWidget()->setArcThickness(4);
        connect(osc2_in_r1,PAR_OSC2_IN_R1);

        MIP_Knob2Widget* osc2_in_r2 = new MIP_Knob2Widget( MIP_FRect(115,180,30,45),"R2",true,false);
        osc2_panel->appendWidget(osc2_in_r2);
        osc2_in_r1->getKnobWidget()->setArcThickness(4);
        connect(osc2_in_r2,PAR_OSC2_IN_R2);

        MIP_Knob2Widget* osc2_in_n = new MIP_Knob2Widget( MIP_FRect(150,180,30,45),"N",true,false);
        osc2_panel->appendWidget(osc2_in_n);
        osc2_in_r2->getKnobWidget()->setArcThickness(4);
        connect(osc2_in_n,PAR_OSC2_IN_N);

        // ps

        MIP_SliderWidget* osc2_phase_shaper_slider = new MIP_SliderWidget(MIP_FRect(10,240,110,20),"Phase");
        osc2_panel->appendWidget(osc2_phase_shaper_slider);
        osc2_in_n->getKnobWidget()->setArcThickness(4);
        connect(osc2_phase_shaper_slider,PAR_OSC2_PM_AMOUNT);

        MIP_SelectorWidget* osc2_phase_shaper_selector = new MIP_SelectorWidget( MIP_FRect(130,240,110,20) );
        osc2_panel->appendWidget(osc2_phase_shaper_selector);
        osc2_phase_shaper_selector->setMenu(phase_shaper_type_menu);
        osc2_phase_shaper_selector->setSelected(0);
        connect(osc2_phase_shaper_selector,PAR_OSC2_PM_TYPE);

        // ws

        MIP_SliderWidget* osc2_wave_shaper_slider = new MIP_SliderWidget(MIP_FRect(10,270,110,20),"Wave");
        osc2_panel->appendWidget(osc2_wave_shaper_slider);
        connect(osc2_wave_shaper_slider,PAR_OSC2_WM_AMOUNT);

        MIP_SelectorWidget* osc2_wave_shaper_selector = new MIP_SelectorWidget( MIP_FRect(130,270,110,20) );
        osc2_panel->appendWidget(osc2_wave_shaper_selector);
        osc2_wave_shaper_selector->setMenu(wave_shaper_type_menu);
        osc2_wave_shaper_selector->setSelected(0);
        connect(osc2_wave_shaper_selector,PAR_OSC2_WM_TYPE);

// ----- res2 -----

      MIP_PanelWidget* res2_panel = new MIP_PanelWidget(MIP_FRect(10+260,320,250,260) );
      res2_panel->setBackgroundColor(0.55);
      controls->appendWidget(res2_panel);

        // text

        MIP_TextWidget* res2_text = new MIP_TextWidget(MIP_FRect(0,0,250,20),"RES2");
        res2_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        res2_text->setDrawBorder(true);
        res2_panel->appendWidget(res2_text);
        res2_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        res2_text->setDrawBorder(false);
        res2_text->setFillBackground(true);
        res2_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // type

        MIP_SelectorWidget* res2_type_selector = new MIP_SelectorWidget( MIP_FRect(10,30,230,20) );
        res2_panel->appendWidget(res2_type_selector);
        res2_type_selector->setText("---");
        res2_type_selector->setMenu(res_type_menu);
        connect(res2_type_selector,PAR_RES2_TYPE);

        // inputs

        MIP_Knob2Widget* res2_in_o1 = new MIP_Knob2Widget( MIP_FRect(10,60,30,45),"O1",true,false);
        res2_panel->appendWidget(res2_in_o1);
        res2_in_o1->getKnobWidget()->setArcThickness(4);
        connect(res2_in_o1,PAR_RES2_IN_O1);

        MIP_Knob2Widget* res2_in_o2 = new MIP_Knob2Widget( MIP_FRect(45,60,30,45),"O2",true,false);
        res2_panel->appendWidget(res2_in_o2);
        res2_in_o2->getKnobWidget()->setArcThickness(4);
        connect(res2_in_o2,PAR_RES2_IN_O2);

        MIP_Knob2Widget* res2_in_r1 = new MIP_Knob2Widget( MIP_FRect(80,60,30,45),"R1",true,false);
        res2_panel->appendWidget(res2_in_r1);
        res2_in_r1->getKnobWidget()->setArcThickness(4);
        connect(res2_in_r1,PAR_RES2_IN_R1);

        MIP_Knob2Widget* res2_in_r2 = new MIP_Knob2Widget( MIP_FRect(115,60,30,45),"R2",true,false);
        res2_panel->appendWidget(res2_in_r2);
        res2_in_r2->getKnobWidget()->setArcThickness(4);
        connect(res2_in_r2,PAR_RES2_IN_R2);

        MIP_Knob2Widget* res2_in_n = new MIP_Knob2Widget( MIP_FRect(150,60,30,45),"N",true,false);
        res2_panel->appendWidget(res2_in_n);
        res2_in_n->getKnobWidget()->setArcThickness(4);
        connect(res2_in_n,PAR_RES2_IN_N);

        // shape

        MIP_Knob2Widget* res2_nshape_knob = new MIP_Knob2Widget( MIP_FRect(10,110,50,82),"Shape");
        res2_panel->appendWidget(res2_nshape_knob);
        connect(res2_nshape_knob,PAR_RES2_SHAPE);

        // fb

        MIP_Knob2Widget* res2_fb_knob = new MIP_Knob2Widget( MIP_FRect(70,110,50,82),"FB");
        res2_panel->appendWidget(res2_fb_knob);
        connect(res2_fb_knob,PAR_RES2_FB);

        // damp

        MIP_Knob2Widget* res2_damp_knob = new MIP_Knob2Widget( MIP_FRect(130,110,50,82),"Damp");
        res2_panel->appendWidget(res2_damp_knob);
        connect(res2_damp_knob,PAR_RES2_DAMP);

        // rough

        MIP_Knob2Widget* res2_rough_knob = new MIP_Knob2Widget( MIP_FRect(190,110,50,82),"Rough");
        res2_panel->appendWidget(res2_rough_knob);
        res2_rough_knob->getKnobWidget()->setSnap(true);
        res2_rough_knob->getKnobWidget()->setSnapPos(0.5);
        res2_rough_knob->getKnobWidget()->setSnapDist(0.05);
        connect(res2_rough_knob,PAR_RES2_ROUGH);

        // speed

        MIP_SliderWidget* res2_speed_slider = new MIP_SliderWidget( MIP_FRect(10,200,230,20) );
        res2_panel->appendWidget(res2_speed_slider);
        connect(res2_speed_slider,PAR_RES2_SPEED);

        // oct

        MIP_DragValueWidget* res2_oct_slider = new MIP_DragValueWidget( MIP_FRect(10,230,70,20),"Oct");
        res2_panel->appendWidget(res2_oct_slider);
        res2_oct_slider->setSnap(true);
        res2_oct_slider->setSnapPos(0.5);
        res2_oct_slider->setSnapDist(0.05);
        res2_oct_slider->setDrawParamName(false);
        res2_oct_slider->setDrawBorder(true);
        res2_oct_slider->setBorderColor(0.4);
        res2_oct_slider->setFillBackground(true);
        res2_oct_slider->setBackgroundColor(0.6);
        connect(res2_oct_slider,PAR_RES2_OCT);

        // semi

        MIP_DragValueWidget* res2_semi_slider = new MIP_DragValueWidget( MIP_FRect(90,230,70,20),"Semi");
        res2_panel->appendWidget(res2_semi_slider);
        res2_semi_slider->setSnap(true);
        res2_semi_slider->setSnapPos(0.5);
        res2_semi_slider->setSnapDist(0.05);
        res2_semi_slider->setDrawParamName(false);
        res2_semi_slider->setDrawBorder(true);
        res2_semi_slider->setBorderColor(0.4);
        res2_semi_slider->setFillBackground(true);
        res2_semi_slider->setBackgroundColor(0.6);
        connect(res2_semi_slider,PAR_RES2_SEMI);

        // cent

        MIP_DragValueWidget* res2_cent_slider = new MIP_DragValueWidget( MIP_FRect(170,230,70,20),"Cent");
        res2_panel->appendWidget(res2_cent_slider);
        res2_cent_slider->setSnap(true);
        res2_cent_slider->setSnapPos(0.5);
        res2_cent_slider->setSnapDist(0.05);
        res2_cent_slider->setDrawParamName(false);
        res2_cent_slider->setDrawBorder(true);
        res2_cent_slider->setBorderColor(0.4);
        res2_cent_slider->setFillBackground(true);
        res2_cent_slider->setBackgroundColor(0.6);
        connect(res2_cent_slider,PAR_RES2_CENT);

//
//
// right --------------------------------------------------
//
//

      // voices

      MIP_PanelWidget* voice2_panel = new MIP_PanelWidget(MIP_FRect(530,10,276,75+30) );
      voice2_panel->setBackgroundColor(0.55);
      controls->appendWidget(voice2_panel);

        // text

        MIP_TextWidget* voices_text = new MIP_TextWidget(MIP_FRect(0,0,276,20),"Voices");
        voice2_panel->appendWidget(voices_text);
        voices_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        voices_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        voices_text->setDrawBorder(false);
        voices_text->setFillBackground(true);
        voices_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // voices

        MVoiceWidget = new sa_ts1_VoiceWidget( MIP_FRect(10,30,256,8) );
        voice2_panel->appendWidget(MVoiceWidget);

        MPlayingVoicesWidget = new MIP_ValueWidget( MIP_FRect(10,45,150,15), "Playing voices:" );
        MPlayingVoicesWidget->setDigits(0);
        voice2_panel->appendWidget(MPlayingVoicesWidget);

        MReleasedVoicesWidget = new MIP_ValueWidget( MIP_FRect(10,60,150,15), "Released voices:" );
        MReleasedVoicesWidget->setDigits(0);
        voice2_panel->appendWidget(MReleasedVoicesWidget);

        MTotalVoicesWidget = new MIP_ValueWidget( MIP_FRect(10,75,150,15), "Total:" );
        MTotalVoicesWidget->setDigits(0);
        voice2_panel->appendWidget(MTotalVoicesWidget);

      // ----- global -----

      MIP_PanelWidget* global_panel = new MIP_PanelWidget(MIP_FRect(530,125,276,210) );
      global_panel->setBackgroundColor(0.57);
      controls->appendWidget(global_panel);

        // text

        MIP_TextWidget* global_text = new MIP_TextWidget(MIP_FRect(0,0,276,20),"Global");
        global_panel->appendWidget(global_text);
        global_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        global_text->setDrawBorder(true);
        global_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        global_text->setDrawBorder(false);
        global_text->setFillBackground(true);
        global_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // vol

        MIP_Knob2Widget* global_vol_knob = new MIP_Knob2Widget( MIP_FRect(10,30,50,82),"Vol");
        global_panel->appendWidget(global_vol_knob);
        connect(global_vol_knob,PAR_MASTER_VOL);

        // pan

        MIP_Knob2Widget* global_pan_knob = new MIP_Knob2Widget( MIP_FRect(70,30,50,82),"Pan");
        global_pan_knob->getKnobWidget()->setSnap(true);
        global_pan_knob->getKnobWidget()->setSnapPos(0.5);
        global_pan_knob->getKnobWidget()->setSnapDist(0.05);
        global_pan_knob->getKnobWidget()->setBipolar(true);
        global_panel->appendWidget(global_pan_knob);
        connect(global_pan_knob,PAR_MASTER_PAN);

        // osc1 out

        MIP_Knob2Widget* global_osc1_out_knob = new MIP_Knob2Widget( MIP_FRect(10,120,50,82),"O1");
        global_panel->appendWidget(global_osc1_out_knob);
        connect(global_osc1_out_knob,PAR_MASTER_OSC1_OUT);

        // osc2 out

        MIP_Knob2Widget* global_osc2_out_knob = new MIP_Knob2Widget( MIP_FRect(70,120,50,82),"O2");
        global_panel->appendWidget(global_osc2_out_knob);
        connect(global_osc2_out_knob,PAR_MASTER_OSC2_OUT);

        // res1 out

        MIP_Knob2Widget* global_res1_out_knob = new MIP_Knob2Widget( MIP_FRect(130,120,50,82),"R1");
        global_panel->appendWidget(global_res1_out_knob);
        connect(global_res1_out_knob,PAR_MASTER_RES1_OUT);

        // res2 out

        MIP_Knob2Widget* global_res2_out_knob = new MIP_Knob2Widget( MIP_FRect(190,120,50,82),"R2");
        global_panel->appendWidget(global_res2_out_knob);
        connect(global_res2_out_knob,PAR_MASTER_RES2_OUT);

//        // osc1/2 mix
//
//        MIP_SelectorWidget* osc_mix_selector = new MIP_SelectorWidget( MIP_FRect(0,180,230,20) );
//        global_panel->appendWidget(osc_mix_selector);
//        osc_mix_selector->setMenu(osc_mix_type_menu);
//        osc_mix_selector->setSelected(0);
//        //connect(osc_mix_selector,PAR_OSC_MIX_TYPE);
//
//        //
//
//        MIP_SliderWidget* osc_mix_slider = new MIP_SliderWidget(MIP_FRect(0,210,230,20),"O1:O2");
//        global_panel->appendWidget(osc_mix_slider);
//        //connect(osc_mix_slider,PAR_OSC_MIX_AMOUNT);

      // ----- flt1 -----

      MIP_PanelWidget* flt1_panel = new MIP_PanelWidget(MIP_FRect(530,345,276,145) );
      flt1_panel->setBackgroundColor(0.55);
      controls->appendWidget(flt1_panel);

        // text

        MIP_TextWidget* flt_text = new MIP_TextWidget(MIP_FRect(0,0,276,20),"Filter");
        flt1_panel->appendWidget(flt_text);
        flt_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        flt_text->setDrawBorder(true);
        flt_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        flt_text->setDrawBorder(false);
        flt_text->setFillBackground(true);
        flt_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // type

        MIP_SelectorWidget* flt1_type_selector = new MIP_SelectorWidget( MIP_FRect(10,30,256,20) );
        flt1_panel->appendWidget(flt1_type_selector);
        flt1_type_selector->setText("---");
        flt1_type_selector->setMenu(flt_type_menu);
        connect(flt1_type_selector,PAR_FLT1_TYPE);

        // freq

        MIP_Knob2Widget* flt1_freq_knob = new MIP_Knob2Widget( MIP_FRect(10,60,50,82),"Freq");
        flt1_panel->appendWidget(flt1_freq_knob);
        connect(flt1_freq_knob,PAR_FLT1_FREQ);

        // res

        MIP_Knob2Widget* flt1_res_knob = new MIP_Knob2Widget( MIP_FRect(70,60,50,82),"Res");
        flt1_panel->appendWidget(flt1_res_knob);
        connect(flt1_res_knob,PAR_FLT1_RES);

      // ----- env1

      MIP_PanelWidget* env1_panel = new MIP_PanelWidget(MIP_FRect(530,500,276,112) );
      env1_panel->setFillBackground(true);
      env1_panel->setBackgroundColor(0.55);
      controls->appendWidget(env1_panel);

        // text

        MIP_TextWidget* env_text = new MIP_TextWidget(MIP_FRect(0,0,276,20),"Envelope");
        env1_panel->appendWidget(env_text);
        env_text->setTextAlignment(MIP_TEXT_ALIGN_CENTER);
        env_text->setDrawBorder(true);
        env_text->setTextColor(MIP_COLOR_LIGHT_GRAY);
        env_text->setDrawBorder(false);
        env_text->setFillBackground(true);
        env_text->setBackgroundColor(MIP_COLOR_DARK_GRAY);

        // att

        MIP_Knob2Widget* env1_att_knob = new MIP_Knob2Widget( MIP_FRect(10,30,50,82),"Att");
        env1_panel->appendWidget(env1_att_knob);
        connect(env1_att_knob,PAR_ENV1_ATT);

        // dec

        MIP_Knob2Widget* env1_dec_knob = new MIP_Knob2Widget( MIP_FRect(70,30,50,82),"Dec");
        env1_panel->appendWidget(env1_dec_knob);
        connect(env1_dec_knob,PAR_ENV1_DEC);

        // sus

        MIP_Knob2Widget* env1_sus_knob = new MIP_Knob2Widget( MIP_FRect(130,30,50,82),"Sus");
        env1_panel->appendWidget(env1_sus_knob);
        connect(env1_sus_knob,PAR_ENV1_SUS);

        // rel

        MIP_Knob2Widget* env1_rel_knob = new MIP_Knob2Widget( MIP_FRect(190,30,50,82),"Rel");
        env1_panel->appendWidget(env1_rel_knob);
        connect(env1_rel_knob,PAR_ENV1_REL);

    // menus

    MEditorPanel->appendWidget(osc_type_menu);
    MEditorPanel->appendWidget(res_type_menu);
    MEditorPanel->appendWidget(flt_type_menu);
    MEditorPanel->appendWidget(wave_shaper_type_menu);
    MEditorPanel->appendWidget(phase_shaper_type_menu);
    //MEditorPanel->appendWidget(osc_mix_type_menu);

    window->appendWidget(MEditorPanel);

  }

  //----------

  /*
    redraw the voice info..
    (then to the normal timer stuff)
  */

  void on_timerCallback(void) final {
    //MIP_PRINT;
    //flushGuiParams();
    MVoiceWidget->redraw();
    MPlayingVoicesWidget->redraw();
    MReleasedVoicesWidget->redraw();
    MTotalVoicesWidget->redraw();
    MIP_Editor::on_timerCallback();
  }

};

//----------------------------------------------------------------------
#endif
