#ifndef kode_main_menu_widget_included
#define kode_main_menu_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_menu_widget.h"
#include "gui/widgets/kode_menu_item_widget.h"
#include "gui/widgets/kode_panel_widget.h"
#include "gui/widgets/kode_button_widget.h"

typedef KODE_Array<KODE_MenuWidget*> KODE_MenuWidgetArray ;
//----------

class KODE_MainMenuWidget
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------

//------------------------------
public:
//------------------------------

  KODE_MainMenuWidget(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {
    setName("KODE_MainMenuWidget");
    setHint("mainmenu");
    setFillBackground(false);
    setDrawBorder(false);
    layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    layout.innerBorder = KODE_FRect(10,10,10,10);
  }

  //----------

  virtual ~KODE_MainMenuWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual uint32_t appendMenu(const char* ALabel, KODE_MenuWidget* AMenu) {
    uint32_t index = getNumChildren();
//    MMenus.append(AMenu);
    KODE_MenuItemWidget* item;
    item = new KODE_MenuItemWidget( KODE_FRect(50,20) );
    item->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT;
    item->setText(ALabel);
    appendWidget(item);
    return index;
  }

};

//----------------------------------------------------------------------
#endif

