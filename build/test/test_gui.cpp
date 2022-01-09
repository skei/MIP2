#define MIP_NO_PLUGIN
#define MIP_GUI_XCB
//#define MIP_USE_CAIRO

//#define MIP_NO_WINDOW_BUFFERING

#include "mip.h"
#include "gui/cairo/mip_cairo.h"
#include "gui/mip_window.h"

#include "gui/mip_widgets.h"
#include "plugin/mip_parameters.h"

int main() {
  //MIP_Print("Hello world!\n");

  MIP_Parameter* parameter1 = new MIP_Parameter( "p1", 0.5, 0.1, 0.9 );
  MIP_Widget* widget = nullptr;

  MIP_Window* window = new MIP_Window(640,480,"MIP_Window");
  window->setFillWindowBackground(true);
  window->setWindowBackgroundColor(MIP_COLOR_GRAY);

  // left panel

  //MIP_Widget* left_panel = new MIP_PanelWidget(MIP_FRect(200));
  //left_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
  //left_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
  //left_panel->layout.spacing = MIP_FPoint(5,5);
  //window->appendWidget(left_panel);

  MIP_ScrollBoxWidget* scroll_box = new MIP_ScrollBoxWidget(200,true,false);
  scroll_box->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
  //scroll_box->layout.innerBorder = MIP_FRect(5,5,5,5);
  //scroll_box->layout.spacing = MIP_FPoint(5,5);
  MIP_Widget* w = scroll_box->getContentWidget();
  w->layout.innerBorder = MIP_FRect(10,10,10,10);
  w->layout.spacing = MIP_FPoint(5,5);
  window->appendWidget(scroll_box);

    widget = new MIP_PanelWidget(20);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(widget);

    widget = new MIP_TextWidget(20,"Text");
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(widget);

    widget = new MIP_ValueWidget(20,"Value");
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(widget);

    widget = new MIP_DragValueWidget(20,"DragValue",0.3);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(widget);

    widget = new MIP_SliderWidget(20,"Slider");
    widget->setParameter(parameter1);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(widget);

    widget = new MIP_ScrollBarWidget(20);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(widget);

    widget = new MIP_RangeSliderWidget(20);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(widget);

    widget = new MIP_ButtonWidget(20);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(widget);

    MIP_ButtonWidget* button2 = new MIP_ButtonWidget(20);
    button2->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    button2->setIsToggle();
    scroll_box->appendWidget(button2);

    widget = new MIP_ButtonRowWidget(20,5);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(widget);

    MIP_ButtonRowWidget* buttonrow2 = new MIP_ButtonRowWidget(20,8,nullptr,MIP_BUTTON_ROW_MULTI);
    buttonrow2->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(buttonrow2);

    widget = new MIP_KeyboardWidget(35);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(widget);

    widget = new MIP_SelectorWidget(20);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(widget);

    widget = new MIP_SliderBankWidget(30);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    scroll_box->appendWidget(widget);

  // sizer

  MIP_SizerWidget* sizer = new MIP_SizerWidget(MIP_FRect(5),MIP_SIZER_LEFT,scroll_box);
  window->appendWidget(sizer);

  // right panel

  MIP_Widget* right_panel = new MIP_PanelWidget(MIP_FRect(0));
  right_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
  right_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
  right_panel->layout.spacing = 10;//MIP_FPoint(10);
  window->appendWidget(right_panel);

    // groupbox

    MIP_GroupBoxWidget* groupbox = new MIP_GroupBoxWidget(100);
    groupbox->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    groupbox->getTitleBar()->setDrawTriangle();
    right_panel->appendWidget(groupbox);


      MIP_PanelWidget* group = new MIP_PanelWidget();
      group->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
      //group->layout.innerBorder = MIP_FRect(10,10,10,10);
      //group->layout.spacing = 5;
      //group->setBackgroundColor(MIP_Color(0.6,0.5,0.5));
      groupbox->appendWidget(group);

        MIP_ValueGraphWidget* vg = new MIP_ValueGraphWidget(100,64);
        vg->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
        group->appendWidget(vg);

    // tabs

    MIP_TabsWidget* tabs = new MIP_TabsWidget(200,3);
    tabs->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    tabs->setHeight(200);
    //MIP_Widget* pages = tabs->getPages();
    //pages->layout.innerBorder = MIP_FRect(10,10,10,10);
    right_panel->appendWidget(tabs);

      // page 1

      MIP_PanelWidget* page1 = new MIP_PanelWidget();
      page1->layout.innerBorder = MIP_FRect(10,10,10,10);
      page1->layout.spacing = 5;
      tabs->appendPage("page1",page1);

        widget = new MIP_KnobWidget(MIP_FRect(40));
        widget->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;//MIP_WIDGET_ALIGN_LEFT_TOP;
        page1->appendWidget(widget);

        widget = new MIP_CurveWidget(40);
        widget->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
        page1->appendWidget(widget);

        widget = new MIP_Knob2Widget( MIP_FRect(40,72) );
        widget->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
        page1->appendWidget(widget);

      // page 2

      MIP_PanelWidget* page2 = new MIP_PanelWidget();
      page2->setBackgroundColor(MIP_Color(0.5,0.6,0.5));
      tabs->appendPage("page2",page2);

      // page 3

      MIP_PanelWidget* page3 = new MIP_PanelWidget();
      page3->setBackgroundColor(MIP_Color(0.5,0.5,0.6));
      tabs->appendPage("page3",page3);

  tabs->selectPage(0);

  window->open();
  window->eventLoop();
  window->close();
  delete window;

  delete parameter1;

  return 0;
}
