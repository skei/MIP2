#ifndef test_synth1_editor_included
#define test_synth1_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widgets.h"
#include "plugin/mip_editor.h"

#include "test_synth1_widgets.h"

//----------------------------------------------------------------------


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
    MEditorPanel->appendWidget(sa_header);
    sa_header->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    sa_header->setPluginName(descriptor->name);
    sa_header->setPluginVersion(descriptor->version);

    // controls panel (knobs)

    MIP_PanelWidget* controls = new MIP_PanelWidget(MIP_FRect());
    MEditorPanel->appendWidget(controls);
    controls->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    controls->setDrawBorder(false);
    controls->setFillBackground(true);
    controls->setBackgroundColor(0.6);
    controls->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

      // voices

      MVoiceWidget = new MIP_VoiceWidget( MIP_FRect(250,10,256,8) );
      controls->appendWidget(MVoiceWidget);

      MPlayingVoicesWidget = new MIP_ValueWidget( MIP_FRect(250,20,150,15), "Playing voices:" );
      controls->appendWidget(MPlayingVoicesWidget);
      MPlayingVoicesWidget->setDigits(0);

      MReleasedVoicesWidget = new MIP_ValueWidget( MIP_FRect(250,35,150,15), "Released voices:" );
      controls->appendWidget(MReleasedVoicesWidget);
      MReleasedVoicesWidget->setDigits(0);

      MTotalVoicesWidget = new MIP_ValueWidget( MIP_FRect(250,50,150,15), "Total:" );
      controls->appendWidget(MTotalVoicesWidget);
      MTotalVoicesWidget->setDigits(0);

      // vol

      MIP_Knob2Widget* vol_knob = new MIP_Knob2Widget( MIP_FRect(10,10,50,82),"Vol");
      controls->appendWidget(vol_knob);
      connect(vol_knob,0);

      // pan

      MIP_Knob2Widget* pan_knob = new MIP_Knob2Widget( MIP_FRect(70,10,50,82),"Pan");
      controls->appendWidget(pan_knob);
      connect(pan_knob,1);

      // pitch

      MIP_Knob2Widget* pitch_knob = new MIP_Knob2Widget( MIP_FRect(130,10,50,82),"Pitch");
      controls->appendWidget(pitch_knob);
      pitch_knob->getKnobWidget()->setSnap(true);
      pitch_knob->getKnobWidget()->setSnapPos(0.5);
      connect(pitch_knob,10);

      // freq

      MIP_Knob2Widget* freq_knob = new MIP_Knob2Widget( MIP_FRect(10,102,50,82),"Freq");
      controls->appendWidget(freq_knob);
      connect(freq_knob,2);

      // res

      MIP_Knob2Widget* res_knob = new MIP_Knob2Widget( MIP_FRect(70,102,50,82),"Res");
      controls->appendWidget(res_knob);
      connect(res_knob,3);

      // squ

      MIP_Knob2Widget* pulse_knob = new MIP_Knob2Widget( MIP_FRect(130,102,50,82),"Pulse");
      controls->appendWidget(pulse_knob);
      connect(pulse_knob,4);

      // width

      MIP_Knob2Widget* width_knob = new MIP_Knob2Widget( MIP_FRect(190,102,50,82),"Width");
      controls->appendWidget(width_knob);
      connect(width_knob,5);

      // amp att

      MIP_Knob2Widget* amp_att_knob = new MIP_Knob2Widget( MIP_FRect(10,194,50,82),"Att");
      controls->appendWidget(amp_att_knob);
      connect(amp_att_knob,6);

      // amp dec

      MIP_Knob2Widget* amp_dec_knob = new MIP_Knob2Widget( MIP_FRect(70,194,50,82),"Dec");
      controls->appendWidget(amp_dec_knob);
      connect(amp_dec_knob,7);

      // amp sus

      MIP_Knob2Widget* amp_sus_knob = new MIP_Knob2Widget( MIP_FRect(130,194,50,82),"Sus");
      controls->appendWidget(amp_sus_knob);
      connect(amp_sus_knob,8);

      // amp rel

      MIP_Knob2Widget* amp_rel_knob = new MIP_Knob2Widget( MIP_FRect(190,194,50,82),"Rel");
      controls->appendWidget(amp_rel_knob);
      connect(amp_rel_knob,9);

    window->appendWidget(MEditorPanel);
  }

  //----------

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
