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

  MIP_Widget* left_panel = new MIP_PanelWidget(MIP_FRect(200));
  left_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
  left_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
  left_panel->layout.spacing = MIP_FPoint(5,5);
  window->appendWidget(left_panel);

    widget = new MIP_PanelWidget(20);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(widget);

    widget = new MIP_TextWidget(20,"Text");
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(widget);

    widget = new MIP_ValueWidget(20,"Value");
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(widget);

    widget = new MIP_DragValueWidget(20,"DragValue",0.3);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(widget);

    widget = new MIP_SliderWidget(20,"Slider");
    widget->setParameter(parameter1);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(widget);

    widget = new MIP_ScrollBarWidget(20);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(widget);

    widget = new MIP_RangeSliderWidget(20);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(widget);

    widget = new MIP_ButtonWidget(20);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(widget);

    widget = new MIP_ButtonRowWidget(20,5);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(widget);

    widget = new MIP_KeyboardWidget(35);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(widget);

    widget = new MIP_SelectorWidget(20);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(widget);

    widget = new MIP_SliderBankWidget(30);
    widget->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    left_panel->appendWidget(widget);

  // sizer

  MIP_SizerWidget* sizer = new MIP_SizerWidget(MIP_FRect(5),MIP_SIZER_LEFT,left_panel);
  window->appendWidget(sizer);

  // right panel

  MIP_Widget* right_panel = new MIP_PanelWidget(MIP_FRect(0));
  right_panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;
  right_panel->layout.innerBorder = MIP_FRect(10,10,10,10);
  right_panel->layout.spacing = 10;//MIP_FPoint(10);
  window->appendWidget(right_panel);

    widget = new MIP_KnobWidget(MIP_FRect(10,10,40,40));
    widget->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
    right_panel->appendWidget(widget);

    widget = new MIP_CurveWidget(40);
    widget->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
    right_panel->appendWidget(widget);

    widget = new MIP_Knob2Widget( MIP_FRect(50,10,40,72) );
    widget->layout.alignment = MIP_WIDGET_ALIGN_STACK_HORIZ;
    right_panel->appendWidget(widget);

  //window->alignWidgets();
  window->open();
  window->eventLoop();
  window->close();
  delete window;

  delete parameter1;

  return 0;
}
