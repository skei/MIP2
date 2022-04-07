#ifndef mip_menu_widget_included
#define mip_menu_widget_included
//----------------------------------------------------------------------

/*
  TODO:
  - take menuitem's text x/y offset into account when calulating
    menu size?
*/

//#include "gui/widgets/v0/mip_menu_item_widget.h"
//#include "gui/widgets/v0/mip_panel_widget.h"
#include "gui/mip_widgets.h"


//typedef vector<char*> MIP_MenuItems;
typedef MIP_Array<char*> MIP_MenuItems;

//----------------------------------------------------------------------

class MIP_MenuListener {
  public:
    virtual void on_menuEvent(int32_t AIndex) {}
};

//----------------------------------------------------------------------

class MIP_MenuWidget
: public MIP_PanelWidget
, public MIP_MenuListener {

//------------------------------
protected:
//------------------------------

  MIP_MenuListener*   MListener     = nullptr;
  MIP_MenuItems       MItems;
  uint32_t            MBorderSize   = 5; //0; //1;
  float               MMenuWidth    = 0;//96;
  float               MMenuHeight   = 0;//16;
  uint32_t            MItemWidth    = 0;//96;
  uint32_t            MItemHeight   = 0;//16;
  int32_t             MSelectedItem = -1;
  int32_t             MPrevSelected = -1;
  uint32_t            MItemsX       = 1;
  uint32_t            MItemsY       = 1;
  bool                MMirrorX      = false;
  bool                MMirrorY      = false;

  bool MAlwaysOpen = false;

//------------------------------
public:
//------------------------------

  MIP_MenuWidget(MIP_FRect ARect)
  : MIP_PanelWidget(ARect) {

    setName("MIP_MenuWidget");
    setHint("menu");
//    MMenuWidth  = ARect.w;
//    MMenuHeight = ARect.h;
    if (MAlwaysOpen) {
      flags.active = true;
      flags.visible = true;
    }
    else {
      flags.active = false;
      flags.visible = false;
    }
    setFillBackground();
    setBackgroundColor(MIP_COLOR_LIGHT_GRAY);
    setDrawBorder();
  }

  //----------

  virtual ~MIP_MenuWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual int32_t     getSelectedItem()               { return MSelectedItem; }
  virtual uint32_t    getNumItems()                   { return MItems.size(); }
  virtual const char* getItem(uint32_t AIndex)        { return MItems[AIndex]; }
  virtual bool        getAlwaysOpen()                 { return MAlwaysOpen; }
  virtual void        setAlwaysOpen(bool state=true)  { MAlwaysOpen = state; }

  //----------

  virtual void setListener(MIP_MenuListener* AListener)     { MListener = AListener; }
  virtual void setItemWidth(int32_t AWidth)                 { MItemWidth = AWidth; }
  virtual void setItemHeight(int32_t AHeight)               { MItemHeight = AHeight; }
  virtual void setItemSize(int32_t AWidth, int32_t AHeight) { MItemWidth = AWidth; MItemHeight = AHeight; }
  virtual void setItemLayout(int32_t x, int32_t y)          { MItemsX = x; MItemsY = y; }
  virtual void setMenuMirror(bool x, bool y)                { MMirrorX = x; MMirrorY = y; }

//------------------------------
public:
//------------------------------

  virtual void appendMenuItem(const char* AItem) {
    //MItems.append((char*)AItem);
    char* item = (char*)AItem;
    MItems.append(item);
  }

  //----------

  virtual void appendMenuItems(const char** AItems, uint32_t ANum) {
    for (uint32_t i=0; i<ANum; i++) {
      appendMenuItem(AItems[i]);
    }
  }

  //----------

//  virtual void clearMenu(void) {
//    MItems.clear();
//    deleteChildren();
//  }

  //----------

  virtual void open(MIP_MenuListener* AListener, int32_t AXpos, int32_t AYpos) {
    if (MAlwaysOpen) return;
    //MIP_FRect mrect = getRect();
    //MIP_Print("%i,%i / %i,%i\n",MItemWidth,MItemHeight,MItemsX,MItemsY);
    MListener = AListener;
    MPrevSelected = MSelectedItem;
    //int32_t w = (MItemWidth  * MItemsX) + (MBorderSize * (MItemsX+1));
    //int32_t h = (MItemHeight * MItemsY) + (MBorderSize * 2 /*(MItemsY+1)*/);
    MMenuWidth = (MItemWidth  * MItemsX) + (MBorderSize * (MItemsX+1));
    MMenuHeight = (MItemHeight * MItemsY) + (MBorderSize * 2 /*(MItemsY+1)*/);
    // h = (MItems.size() * MItemHeight)  + (MBorderSize * 2);
    if (MMirrorX) AXpos -= MMenuWidth;
    if (MMirrorY) AYpos -= MMenuHeight;

    //setRect(MIP_FRect(AXpos,AYpos,w,h));

    //MIP_Print("%i,%i - %i,%i\n",AXpos,AYpos,w,h);
    MIP_FRect R = MIP_FRect(AXpos+MBorderSize, AYpos+MBorderSize, MItemWidth, MItemHeight);
    uint32_t num = MItems.size();
    uint32_t i = 0;
    deleteWidgets();
    for (uint32_t x=0; x<MItemsX; x++) {
      for (uint32_t y=0; y<MItemsY; y++) {
        if (i < num) {
          const char* txt = MItems[i];
          //MIP_Print("%i %i %s : %i %i %i %i\n",x,y,txt,(int)R.x,(int)R.y,(int)R.w,(int)R.h);
          MIP_MenuItemWidget* mi = (MIP_MenuItemWidget*)appendWidget( new MIP_MenuItemWidget( R ));
          mi->setText(txt);
          R.y += MItemHeight;
        }
        i++;
      }
      R.y = AYpos + MBorderSize;
      R.x += MItemWidth + MBorderSize;
    }
    //MIsActive = true;
    //MIsVisible = true;
    flags.active = true;
    flags.visible = true;
    //setWidth(MMenuWidth);
    //setHeight(MMenuHeight);

    MIP_FRect menu_rect = MIP_FRect(AXpos,AYpos,MMenuWidth,MMenuHeight);
    //MIP_Print("%.0f,%.0f - %.0f,%.0f\n",menu_rect.x,menu_rect.y,menu_rect.w,menu_rect.h);
    //setRect(AXpos,AYpos,MMenuWidth,MMenuHeight);
    setRect(menu_rect);
    do_widget_redraw(this,menu_rect,0); // redraw parent?
    do_widget_setModal(this);
  }

  //----------

  virtual void close(void) {
    if (MAlwaysOpen) return;
    MListener = nullptr;
    //MIsActive = false;
    //MIsVisible = false;
    flags.active = false;
    flags.visible = false;
    do_widget_redraw(this,getRect(),0);
    do_widget_setModal(nullptr);
    setWidth(0);
    setHeight(0);
  }

//------------------------------
public:
//------------------------------

  //virtual void appendMenuItem2(const char* ALabel) {
  //  MIP_MenuItemWidget* text;
  //  text = new MIP_MenuItemWidget( MIP_FRect(50,20) );
  //  text->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
  //  text->setText(ALabel);
  //  appendWidget(text);
  //}

  //----------

  //virtual void open2(int32_t AXpos, int32_t AYpos) {
  //}

//------------------------------
public:
//------------------------------

  void alignWidgets(bool ARecursive=true) override {
    MIP_PanelWidget::alignWidgets(ARecursive);
  }

//------------------------------
public:
//------------------------------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
    MIP_PRINT;
    MIP_PanelWidget::on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp);
    if (AButton == MIP_BUTTON_LEFT) {
      if (!getRect().contains(AXpos,AYpos)) {
        // left-clicked outside of widget
        MSelectedItem = MPrevSelected;
        if (MListener) MListener->on_menuEvent(MSelectedItem);
        if (!MAlwaysOpen) close();
      }
    }
    if (AButton == MIP_BUTTON_RIGHT) {
      // right-clicked
      MSelectedItem = MPrevSelected;
      if (MListener) MListener->on_menuEvent(MSelectedItem);
      if (!MAlwaysOpen) close();
    }
  }

//------------------------------
public:
//------------------------------

  // item selected

  void do_widget_update(MIP_Widget* ASender, uint32_t AMode=0) override {
    //MIP_PRINT;
    int32_t index = ASender->getWidgetIndex();
    MSelectedItem = index;
    if (MListener) MListener->on_menuEvent(MSelectedItem);
    if (!MAlwaysOpen) close();
    MIP_PanelWidget::do_widget_update(this,AMode);
    //MIP_Widget::do_widget_update(ASender);
  }

  //----------

  // (right button clicked)

  void do_widget_notify(MIP_Widget* AWidget, uint32_t AValue=0) override {
    //MIP_PRINT;
    if (AValue == MIP_MENU_NOTIFY_CLOSE) {
      // right clicked on menuitem
      MSelectedItem = MPrevSelected;
      if (MListener) MListener->on_menuEvent(MSelectedItem);
      if (!MAlwaysOpen) close();
    }
    //MIP_Widget::do_widget_notify(AWidget,AValue);
  }

};

//----------------------------------------------------------------------
#endif
