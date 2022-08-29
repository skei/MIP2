#ifndef mip_saheader_widget_included
#define mip_saheader_widget_included
//----------------------------------------------------------------------

#include "gfx/mip_bitmap.h"

#include "gui/mip_window.h"
#include "gui/mip_surface.h"
#include "gui/widgets/mip_image_widget.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_text_widget.h"

#include "../data/img/sa_logo_40_trans_black.h"
#include "../data/img/mip2_trans_129x34.h"


//----------

//#if 0

class MIP_SAHeaderWidget
: public MIP_PanelWidget {


//------------------------------
private:
//------------------------------

  MIP_Bitmap*   MLogoBitmap   = nullptr;
  MIP_Bitmap*   MMip2Bitmap   = nullptr;

  //MIP_Surface*  MLogoSurface  = nullptr;
  //MIP_Surface*  MMip2Surface  = nullptr;


  MIP_ImageWidget*  MLogoImage      = nullptr;
  MIP_ImageWidget*  MMip2Image      = nullptr;

  MIP_TextWidget*  MClapTextWidget  = nullptr;
  char             MClapText[64]    = {};

  MIP_TextWidget*  MPluginName     = nullptr;
  MIP_TextWidget*  MPluginVersion  = nullptr;


//------------------------------
public:
//------------------------------

  //MIP_SAHeaderWidget(MIP_FRect ARect, MIP_Surface* ALogoSurface, MIP_Surface* AMip2Surface)
  MIP_SAHeaderWidget(MIP_DRect ARect/*, MIP_Window* AWindow*/)
  : MIP_PanelWidget(ARect) {

    setWidgetName("MIP_SAHeaderWidget");
    Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    Layout.border = MIP_DRect(10,10,10,10);
    //layout.spacing = MIP_FPoint(10,0);

    // bitmaps

    MLogoBitmap = new MIP_Bitmap(sa_logo_40_trans_black,sa_logo_40_trans_black_size);
    MLogoBitmap->premultAlpha(0x808080);
    MLogoBitmap->fillLayer(3,255);

    MMip2Bitmap = new MIP_Bitmap(mip2_trans_129x34,mip2_trans_129x34_size);
    MMip2Bitmap->scaleLayer(3,128);
    MMip2Bitmap->premultAlpha(0x808080);

    // image widgets

    MLogoImage = new MIP_ImageWidget( MIP_DRect(40,40), MLogoBitmap );
    MLogoImage->Layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT_TOP;
    appendChildWidget(MLogoImage);

    MMip2Image = new MIP_ImageWidget( MIP_DRect(129,34), MMip2Bitmap );
    MMip2Image->Layout.alignment = MIP_WIDGET_ALIGN_RIGHT_TOP;
    appendChildWidget(MMip2Image);

    // text - clap version

    sprintf(MClapText,"CLAP v%i.%i.%i",CLAP_VERSION_MAJOR,CLAP_VERSION_MINOR,CLAP_VERSION_REVISION);
    //MClapTextWidget = new MIP_TextWidget( MIP_DRect(0,0,129,16),MClapText );
    MClapTextWidget = new MIP_TextWidget( MIP_DRect(-37,0,129,16),MClapText );
    MClapTextWidget->Layout.alignment = MIP_WIDGET_ALIGN_RIGHT_BOTTOM;
    MClapTextWidget->setText(MClapText);
    MClapTextWidget->setTextSize(9);
    MClapTextWidget->setTextColor( MIP_Color(0.25) /*MIP_COLOR_DARK_GRAY*/);
    MClapTextWidget->setDrawBorder(false);
    MClapTextWidget->setFillBackground(false);
    appendChildWidget(MClapTextWidget);

    // text - plugin name

    MPluginName = new MIP_TextWidget( MIP_DRect(10,0,150,24), "Plugin" );
    MPluginName->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP_LEFT;
    //MPluginName->setText("PluginName");
    MPluginName->setTextSize(24);
    MPluginName->setTextColor(MIP_Color(0)/*MIP_COLOR_BLACK*/);
    MPluginName->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MPluginName->setDrawBorder(false);
    MPluginName->setFillBackground(false);
    appendChildWidget(MPluginName);

    // text - plugin version

    MPluginVersion  = new MIP_TextWidget( MIP_DRect(10,0,100,20), "v0.0.0" );
    MPluginVersion->Layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP_LEFT;
    //MPluginVersion->setText("v0.0.0");
    MPluginVersion->setTextSize(10);
    MPluginVersion->setTextColor(MIP_Color(0.25)/*MIP_COLOR_DARK_GRAY*/);
    MPluginVersion->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MPluginVersion->setDrawBorder(false);
    MPluginVersion->setFillBackground(false);
    appendChildWidget(MPluginVersion);
  }

  //----------

  virtual ~MIP_SAHeaderWidget() {
    if (MMip2Bitmap) delete MMip2Bitmap;
    if (MLogoBitmap) delete MLogoBitmap;
  }

//------------------------------
public:
//------------------------------

  //virtual void setImage(MIP_Drawable* ATarget, uint8_t* ABuffer, uint32_t ASize,MIP_Color ABackground) {
  //  MLogoImage->setImage(ATarget,ABuffer,ASize,ABackground);
  //}

  virtual void setPluginName(const char* AText) {
    MPluginName->setText(AText);
  }

  virtual void setPluginNameColor(MIP_Color AColor) {
    MPluginName->setTextColor(AColor);
 }

  virtual void setPluginVersion(const char* AText) {
    MPluginVersion->setText(AText);
  }

  virtual void setPluginVersionColor(MIP_Color AColor) {
    MPluginVersion->setTextColor(AColor);
  }

//------------------------------
public:
//------------------------------

public:

//  void on_widget_config(MIP_Widget* AOwnerWindow) override {
//    MIP_PRINT;
//    MIP_PanelWidget::on_widget_config(AOwnerWindow);
//  }

//  void on_widget_paint(MIP_Painter* APainter, MIP_DRect ARect, uint32_t AMode) final {
//  }

};

//#endif // 0

//----------------------------------------------------------------------
#endif

