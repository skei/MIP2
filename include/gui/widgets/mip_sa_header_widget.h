#ifndef mip_saheader_widget_included
#define mip_saheader_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_image_widget.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/widgets/mip_text_widget.h"

//----------

class MIP_SAHeaderWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

  MIP_ImageWidget*  MLogoImage      = nullptr;
  MIP_ImageWidget*  MMip2Image      = nullptr;

  MIP_TextWidget*  MClapTextWidget  = nullptr;
  char             MClapText[64]    = {};

  MIP_TextWidget*  MPluginName     = nullptr;
  MIP_TextWidget*  MPluginVersion  = nullptr;

//------------------------------
public:
//------------------------------

  MIP_SAHeaderWidget(MIP_FRect ARect, MIP_Surface* ALogoSurface, MIP_Surface* AMip2Surface/*, const char* AName, const char* AVersion, MIP_Drawable* ATarget*/)
  : MIP_PanelWidget(ARect) {

    setName("MIP_SAHeaderWidget");
    setHint("saheader");
    layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    layout.innerBorder = MIP_FRect(10,10,10,10);
    //layout.spacing = MIP_FPoint(10,0);

    MLogoImage = new MIP_ImageWidget( MIP_FRect(40,40), ALogoSurface );
    MLogoImage->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT_TOP;
    appendWidget(MLogoImage);

    MMip2Image = new MIP_ImageWidget( MIP_FRect(129,34), AMip2Surface );
    MMip2Image->layout.alignment = MIP_WIDGET_ALIGN_RIGHT_TOP;
    appendWidget(MMip2Image);

    sprintf(MClapText,"CLAP v%i.%i.%i",CLAP_VERSION_MAJOR,CLAP_VERSION_MINOR,CLAP_VERSION_REVISION);

    MClapTextWidget = new MIP_TextWidget( MIP_FRect(0,0,129,16),MClapText );
    MClapTextWidget->layout.alignment = MIP_WIDGET_ALIGN_RIGHT_BOTTOM;
    MClapTextWidget->setText(MClapText);
    MClapTextWidget->setTextSize(9);
    MClapTextWidget->setTextColor(MIP_COLOR_DARK_GRAY);
    MClapTextWidget->setFillBackground(false);
    appendWidget(MClapTextWidget);

    MPluginName = new MIP_TextWidget( MIP_FRect(10,0,100,24) );
    MPluginName->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP_LEFT;
    MPluginName->setText("PluginName");
    MPluginName->setTextSize(24);
    MPluginName->setTextColor(MIP_COLOR_BLACK);
    MPluginName->setFillBackground(false);
    appendWidget(MPluginName);

    MPluginVersion  = new MIP_TextWidget( MIP_FRect(10,0,100,20) );
    MPluginVersion->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP_LEFT;
    MPluginVersion->setText("v0.0.0");
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

