#ifndef mip_saheader_widget_included
#define mip_saheader_widget_included
//----------------------------------------------------------------------

#include "gfx/mip_bitmap.h"

#include "gui/mip_window.h"
#include "gui/mip_surface.h"
#include "gui/widgets/mip_image_widget.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_text_widget.h"

#include "../data/img/sa_60x60.h"
#include "../data/img/mip2_100x60.h"

//----------

//#if 0

class MIP_SAHeaderWidget
: public MIP_PanelWidget {


//------------------------------
private:
//------------------------------

  MIP_Bitmap*       MLogoBitmap     = nullptr;
  MIP_Bitmap*       MMip2Bitmap     = nullptr;
  MIP_ImageWidget*  MLogoImage      = nullptr;
  MIP_ImageWidget*  MMip2Image      = nullptr;
  MIP_TextWidget*   MClapTextWidget = nullptr;
  char              MClapText[64]   = {};
  MIP_TextWidget*   MPluginName     = nullptr;
  MIP_TextWidget*   MPluginVersion  = nullptr;

//------------------------------
public:
//------------------------------

  //MIP_SAHeaderWidget(MIP_FRect ARect, MIP_Surface* ALogoSurface, MIP_Surface* AMip2Surface)
  MIP_SAHeaderWidget(MIP_DRect ARect/*, MIP_Window* AWindow*/)
  : MIP_PanelWidget(ARect) {

    setWidgetName("MIP_SAHeaderWidget");

    // bitmaps

    MLogoBitmap = new MIP_Bitmap(sa_60x60,sa_60x60_size);
    MLogoBitmap->scaleLayer(3,128);
    MLogoBitmap->premultAlpha(0x808080);
    MLogoBitmap->fillLayer(3,255);

    MMip2Bitmap = new MIP_Bitmap(mip2_100x60,mip2_100x60_size);
    MMip2Bitmap->scaleLayer(3,128);
    MMip2Bitmap->premultAlpha(0x808080);
    MMip2Bitmap->fillLayer(3,255);

    // SA

    MLogoImage = new MIP_ImageWidget( MIP_DRect(10,10,60,60), MLogoBitmap );
    //MLogoImage->Layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT_TOP;
    MLogoImage->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    appendChildWidget(MLogoImage);

    // botage

    MPluginName = new MIP_TextWidget( MIP_DRect(80,10,160,60), "" );
    MPluginName->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    MPluginName->setTextSize(-0.7);
    MPluginName->setTextFont("Header");
    MPluginName->setTextColor(MIP_Color(0.75));
    MPluginName->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MPluginName->setDrawBorder(false);
    MPluginName->setFillBackground(false);
    appendChildWidget(MPluginName);

    // plugin version

    MPluginVersion  = new MIP_TextWidget( MIP_DRect(80,56,200,10), "" );
    MPluginVersion->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    MPluginVersion->setTextSize(-1);
    MPluginVersion->setTextColor(MIP_Color(0.25) );
    MPluginVersion->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MPluginVersion->setDrawBorder(false);
    MPluginVersion->setFillBackground(false);
    appendChildWidget(MPluginVersion);

    // mip2

    MMip2Image = new MIP_ImageWidget( MIP_DRect((ARect.w - 110),10,100,60), MMip2Bitmap );
    MMip2Image->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    appendChildWidget(MMip2Image);

    MIP_PRINT;

    // clap version

    sprintf(MClapText,"CLAP v%i.%i.%i",CLAP_VERSION_MAJOR,CLAP_VERSION_MINOR,CLAP_VERSION_REVISION);
    MClapTextWidget = new MIP_TextWidget( MIP_DRect((ARect.w - 110),51,100,10),MClapText );
    MClapTextWidget->Layout.rectMode = MIP_WIDGET_RECT_MODE_INITIAL_RATIO;
    MClapTextWidget->setText(MClapText);
    MClapTextWidget->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MClapTextWidget->setTextSize(-0.9);
    MClapTextWidget->setTextColor( MIP_Color(0.25) );
    MClapTextWidget->setDrawBorder(false);
    MClapTextWidget->setFillBackground(false);
    appendChildWidget(MClapTextWidget);

  }

  //----------

  virtual ~MIP_SAHeaderWidget() {
    if (MMip2Bitmap) delete MMip2Bitmap;
    if (MLogoBitmap) delete MLogoBitmap;
  }

//------------------------------
public:
//------------------------------

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

};

//#endif // 0

//----------------------------------------------------------------------
#endif

