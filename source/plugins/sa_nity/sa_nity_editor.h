#ifndef sa_nity_editor_included
#define sa_nity_editor_included
//----------------------------------------------------------------------

#include "mip.h"
#include "plugin/mip_editor.h"

//----------------------------------------------------------------------

#define EDITOR_WIDTH  640
#define EDITOR_HEIGHT 480

//----------------------------------------------------------------------

class sa_nity_editor
: public MIP_Editor {

//------------------------------
public:
//------------------------------

  sa_nity_editor(MIP_EditorListener* AListener, uint32_t AWidth, uint32_t AHeight)
  : MIP_Editor(AListener,AWidth,AHeight) {
    init();
  }

  //----------

  virtual ~sa_nity_editor() {
    cleanup();
  }

//------------------------------
public:
//------------------------------


  void init() {

    MIP_Window* window = getWindow();

    window->setWindowFillBackground(false);

    // background

    MIP_PanelWidget* background = new MIP_PanelWidget(0);
    window->appendChildWidget(background);
    background->Layout.alignment = MIP_WIDGET_ALIGN_FILL_PARENT;
    background->setDrawBorder(false);
    background->setFillBackground(true);
    background->setBackgroundColor(0.45);

    // aspect

    MIP_PanelWidget* aspect = new MIP_PanelWidget(0);
    background->appendChildWidget(aspect);
    aspect->Layout.alignment = MIP_WIDGET_ALIGN_FILL_PARENT;
    aspect->Layout.aspectRatio = (double)EDITOR_WIDTH / (double)EDITOR_HEIGHT;
    {

      // left panel

      MIP_PanelWidget* left_panel = new MIP_PanelWidget(150);
      aspect->appendChildWidget(left_panel);
      left_panel->Layout.alignment = MIP_WIDGET_ALIGN_FILL_LEFT;

    }

  }

  //----------

  void cleanup() {
  }

};

//----------------------------------------------------------------------

#undef EDITOR_WIDTH
#undef EDITOR_HEIGHT

#undef H1
#undef H5
#undef H10
#undef H20


//----------------------------------------------------------------------
#endif
