#ifndef mip_saheader_widget_included
#define mip_saheader_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/mip_panel_widget.h"

//----------

class MIP_SAHeaderWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

  MIP_ImageWidget* MLogoImage      = nullptr;
  MIP_TextWidget*  MPluginName     = nullptr;
  MIP_TextWidget*  MPluginVersion  = nullptr;

//------------------------------
public:
//------------------------------

  MIP_SAHeaderWidget(MIP_FRect ARect/*, const char* AName, const char* AVersion, MIP_Drawable* ATarget*/)
  : MIP_PanelWidget(ARect) {

    setName("MIP_SAHeaderWidget");
    setHint("saheader");
    layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP;
    layout.innerBorder = MIP_FRect(10,10,10,10);
    layout.spacing = MIP_FPoint(10,0);

    MLogoImage = new MIP_ImageWidget( MIP_FRect(40,40) );
    MLogoImage->layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT_TOP;
    appendWidget(MLogoImage);

    MPluginName = new MIP_TextWidget( MIP_FRect(200,16) );
    MPluginName->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP_LEFT;
    MPluginName->setText("plugin");
    MPluginName->setTextColor(MIP_COLOR_BLACK);
    MPluginName->setFillBackground(false);
    appendWidget(MPluginName);

    MPluginVersion  = new MIP_TextWidget( MIP_FRect(200,16) );
    MPluginVersion->layout.alignment = MIP_WIDGET_ALIGN_FILL_TOP_LEFT;
    MPluginVersion->setText("v0.0.1");
    MPluginVersion->setTextColor(MIP_COLOR_DARK_GRAY);
    MPluginVersion->setFillBackground(false);
    appendWidget(MPluginVersion);

  }

//  virtual ~MIP_SAHeaderWidget() {
//  }

//------------------------------
public:
//------------------------------

  virtual void setImage(MIP_Drawable* ATarget, uint8_t* ABuffer, uint32_t ASize,MIP_Color ABackground) {
//    MLogoImage->setImage(ATarget,ABuffer,ASize,ABackground);
  }

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

