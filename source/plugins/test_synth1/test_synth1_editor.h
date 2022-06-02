#ifndef test_synth1_editor_included
#define test_synth1_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_editor.h"

#include "test_synth1_widgets.h"

//----------------------------------------------------------------------

#define EDITOR_WIDTH          536
#define EDITOR_HEIGHT         530


class myEditor
: public MIP_Editor {

//------------------------------
public:
//------------------------------

  MIP_VoiceWidget*  MVoiceWidget          = nullptr;
  MIP_ValueWidget*  MPlayingVoicesWidget  = nullptr;
  MIP_ValueWidget*  MReleasedVoicesWidget = nullptr;
  MIP_ValueWidget*  MTotalVoicesWidget    = nullptr;

//------------------------------
public:
//------------------------------

  myEditor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded, const clap_plugin_descriptor_t* descriptor)
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

      MIP_PanelWidget* osc_panel = new MIP_PanelWidget(MIP_FRect(10,10,230,174) );
      osc_panel->setFillBackground(true);
      osc_panel->setBackgroundColor(0.55);
      controls->appendWidget(osc_panel);

        // pulse

        MIP_Knob2Widget* pulse_knob = new MIP_Knob2Widget( MIP_FRect(0,0,50,82),"Pulse");
        osc_panel->appendWidget(pulse_knob);
        connect(pulse_knob,PAR_OSC1_PULSE);

        // width

        MIP_Knob2Widget* width_knob = new MIP_Knob2Widget( MIP_FRect(60,0,50,82),"Width");
        width_knob->getKnobWidget()->setSnap(true);
        width_knob->getKnobWidget()->setSnapPos(0.5);
        width_knob->getKnobWidget()->setSnapDist(0.05);
        width_knob->getKnobWidget()->setBipolar(true);
        osc_panel->appendWidget(width_knob);
        connect(width_knob,PAR_OSC1_WIDTH);

        // tri

        MIP_Knob2Widget* tri_knob = new MIP_Knob2Widget( MIP_FRect(120,0,50,82),"Tri");
        osc_panel->appendWidget(tri_knob);
        connect(tri_knob,PAR_OSC1_TRI);

        // sin

        MIP_Knob2Widget* sin_knob = new MIP_Knob2Widget( MIP_FRect(180,0,50,82),"Sin");
        osc_panel->appendWidget(sin_knob);
        connect(sin_knob,PAR_OSC1_SIN);

        // oct

        MIP_Knob2Widget* osc_oct_knob = new MIP_Knob2Widget( MIP_FRect(0,92,50,82),"Oct");
        osc_oct_knob->getKnobWidget()->setSnap(true);
        osc_oct_knob->getKnobWidget()->setSnapPos(0.5);
        osc_oct_knob->getKnobWidget()->setSnapDist(0.05);
        osc_oct_knob->getKnobWidget()->setBipolar(true);
        //osc_oct_knob->getKnobWidget()->setQuantize(true);
        osc_panel->appendWidget(osc_oct_knob);
        connect(osc_oct_knob,PAR_OSC1_OCT);

        // semi

        MIP_Knob2Widget* osc_semi_knob = new MIP_Knob2Widget( MIP_FRect(60,92,50,82),"Semi");
        osc_semi_knob->getKnobWidget()->setSnap(true);
        osc_semi_knob->getKnobWidget()->setSnapPos(0.5);
        osc_semi_knob->getKnobWidget()->setSnapDist(0.05);
        osc_semi_knob->getKnobWidget()->setBipolar(true);
        osc_panel->appendWidget(osc_semi_knob);
        connect(osc_semi_knob,PAR_OSC1_SEMI);

        // cent

        MIP_Knob2Widget* osc_cent_knob = new MIP_Knob2Widget( MIP_FRect(120,92,50,82),"Cent");
        osc_cent_knob->getKnobWidget()->setSnap(true);
        osc_cent_knob->getKnobWidget()->setSnapPos(0.5);
        osc_cent_knob->getKnobWidget()->setSnapDist(0.05);
        osc_cent_knob->getKnobWidget()->setBipolar(true);
        osc_panel->appendWidget(osc_cent_knob);
        connect(osc_cent_knob,PAR_OSC1_CENT);

      // ----- res1 -----

      MIP_PanelWidget* res_panel = new MIP_PanelWidget(MIP_FRect(10,194,230,174) );
      res_panel->setBackgroundColor(0.55);
      controls->appendWidget(res_panel);

        // noise

        MIP_Knob2Widget* res_noise_knob = new MIP_Knob2Widget( MIP_FRect(0,0,50,82),"Noise");
        res_panel->appendWidget(res_noise_knob);
        connect(res_noise_knob,PAR_RES1_NOISE);

        // nshape

        MIP_Knob2Widget* res_nshape_knob = new MIP_Knob2Widget( MIP_FRect(60,0,50,82),"NShape");
        res_panel->appendWidget(res_nshape_knob);
        connect(res_nshape_knob,PAR_RES1_NSHAPE);

        // fb

        MIP_Knob2Widget* res_fb_knob = new MIP_Knob2Widget( MIP_FRect(120,0,50,82),"FB");
        res_panel->appendWidget(res_fb_knob);
        connect(res_fb_knob,PAR_RES1_FB);

        // damp

        MIP_Knob2Widget* res_damp_knob = new MIP_Knob2Widget( MIP_FRect(180,0,50,82),"Damp");
        res_panel->appendWidget(res_damp_knob);
        connect(res_damp_knob,PAR_RES1_DAMP);

        // oct

        MIP_Knob2Widget* res_oct_knob = new MIP_Knob2Widget( MIP_FRect(0,92,50,82),"Oct");
        res_oct_knob->getKnobWidget()->setSnap(true);
        res_oct_knob->getKnobWidget()->setSnapPos(0.5);
        res_oct_knob->getKnobWidget()->setSnapDist(0.05);
        res_oct_knob->getKnobWidget()->setBipolar(true);
        res_panel->appendWidget(res_oct_knob);
        connect(res_oct_knob,PAR_RES1_OCT);

        // semi

        MIP_Knob2Widget* res_semi_knob = new MIP_Knob2Widget( MIP_FRect(60,92,50,82),"Semi");
        res_semi_knob->getKnobWidget()->setSnap(true);
        res_semi_knob->getKnobWidget()->setSnapPos(0.5);
        res_semi_knob->getKnobWidget()->setSnapDist(0.05);
        res_semi_knob->getKnobWidget()->setBipolar(true);
        res_panel->appendWidget(res_semi_knob);
        connect(res_semi_knob,PAR_RES1_SEMI);

        // cent

        MIP_Knob2Widget* res_cent_knob = new MIP_Knob2Widget( MIP_FRect(120,92,50,82),"Cent");
        res_cent_knob->getKnobWidget()->setSnap(true);
        res_cent_knob->getKnobWidget()->setSnapPos(0.5);
        res_cent_knob->getKnobWidget()->setSnapDist(0.05);
        res_cent_knob->getKnobWidget()->setBipolar(true);
        res_panel->appendWidget(res_cent_knob);
        connect(res_cent_knob,PAR_RES1_CENT);

        // rough

        MIP_Knob2Widget* res_rough_knob = new MIP_Knob2Widget( MIP_FRect(180,92,50,82),"Rough");
        res_panel->appendWidget(res_rough_knob);
        res_rough_knob->getKnobWidget()->setSnap(true);
        res_rough_knob->getKnobWidget()->setSnapPos(0.5);
        res_rough_knob->getKnobWidget()->setSnapDist(0.05);
        connect(res_rough_knob,PAR_RES1_ROUGH);

      // ----- flt1 -----

      MIP_PanelWidget* filter_panel = new MIP_PanelWidget(MIP_FRect(10,378,170,82) );
      filter_panel->setBackgroundColor(0.55);
      controls->appendWidget(filter_panel);

        // type

        MIP_Knob2Widget* type_knob = new MIP_Knob2Widget( MIP_FRect(0,0,50,82),"Type");
        filter_panel->appendWidget(type_knob);
        connect(type_knob,PAR_FLT1_TYPE);

        // freq

        MIP_Knob2Widget* freq_knob = new MIP_Knob2Widget( MIP_FRect(60,0,50,82),"Freq");
        filter_panel->appendWidget(freq_knob);
        connect(freq_knob,PAR_FLT1_FREQ);

        // res

        MIP_Knob2Widget* res_knob = new MIP_Knob2Widget( MIP_FRect(120,0,50,82),"Res");
        filter_panel->appendWidget(res_knob);
        connect(res_knob,PAR_FLT1_RES);

// right

      // voices

      MIP_PanelWidget* voice_panel = new MIP_PanelWidget(MIP_FRect(270,10,256,55) );
      voice_panel->setBackgroundColor(0.55);
      controls->appendWidget(voice_panel);

        MVoiceWidget = new MIP_VoiceWidget( MIP_FRect(0,0,256,8) );
        voice_panel->appendWidget(MVoiceWidget);

        MPlayingVoicesWidget = new MIP_ValueWidget( MIP_FRect(0,10,150,15), "Playing voices:" );
        MPlayingVoicesWidget->setDigits(0);
        voice_panel->appendWidget(MPlayingVoicesWidget);

        MReleasedVoicesWidget = new MIP_ValueWidget( MIP_FRect(0,25,150,15), "Released voices:" );
        MReleasedVoicesWidget->setDigits(0);
        voice_panel->appendWidget(MReleasedVoicesWidget);

        MTotalVoicesWidget = new MIP_ValueWidget( MIP_FRect(0,40,150,15), "Total:" );
        MTotalVoicesWidget->setDigits(0);
        voice_panel->appendWidget(MTotalVoicesWidget);

      // ----- global -----

      MIP_PanelWidget* global_panel = new MIP_PanelWidget(MIP_FRect(270,75,230,82) );
      global_panel->setBackgroundColor(0.57);
      controls->appendWidget(global_panel);

        // vol

        MIP_Knob2Widget* vol_knob = new MIP_Knob2Widget( MIP_FRect(0,0,50,82),"Vol");
        global_panel->appendWidget(vol_knob);
        connect(vol_knob,PAR_VOL);

        // pan

        MIP_Knob2Widget* pan_knob = new MIP_Knob2Widget( MIP_FRect(60,0,50,82),"Pan");
        pan_knob->getKnobWidget()->setSnap(true);
        pan_knob->getKnobWidget()->setSnapPos(0.5);
        pan_knob->getKnobWidget()->setSnapDist(0.05);
        pan_knob->getKnobWidget()->setBipolar(true);
        global_panel->appendWidget(pan_knob);
        connect(pan_knob,PAR_PAN);

        // osc1 out

        MIP_Knob2Widget* osc1_out_knob = new MIP_Knob2Widget( MIP_FRect(120,0,50,82),"O1 out");
        global_panel->appendWidget(osc1_out_knob);
        connect(osc1_out_knob,PAR_OSC1_OUT);

        // res1 out

        MIP_Knob2Widget* res1_out_knob = new MIP_Knob2Widget( MIP_FRect(180,0,50,82),"R1 out");
        global_panel->appendWidget(res1_out_knob);
        connect(res1_out_knob,PAR_RES1_OUT);

      // ----- env1

      MIP_PanelWidget* env_panel = new MIP_PanelWidget(MIP_FRect(270,75+10+82,230,82) );
      env_panel->setFillBackground(true);
      env_panel->setBackgroundColor(0.55);
      controls->appendWidget(env_panel);

        // att

        MIP_Knob2Widget* amp_att_knob = new MIP_Knob2Widget( MIP_FRect(0,0,50,82),"Att");
        env_panel->appendWidget(amp_att_knob);
        connect(amp_att_knob,PAR_ENV1_ATT);

        // dec

        MIP_Knob2Widget* amp_dec_knob = new MIP_Knob2Widget( MIP_FRect(60,0,50,82),"Dec");
        env_panel->appendWidget(amp_dec_knob);
        connect(amp_dec_knob,PAR_ENV1_DEC);

        // sus

        MIP_Knob2Widget* amp_sus_knob = new MIP_Knob2Widget( MIP_FRect(120,0,50,82),"Sus");
        env_panel->appendWidget(amp_sus_knob);
        connect(amp_sus_knob,PAR_ENV1_SUS);

        // rel

        MIP_Knob2Widget* amp_rel_knob = new MIP_Knob2Widget( MIP_FRect(180,0,50,82),"Rel");
        env_panel->appendWidget(amp_rel_knob);
        connect(amp_rel_knob,PAR_ENV1_REL);

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
