
#define MIP_GUI_XCB
#define MIP_PAINTER_CAIRO
//#define MIP_NO_PLUGIN

#include "mip.h"
#include "gui/mip_window.h"
#include "gui/mip_widgets.h"

#include "../data/img/knob4_60x60_131.h"

#include "gui/mip_widget2.h"


//----------------------------------------------------------------------
//
// window
//
//----------------------------------------------------------------------

class myWindow
: public MIP_Window
, public MIP_MenuListener {

//------------------------------
private:
//------------------------------

  MIP_Bitmap*   MKnobBitmap   = nullptr;
  MIP_Surface*  MKnobSurface  = nullptr;

//------------------------------
public:
//------------------------------

  myWindow(uint32_t AWidth, uint32_t AHeight, MIP_WindowListener* AListener=nullptr)
  : MIP_Window(AWidth,AHeight,AListener,false) {
    init();
  }

  //----------

  virtual ~myWindow() {
    cleanup();
  }

//------------------------------
public:
//------------------------------

  void on_menuEvent(int32_t AIndex) override {
    MIP_Print("%i\n",AIndex);
  }


//------------------------------
public:
//------------------------------

  void init() {

    // menus

    MIP_MenuWidget* file_menu = new MIP_MenuWidget(MIP_FRect());
    file_menu->setItemLayout(1,5);
    file_menu->setItemSize(80,16);
    file_menu->appendMenuItem("New");
    file_menu->appendMenuItem("Open");
    file_menu->appendMenuItem("Save");
    file_menu->appendMenuItem("---");
    file_menu->appendMenuItem("Exit");

    MIP_MenuWidget* host_menu = new MIP_MenuWidget(MIP_FRect());
    host_menu->setItemLayout(1,1);
    host_menu->setItemSize(80,16);
    host_menu->appendMenuItem("Reset");

    MIP_MenuWidget* plugin_menu = new MIP_MenuWidget(MIP_FRect());
    plugin_menu->setItemLayout(1,3);
    plugin_menu->setItemSize(80,16);
    plugin_menu->appendMenuItem("Load");
    plugin_menu->appendMenuItem("Unload");
    plugin_menu->appendMenuItem("Open Editor");

    //

    MIP_MainMenuWidget* main_menu = new MIP_MainMenuWidget(MIP_FRect(25),this);
    appendWidget(main_menu);
    //main_menu->setItemSize(32,16);

    main_menu->appendMenu("File",file_menu);
    main_menu->appendMenu("Host",host_menu);
    main_menu->appendMenu("Plugin",plugin_menu);

    // knob bitmap

    MKnobBitmap = new MIP_Bitmap(knob4_60x60_131,knob4_60x60_131_size);
    //knob_bitmap->convertRgbaToBgra();
    uint32_t color = MIP_Color(0.6);
    MKnobBitmap->premultAlpha(color);//0x808080);
    MKnobSurface = new MIP_Surface(this,MKnobBitmap->getWidth(),MKnobBitmap->getHeight());
    MIP_Painter* painter = new MIP_Painter(MKnobSurface);
    painter->uploadBitmap(0,0,MKnobBitmap);
    painter->flush();
    delete painter;

    // panel

    //setFillWindowBackground(true);
    MIP_PanelWidget* panel = new MIP_PanelWidget();
    appendWidget(panel);
    panel->layout.alignment = MIP_WIDGET_ALIGN_FILL_CLIENT;

    // knob

    MIP_ImageStripWidget* imagestrip = new MIP_ImageStripWidget( MIP_FRect(10,10, 60,60) );
    panel->appendWidget(imagestrip);
    imagestrip->setup(1,131,MKnobSurface);

    //
    appendWidget(file_menu);
    appendWidget(host_menu);
    appendWidget(plugin_menu);

  }

  //----------

  void cleanup() {
    delete MKnobSurface;
    delete MKnobBitmap;
  }

};

//----------------------------------------------------------------------
//
// main
//
//----------------------------------------------------------------------

int main() {
  myWindow* window = new myWindow(400,400);
  window->open();
  window->eventLoop();
  window->close();
  delete window;
  return 0;
}
