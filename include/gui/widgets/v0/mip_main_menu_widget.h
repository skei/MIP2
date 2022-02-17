#ifndef mip_main_menu_widget_included
#define mip_main_menu_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/v0/mip_menu_widget.h"
#include "gui/widgets/v0/mip_menu_item_widget.h"
#include "gui/widgets/v0/mip_panel_widget.h"
#include "gui/widgets/v0/mip_button_widget.h"

typedef MIP_Array<MIP_MenuWidget*> MIP_MenuWidgetArray ;
//----------

class MIP_MainMenuWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  MIP_MainMenuWidget(MIP_FRect ARect)
  : MIP_PanelWidget(ARect) {
    setName("MIP_MainMenuWidget");
    setHint("mainmenu");
    setFillBackground(false);
    setDrawBorder(false);
    layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    layout.innerBorder = MIP_FRect(10,10,10,10);
  }

  //----------

  virtual ~MIP_MainMenuWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual uint32_t appendMenu(const char* ALabel, MIP_MenuWidget* AMenu) {
    uint32_t index = getNumChildren();
//    MMenus.append(AMenu);
    MIP_MenuItemWidget* item;
    item = new MIP_MenuItemWidget( MIP_FRect(50,20) );
    item->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;
    item->setText(ALabel);
    appendWidget(item);
    return index;
  }

};

//----------------------------------------------------------------------
#endif

