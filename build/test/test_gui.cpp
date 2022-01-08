
#define MIP_NO_PLUGIN
#define MIP_GUI_XCB
//#define MIP_USE_CAIRO

//#define MIP_NO_WINDOW_BUFFERING

#include "mip.h"
#include "gui/cairo/mip_cairo.h"
#include "gui/mip_window.h"

#include "gui/mip_widgets.h"

int main() {
  //MIP_Print("Hello world!\n");

  MIP_Window* window = new MIP_Window(640,480,"MIP_Window");
  //window->setFillBackground(false);

  window->appendWidget( new MIP_PanelWidget(        MIP_FRect(  10, 10,   150,20  )  ));
  window->appendWidget( new MIP_TextWidget(         MIP_FRect(  10, 35,   150,20  ), "Text" ));
  window->appendWidget( new MIP_ValueWidget(        MIP_FRect(  10, 60,   150,20  ), "Value" ));
  window->appendWidget( new MIP_DragValueWidget(    MIP_FRect(  10, 85,   150,20  ), "DragValue", 0.3  ));
  window->appendWidget( new MIP_SliderWidget(       MIP_FRect(  10, 110,  150,20  ), "Slider"  ));
  window->appendWidget( new MIP_ScrollBarWidget(    MIP_FRect(  10, 135,  150,20  )  ));
  window->appendWidget( new MIP_ButtonWidget(       MIP_FRect(  10, 160,  150,20  )  ));
  window->appendWidget( new MIP_RangeSliderWidget(  MIP_FRect(  10, 185,  150,20  )  ));
  window->appendWidget( new MIP_ButtonRowWidget(    MIP_FRect(  10, 210,  150,20  ), 5 ));
  window->appendWidget( new MIP_KeyboardWidget(     MIP_FRect(  10, 235,  150,35  )  ));
  window->appendWidget( new MIP_SelectorWidget(     MIP_FRect(  10, 275,  150,20  )  ));
  window->appendWidget( new MIP_SliderBankWidget(   MIP_FRect(  10, 300,  150,30  )  ));

  window->appendWidget( new MIP_KnobWidget(         MIP_FRect(  170,26,   40,40   )  ));
  window->appendWidget( new MIP_Knob2Widget(        MIP_FRect(  215,10,   40,72   )  ));
  window->appendWidget( new MIP_CurveWidget(        MIP_FRect(  260,26,   40,40   )  ));

  //window->alignWidgets();
  window->open();
  window->eventLoop();
  window->close();
  delete window;

  return 0;
}
