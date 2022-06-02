#ifndef sa_ts1_editor_included
#define sa_ts1_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_editor.h"

#include "sa_ts1_widgets.h"

//----------------------------------------------------------------------

#define EDITOR_WIDTH          536 + 240
#define EDITOR_HEIGHT         530


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

// left

      // ----- osc1

      MIP_PanelWidget* osc1_panel = new MIP_PanelWidget(MIP_FRect(10,10,230,174) );
      osc1_panel->setFillBackground(true);
      osc1_panel->setBackgroundColor(0.55);
      controls->appendWidget(osc1_panel);

        // pulse

        MIP_Knob2Widget* osc1_pulse_knob = new MIP_Knob2Widget( MIP_FRect(0,0,50,82),"Pulse");
        osc1_panel->appendWidget(osc1_pulse_knob);
        connect(osc1_pulse_knob,PAR_OSC1_PULSE);

        // width

        MIP_Knob2Widget* osc1_width_knob = new MIP_Knob2Widget( MIP_FRect(60,0,50,82),"Width");
        osc1_width_knob->getKnobWidget()->setSnap(true);
        osc1_width_knob->getKnobWidget()->setSnapPos(0.5);
        osc1_width_knob->getKnobWidget()->setSnapDist(0.05);
        osc1_width_knob->getKnobWidget()->setBipolar(true);
        osc1_panel->appendWidget(osc1_width_knob);
        connect(osc1_width_knob,PAR_OSC1_WIDTH);

        // tri

        MIP_Knob2Widget* osc1_tri_knob = new MIP_Knob2Widget( MIP_FRect(120,0,50,82),"Tri");
        osc1_panel->appendWidget(osc1_tri_knob);
        connect(osc1_tri_knob,PAR_OSC1_TRI);

        // sin

        MIP_Knob2Widget* osc1_sin_knob = new MIP_Knob2Widget( MIP_FRect(180,0,50,82),"Sin");
        osc1_panel->appendWidget(osc1_sin_knob);
        connect(osc1_sin_knob,PAR_OSC1_SIN);

        // oct

        MIP_Knob2Widget* osc1_oct_knob = new MIP_Knob2Widget( MIP_FRect(0,92,50,82),"Oct");
        osc1_oct_knob->getKnobWidget()->setSnap(true);
        osc1_oct_knob->getKnobWidget()->setSnapPos(0.5);
        osc1_oct_knob->getKnobWidget()->setSnapDist(0.05);
        osc1_oct_knob->getKnobWidget()->setBipolar(true);
        //osc1_oct_knob->getKnobWidget()->setQuantize(true);
        osc1_panel->appendWidget(osc1_oct_knob);
        connect(osc1_oct_knob,PAR_OSC1_OCT);

        // semi

        MIP_Knob2Widget* osc1_semi_knob = new MIP_Knob2Widget( MIP_FRect(60,92,50,82),"Semi");
        osc1_semi_knob->getKnobWidget()->setSnap(true);
        osc1_semi_knob->getKnobWidget()->setSnapPos(0.5);
        osc1_semi_knob->getKnobWidget()->setSnapDist(0.05);
        osc1_semi_knob->getKnobWidget()->setBipolar(true);
        osc1_panel->appendWidget(osc1_semi_knob);
        connect(osc1_semi_knob,PAR_OSC1_SEMI);

        // cent

        MIP_Knob2Widget* osc1_cent_knob = new MIP_Knob2Widget( MIP_FRect(120,92,50,82),"Cent");
        osc1_cent_knob->getKnobWidget()->setSnap(true);
        osc1_cent_knob->getKnobWidget()->setSnapPos(0.5);
        osc1_cent_knob->getKnobWidget()->setSnapDist(0.05);
        osc1_cent_knob->getKnobWidget()->setBipolar(true);
        osc1_panel->appendWidget(osc1_cent_knob);
        connect(osc1_cent_knob,PAR_OSC1_CENT);

      // ----- res1 -----

      MIP_PanelWidget* res1_panel = new MIP_PanelWidget(MIP_FRect(10,194,230,174) );
      res1_panel->setBackgroundColor(0.55);
      controls->appendWidget(res1_panel);

        // noise

        MIP_Knob2Widget* res1_noise_knob = new MIP_Knob2Widget( MIP_FRect(0,0,50,82),"Noise");
        res1_panel->appendWidget(res1_noise_knob);
        connect(res1_noise_knob,PAR_RES1_NOISE);

        // nshape

        MIP_Knob2Widget* res1_nshape_knob = new MIP_Knob2Widget( MIP_FRect(60,0,50,82),"NShape");
        res1_panel->appendWidget(res1_nshape_knob);
        connect(res1_nshape_knob,PAR_RES1_NSHAPE);

        // fb

        MIP_Knob2Widget* res1_fb_knob = new MIP_Knob2Widget( MIP_FRect(120,0,50,82),"FB");
        res1_panel->appendWidget(res1_fb_knob);
        connect(res1_fb_knob,PAR_RES1_FB);

        // damp

        MIP_Knob2Widget* res1_damp_knob = new MIP_Knob2Widget( MIP_FRect(180,0,50,82),"Damp");
        res1_panel->appendWidget(res1_damp_knob);
        connect(res1_damp_knob,PAR_RES1_DAMP);

        // oct

        MIP_Knob2Widget* res1_oct_knob = new MIP_Knob2Widget( MIP_FRect(0,92,50,82),"Oct");
        res1_oct_knob->getKnobWidget()->setSnap(true);
        res1_oct_knob->getKnobWidget()->setSnapPos(0.5);
        res1_oct_knob->getKnobWidget()->setSnapDist(0.05);
        res1_oct_knob->getKnobWidget()->setBipolar(true);
        res1_panel->appendWidget(res1_oct_knob);
        connect(res1_oct_knob,PAR_RES1_OCT);

        // semi

        MIP_Knob2Widget* res1_semi_knob = new MIP_Knob2Widget( MIP_FRect(60,92,50,82),"Semi");
        res1_semi_knob->getKnobWidget()->setSnap(true);
        res1_semi_knob->getKnobWidget()->setSnapPos(0.5);
        res1_semi_knob->getKnobWidget()->setSnapDist(0.05);
        res1_semi_knob->getKnobWidget()->setBipolar(true);
        res1_panel->appendWidget(res1_semi_knob);
        connect(res1_semi_knob,PAR_RES1_SEMI);

        // cent

        MIP_Knob2Widget* res1_cent_knob = new MIP_Knob2Widget( MIP_FRect(120,92,50,82),"Cent");
        res1_cent_knob->getKnobWidget()->setSnap(true);
        res1_cent_knob->getKnobWidget()->setSnapPos(0.5);
        res1_cent_knob->getKnobWidget()->setSnapDist(0.05);
        res1_cent_knob->getKnobWidget()->setBipolar(true);
        res1_panel->appendWidget(res1_cent_knob);
        connect(res1_cent_knob,PAR_RES1_CENT);

        // rough

        MIP_Knob2Widget* res1_rough_knob = new MIP_Knob2Widget( MIP_FRect(180,92,50,82),"Rough");
        res1_panel->appendWidget(res1_rough_knob);
        res1_rough_knob->getKnobWidget()->setSnap(true);
        res1_rough_knob->getKnobWidget()->setSnapPos(0.5);
        res1_rough_knob->getKnobWidget()->setSnapDist(0.05);
        connect(res1_rough_knob,PAR_RES1_ROUGH);

// mid

      // ----- osc2

      MIP_PanelWidget* osc2_panel = new MIP_PanelWidget(MIP_FRect(250,10,230,174) );
      osc2_panel->setFillBackground(true);
      osc2_panel->setBackgroundColor(0.55);
      controls->appendWidget(osc2_panel);

      // ----- res1 -----

      MIP_PanelWidget* res2_panel = new MIP_PanelWidget(MIP_FRect(250,194,230,174) );
      res2_panel->setFillBackground(true);
      res2_panel->setBackgroundColor(0.55);
      controls->appendWidget(res2_panel);

// right

      // voices

      MIP_PanelWidget* voice2_panel = new MIP_PanelWidget(MIP_FRect(490,10,256,55) );
      voice2_panel->setBackgroundColor(0.55);
      controls->appendWidget(voice2_panel);

        MVoiceWidget = new sa_ts1_VoiceWidget( MIP_FRect(0,0,256,8) );
        voice2_panel->appendWidget(MVoiceWidget);

        MPlayingVoicesWidget = new MIP_ValueWidget( MIP_FRect(0,10,150,15), "Playing voices:" );
        MPlayingVoicesWidget->setDigits(0);
        voice2_panel->appendWidget(MPlayingVoicesWidget);

        MReleasedVoicesWidget = new MIP_ValueWidget( MIP_FRect(0,25,150,15), "Released voices:" );
        MReleasedVoicesWidget->setDigits(0);
        voice2_panel->appendWidget(MReleasedVoicesWidget);

        MTotalVoicesWidget = new MIP_ValueWidget( MIP_FRect(0,40,150,15), "Total:" );
        MTotalVoicesWidget->setDigits(0);
        voice2_panel->appendWidget(MTotalVoicesWidget);

      // ----- global -----

      MIP_PanelWidget* global_panel = new MIP_PanelWidget(MIP_FRect(490,75,230,82) );
      global_panel->setBackgroundColor(0.57);
      controls->appendWidget(global_panel);

        // vol

        MIP_Knob2Widget* global_vol_knob = new MIP_Knob2Widget( MIP_FRect(0,0,50,82),"Vol");
        global_panel->appendWidget(global_vol_knob);
        connect(global_vol_knob,PAR_VOL);

        // pan

        MIP_Knob2Widget* global_pan_knob = new MIP_Knob2Widget( MIP_FRect(60,0,50,82),"Pan");
        global_pan_knob->getKnobWidget()->setSnap(true);
        global_pan_knob->getKnobWidget()->setSnapPos(0.5);
        global_pan_knob->getKnobWidget()->setSnapDist(0.05);
        global_pan_knob->getKnobWidget()->setBipolar(true);
        global_panel->appendWidget(global_pan_knob);
        connect(global_pan_knob,PAR_PAN);

        // osc1 out

        MIP_Knob2Widget* global_osc1_out_knob = new MIP_Knob2Widget( MIP_FRect(120,0,50,82),"O1 out");
        global_panel->appendWidget(global_osc1_out_knob);
        connect(global_osc1_out_knob,PAR_OSC1_OUT);

        // res1 out

        MIP_Knob2Widget* global_res1_out_knob = new MIP_Knob2Widget( MIP_FRect(180,0,50,82),"R1 out");
        global_panel->appendWidget(global_res1_out_knob);
        connect(global_res1_out_knob,PAR_RES1_OUT);

      // ----- flt1 -----

      //(270,75,230,82) );

      MIP_PanelWidget* flt1_panel = new MIP_PanelWidget(MIP_FRect(490,167,170,82) );
      flt1_panel->setBackgroundColor(0.55);
      controls->appendWidget(flt1_panel);

        // type

        MIP_Knob2Widget* flt1_type_knob = new MIP_Knob2Widget( MIP_FRect(0,0,50,82),"Type");
        flt1_panel->appendWidget(flt1_type_knob);
        connect(flt1_type_knob,PAR_FLT1_TYPE);

        // freq

        MIP_Knob2Widget* flt1_freq_knob = new MIP_Knob2Widget( MIP_FRect(60,0,50,82),"Freq");
        flt1_panel->appendWidget(flt1_freq_knob);
        connect(flt1_freq_knob,PAR_FLT1_FREQ);

        // res

        MIP_Knob2Widget* flt1_res_knob = new MIP_Knob2Widget( MIP_FRect(120,0,50,82),"Res");
        flt1_panel->appendWidget(flt1_res_knob);
        connect(flt1_res_knob,PAR_FLT1_RES);

      // ----- env1

      MIP_PanelWidget* env1_panel = new MIP_PanelWidget(MIP_FRect(490,259,230,82) );
      env1_panel->setFillBackground(true);
      env1_panel->setBackgroundColor(0.55);
      controls->appendWidget(env1_panel);

        // att

        MIP_Knob2Widget* env1_att_knob = new MIP_Knob2Widget( MIP_FRect(0,0,50,82),"Att");
        env1_panel->appendWidget(env1_att_knob);
        connect(env1_att_knob,PAR_ENV1_ATT);

        // dec

        MIP_Knob2Widget* env1_dec_knob = new MIP_Knob2Widget( MIP_FRect(60,0,50,82),"Dec");
        env1_panel->appendWidget(env1_dec_knob);
        connect(env1_dec_knob,PAR_ENV1_DEC);

        // sus

        MIP_Knob2Widget* env1_sus_knob = new MIP_Knob2Widget( MIP_FRect(120,0,50,82),"Sus");
        env1_panel->appendWidget(env1_sus_knob);
        connect(env1_sus_knob,PAR_ENV1_SUS);

        // rel

        MIP_Knob2Widget* env1_rel_knob = new MIP_Knob2Widget( MIP_FRect(180,0,50,82),"Rel");
        env1_panel->appendWidget(env1_rel_knob);
        connect(env1_rel_knob,PAR_ENV1_REL);

    window->appendWidget(MEditorPanel);
  }

  //----------

  /*
    redraw the voice info..
    (then to the normal timer stuff)
  */

  void on_timerCallback(void) final {
    MVoiceWidget->redraw();
    MPlayingVoicesWidget->redraw();
    MReleasedVoicesWidget->redraw();
    MTotalVoicesWidget->redraw();
    MIP_Editor::on_timerCallback();
  }

};

//----------------------------------------------------------------------
#endif
