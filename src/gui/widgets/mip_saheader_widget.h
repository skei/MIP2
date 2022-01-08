#ifndef kode_saheader_widget_included
#define kode_saheader_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_panel_widget.h"

//----------

class KODE_SAHeaderWidget
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------

  KODE_ImageWidget* MLogoImage      = KODE_NULL;
  KODE_TextWidget*  MPluginName     = KODE_NULL;
  KODE_TextWidget*  MPluginVersion  = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_SAHeaderWidget(KODE_FRect ARect/*, const char* AName, const char* AVersion, KODE_Drawable* ATarget*/)
  : KODE_PanelWidget(ARect) {

    setName("KODE_SAHeaderWidget");
    setHint("saheader");
    layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP;
    layout.innerBorder = KODE_FRect(10,10,10,10);
    layout.spacing = KODE_FPoint(10,0);

    MLogoImage = new KODE_ImageWidget( KODE_FRect(40,40) );
    MLogoImage->layout.alignment = KODE_WIDGET_ALIGN_FILL_LEFT_TOP;
    appendWidget(MLogoImage);

    MPluginName = new KODE_TextWidget( KODE_FRect(200,16) );
    MPluginName->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP_LEFT;
    MPluginName->setText("plugin");
    MPluginName->setTextColor(KODE_COLOR_BLACK);
    MPluginName->setFillBackground(false);
    appendWidget(MPluginName);

    MPluginVersion  = new KODE_TextWidget( KODE_FRect(200,16) );
    MPluginVersion->layout.alignment = KODE_WIDGET_ALIGN_FILL_TOP_LEFT;
    MPluginVersion->setText("v0.0.1");
    MPluginVersion->setTextColor(KODE_COLOR_DARK_GRAY);
    MPluginVersion->setFillBackground(false);
    appendWidget(MPluginVersion);

  }

//  virtual ~KODE_SAHeaderWidget() {
//  }

//------------------------------
public:
//------------------------------

  virtual void setImage(KODE_Drawable* ATarget, uint8_t* ABuffer, uint32_t ASize,KODE_Color ABackground) {
    MLogoImage->setImage(ATarget,ABuffer,ASize,ABackground);
  }

  virtual void setPluginName(const char* AText) {
    MPluginName->setText(AText);
  }

  virtual void setPluginNameColor(KODE_Color AColor) {
    MPluginName->setTextColor(AColor);
  }

  virtual void setPluginVersion(const char* AText) {
    MPluginVersion->setText(AText);
  }

  virtual void setPluginVersionColor(KODE_Color AColor) {
    MPluginVersion->setTextColor(AColor);
  }

//  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) final {
//  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif

