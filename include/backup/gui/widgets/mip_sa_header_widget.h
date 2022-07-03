#ifndef mip_saheader_widget_included
#define mip_saheader_widget_included
//----------------------------------------------------------------------

#include "gui/mip_window.h"
#include "gui/widgets/mip_image_widget.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_text_widget.h"

#include "../data/img/sa_logo_40_trans_black.h"
#include "../data/img/mip2_trans_129x34.h"


//----------

class MIP_SAHeaderWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

  MIP_Bitmap*   MLogoBitmap   = nullptr;
  MIP_Bitmap*   MMip2Bitmap   = nullptr;

  MIP_Surface*  MLogoSurface  = nullptr;
  MIP_Surface*  MMip2Surface  = nullptr;

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
  MIP_SAHeaderWidget(MIP_FRect ARect, MIP_Window* AWindow)
  : MIP_PanelWidget(ARect) {

    setName("MIP_SAHeaderWidget");
    setHint("saheader");
    layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    layout.innerBorder = MIP_FRect(10,10,10,10);
    //layout.spacing = MIP_FPoint(10,0);

    MIP_Painter* painter;

    // logo
    MLogoBitmap = new MIP_Bitmap(sa_logo_40_trans_black,sa_logo_40_trans_black_size);
    //knob_bitmap->convertRgbaToBgra();
    //MLogoBitmap->premultAlpha(0x999999);
    MLogoBitmap->premultAlpha(0x808080);
    MLogoBitmap->fillLayer(3,255);
    MLogoSurface = new MIP_Surface(AWindow,MLogoBitmap->getWidth(),MLogoBitmap->getHeight());
    painter = new MIP_Painter(MLogoSurface);
    painter->drawBitmap(0,0,MLogoBitmap);
    painter->flush();
    delete painter;

    // mip2
    MMip2Bitmap = new MIP_Bitmap(mip2_trans_129x34,mip2_trans_129x34_size);
    //knob_bitmap->convertRgbaToBgra();
    MMip2Bitmap->scaleLayer(3,128);
    MMip2Bitmap->premultAlpha(0x808080);
    MMip2Surface = new MIP_Surface(AWindow,MMip2Bitmap->getWidth(),MMip2Bitmap->getHeight());
    painter = new MIP_Painter(MMip2Surface);
    painter->drawBitmap(0,0,MMip2Bitmap);
    painter->flush();
    delete painter;

    //

    MLogoImage = new MIP_ImageWidget( MIP_FRect(40,40), MLogoSurface );
    MLogoImage->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT_TOP;
    appendWidget(MLogoImage);

    MMip2Image = new MIP_ImageWidget( MIP_FRect(129,34), MMip2Surface );
    MMip2Image->layout.alignment = MIP_WIDGET_ALIGN_RIGHT_TOP;
    appendWidget(MMip2Image);

    sprintf(MClapText,"CLAP %i.%i.%i",CLAP_VERSION_MAJOR,CLAP_VERSION_MINOR,CLAP_VERSION_REVISION);

    MClapTextWidget = new MIP_TextWidget( MIP_FRect(0,0,129,16),MClapText );
    MClapTextWidget->layout.alignment = MIP_WIDGET_ALIGN_RIGHT_BOTTOM;
    MClapTextWidget->setText(MClapText);
    MClapTextWidget->setTextSize(9);
    MClapTextWidget->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MClapTextWidget->setTextColor(MIP_COLOR_DARK_GRAY);
    MClapTextWidget->setFillBackground(false);
    appendWidget(MClapTextWidget);

    MPluginName = new MIP_TextWidget( MIP_FRect(10,0,100,24) );
    MPluginName->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP_LEFT;
    MPluginName->setText("PluginName");
    MPluginName->setTextSize(24);
    MPluginName->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MPluginName->setTextColor(MIP_COLOR_BLACK);
    MPluginName->setFillBackground(false);
    appendWidget(MPluginName);

    MPluginVersion  = new MIP_TextWidget( MIP_FRect(10,0,100,20) );
    MPluginVersion->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP_LEFT;
    MPluginVersion->setText("v0.0.0");
    MPluginVersion->setTextSize(10);
    MPluginVersion->setTextAlignment(MIP_TEXT_ALIGN_LEFT);
    MPluginVersion->setTextColor(MIP_COLOR_DARK_GRAY);
    MPluginVersion->setFillBackground(false);
    appendWidget(MPluginVersion);

  }

//  virtual ~MIP_SAHeaderWidget() {
//  }

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

//  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) final {
//  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif

