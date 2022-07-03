#ifndef sa_tyr_editor_included
#define sa_tyr_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_editor.h"

#include "sa_tyr_widgets.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class sa_tyr_editor
: public MIP_Editor {

//------------------------------
public:
//------------------------------


//------------------------------
public:
//------------------------------

  sa_tyr_editor(MIP_EditorListener* AListener, MIP_ClapPlugin* APlugin, uint32_t AWidth, uint32_t AHeight, bool AEmbedded, const clap_plugin_descriptor_t* ADescriptor/*=nullptr*/)
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
    sa_header->setPluginName("sa_tyr"); // (ADescriptor->name);
    sa_header->setPluginVersion(ADescriptor->version);

    // footer

    MIP_PanelWidget* footer = new MIP_PanelWidget(20);
    MEditorPanel->appendWidget(footer);
    footer->layout.alignment = MIP_WIDGET_ALIGN_FILL_BOTTOM;

    // mid_panel

    MIP_PanelWidget* mid_panel = new MIP_PanelWidget();
    MEditorPanel->appendWidget(mid_panel);
    mid_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
    mid_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
    mid_panel->layout.spacing = MIP_FPoint(5,5);
    mid_panel->setDrawBorder(true);

      //----------

      // osc1

      MIP_PanelWidget* osc1_group = new MIP_PanelWidget(MIP_FRect(200,200));
      mid_panel->appendWidget(osc1_group);
      osc1_group->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
      osc1_group->layout.innerBorder = MIP_FRect(5,5,5,5);
      osc1_group->layout.spacing = MIP_FPoint(5,5);
      osc1_group->setDrawBorder(true);

        // osc1_header

        MIP_TextWidget* osc1_text_header = new MIP_TextWidget(20,"OSC 1");
        osc1_group->appendWidget(osc1_text_header);
        osc1_text_header->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
        osc1_text_header->setFillBackground(true);
        osc1_text_header->setBackgroundColor(0.2);
        osc1_text_header->setTextColor(0.8);
        osc1_text_header->setTextOffset(5);

        // osc1 type

        MIP_SelectorWidget* osc1_selector = new MIP_SelectorWidget(MIP_FRect(10,10,130,20));
        osc1_group->appendWidget(osc1_selector);
        osc1_selector->layout.alignment = MIP_WIDGET_ALIGN_CLIENT;
        osc1_selector->setText("Type");

        // osc1 shape

        MIP_Knob2Widget* osc1_shape_knob = new MIP_Knob2Widget(MIP_FRect(10,35,40,80),"Shape");
        osc1_group->appendWidget(osc1_shape_knob);
        osc1_shape_knob->layout.alignment = MIP_WIDGET_ALIGN_CLIENT;

        // osc1 width

        MIP_Knob2Widget* osc1_width_knob = new MIP_Knob2Widget(MIP_FRect(55,35,40,80),"Width");
        osc1_group->appendWidget(osc1_width_knob);
        osc1_width_knob->layout.alignment = MIP_WIDGET_ALIGN_CLIENT;

        // osc1 ..

        MIP_Knob2Widget* osc1_knob3 = new MIP_Knob2Widget(MIP_FRect(100,35,40,80),"...");
        osc1_group->appendWidget(osc1_knob3);
        osc1_knob3->layout.alignment = MIP_WIDGET_ALIGN_CLIENT;

      //----------

      // osc2

      //----------

    MIP_Widget* break1 = new MIP_Widget(0);
    mid_panel->appendWidget(break1);
    break1->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;

      //----------

      // res1

      //----------

      // res2




      // ...

    window->appendWidget(MEditorPanel);
  }

  //----------

  //void on_timerCallback(void) final {
  //  MVoiceWidget->redraw();
  //  MIP_Editor::on_timerCallback();
  //}
};

//----------------------------------------------------------------------
#endif
