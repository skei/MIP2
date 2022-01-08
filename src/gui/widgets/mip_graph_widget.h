#ifndef kode_graph_widget_included
#define kode_graph_widget_included
//----------------------------------------------------------------------

/*
  todo:
  - check all drawRect, etc  -> x1,y1,x2,y2 -> x,m,w,h
  - wire endpoint calculations are wrong..
*/

//#include "base/types/kode_list.h"
#include "gui/widgets/kode_panel_widget.h"
#include "gui/kode_widget.h"

struct  KODE_GraphPin;
class   KODE_GraphWire;
class   KODE_GraphModule;
struct  KODE_Graph;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define MODULE_WIDTH    100
#define MODULE_HEIGHT   30

#define PIN_WIDTH       6
#define PIN_XDIST       8

#define PIN_HEIGHT      5
#define PIN_YDIST       8

#define MAX_PINS        16
//#define BASE_HEIGHT     (MODULE_HEIGHT - (PIN_YDIST * 2) - 2)
#define BASE_HEIGHT     (MODULE_HEIGHT - (PIN_YDIST * 2))

//----------

enum KODE_PinType {
  kpt_data    = 0,
  kpt_signal  = 1,
};

//----------

class KODE_GraphWire : public KODE_ListNode {

//------------------------------
public:
//------------------------------

  KODE_GraphModule* inModule  = KODE_NULL;
  int32_t           inPin     = 0;
  KODE_GraphModule* outModule = KODE_NULL;
  int32_t           outPin    = 0;

//------------------------------
public:
//------------------------------

  KODE_GraphWire(KODE_GraphModule* AInModule, int32_t AInPin, KODE_GraphModule* AOutModule, int32_t AOutPin) {
    inModule  = AInModule;
    inPin     = AInPin;
    outModule = AOutModule;
    outPin    = AOutPin;
  }

};

//----------

class KODE_GraphModule : public KODE_ListNode {

//------------------------------
public:
//------------------------------

  const char* name        = "module";
  int32_t     xpos        = 0;
  int32_t     ypos        = 0;
  int32_t     numInputs   = 0;
  int32_t     numOutputs  = 0;
  uint32_t    inputs[MAX_PINS]  = {0};
  uint32_t    outputs[MAX_PINS] = {0};
  bool        selected    = false;
  bool        hovering    = false;

//------------------------------
public:
//------------------------------

  bool inside(int32_t x, int32_t y) {
    if ( (x >= xpos) && (x <  xpos + MODULE_WIDTH)
      && (y >= ypos) && (y <  ypos + MODULE_HEIGHT)) return true;
    return false;
  }

  bool insideBase(int32_t x, int32_t y) {
    if (y < (ypos + /*PIN_HEIGHT*/PIN_YDIST + BASE_HEIGHT)) return true;
    return false;
  }

  int32_t insideInput(int32_t x, int32_t y) {
    if (y < (ypos + PIN_YDIST)) {
      int32_t pin = (x-xpos) / PIN_XDIST;
      if (pin < numInputs) return pin;
    }
    return -1;
  }

  int32_t insideOutput(int32_t x, int32_t y) {
    int32_t pin = (x-xpos) / PIN_XDIST;
    if (pin < numOutputs) return pin;
    return -1;
  }

};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class KODE_GraphWidget
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------

//KODE_Graph*   MGraph      = KODE_NULL;
  KODE_List     MModules;
  KODE_List     MWires;
  KODE_Painter* MPainter    = KODE_NULL;
  KODE_FRect    MPaintRect  = KODE_FRect(0,0);

//------------------------------
protected:
//------------------------------

    KODE_Color MModuleColor             = KODE_COLOR_LIGHT_GRAY;
    KODE_Color MSelectedModuleColor     = KODE_COLOR_DARK_GRAY;

    KODE_Color MSignalPinColor          = KODE_COLOR_LIGHT_GRAY;
    KODE_Color MDataPinColor            = KODE_COLOR_DARK_GRAY;
    KODE_Color MSignalPinHoverColor     = KODE_COLOR_BLACK;
    KODE_Color MDataPinHoverColor       = KODE_COLOR_WHITE;

    KODE_Color MModuleNameColor         = KODE_COLOR_DARK_GRAY;
    KODE_Color MSelectedModuleNameColor = KODE_COLOR_LIGHT_GRAY;

    KODE_Color MSignalWireColor          = KODE_COLOR_LIGHT_GRAY;
    KODE_Color MDataWireColor            = KODE_COLOR_DARK_GRAY;

    bool              MDraggingModules  = false;
    bool              MDraggingWire     = false;
    bool              MDraggingSelect   = false;
    // hovering
    KODE_GraphModule* MHoverModule      = KODE_NULL;
    int32_t           MHoverInput       = -1;
    int32_t           MHoverOutput      = -1;
    int32_t           MMousePrevX       = 0;
    int32_t           MMousePrevY       = 0;
    // wire
    int32_t           MDragWireX1       = 0;
    int32_t           MDragWireY1       = 0;
    int32_t           MDragWireX2       = 0;
    int32_t           MDragWireY2       = 0;
    KODE_GraphModule* MDragWireModule   = KODE_NULL;
    int32_t           MDragWireInput    = -1;
    int32_t           MDragWireOutput   = -1;
    // selection
    int32_t           MDragSelectX1     = 0;
    int32_t           MDragSelectY1     = 0;
    int32_t           MDragSelectX2     = 0;
    int32_t           MDragSelectY2     = 0;


//------------------------------
public:
//------------------------------

  KODE_GraphWidget(KODE_FRect ARect=KODE_FRect())
  : KODE_PanelWidget(ARect) {

    setName("KODE_GraphWidget");
    setHint("graph");
    clearSelection();
//    setFlag(KODE_WIDGET_HOVER);
//    //setFlag(KODE_WIDGET_CLIP);
    setBackgroundColor( KODE_Color(0.55f) );
  }

  //----------

  virtual ~KODE_GraphWidget() {
    #ifndef KODE_NO_AUTODELETE
      deleteModules();
      deleteWires();
    #endif
  }

//------------------------------
public:
//------------------------------

  //----------------------------------------
  // modules
  //----------------------------------------

  public:

    //void appendModule(KODE_GraphModule* AModule) {
    //  MModules.append(AModule);
    //}

    //----------

    void deleteModules(void) {
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_ListNode* next = node->next();
        delete node;
        node = next;
      }
    }

    //----------

    void addModule(KODE_GraphModule* AModule, int32_t AXpos, int32_t AYpos, const char* AName="") {
      AModule->xpos = AXpos;
      AModule->ypos = AYpos;
      AModule->selected = false;
      AModule->hovering = false;
      AModule->name = AName;
      MModules.insertTail(AModule);
    }

    //----------

    void removeModule(KODE_GraphModule* AModule) {
      MModules.removeNode(AModule);
    }

    //----------

    void removeSelectedModules(void) {
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        node = node->next();
        if (module->selected) {
          removeWires(module);
          removeModule(module);
          //delete module; // module.destroy;
        }
      }
    }

  //----------------------------------------
  // wires
  //----------------------------------------

  public:

    void appendWire(KODE_GraphWire* AWire) {
      MWires.insertTail(AWire);
    }

    //----------

    void deleteWires(void) {
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_ListNode* next = node->next();
        delete node;
        node = next;
      }

    }

    //----------

    void addWire(KODE_GraphWire* AWire) {
      MWires.insertTail(AWire);
    }

    //----------

    void removeWire(KODE_GraphWire* AWire) {
      MWires.removeNode(AWire);
    }

    //----------

    // remove all input wires connected to a specific pin on a module

    void removeInputWires(KODE_GraphModule* AModule, int32_t APin) {
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_GraphWire* wire = (KODE_GraphWire*)node;
        node = node->next();
        if ((AModule == wire->inModule) && (APin == wire->inPin))
        /*or ( (AModule = wire.outModule) and (APin = wire.outPin) )*/ {
          removeWire(wire);
          delete wire; // wire.Destroy;
        }
        //node = node.next;
      }
    }

    //----------

    // remove all input wires connected to a specific pin on a module

    void removeOutputWires(KODE_GraphModule* AModule, int32_t APin) {
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_GraphWire* wire = (KODE_GraphWire*)node;
        node = node->next();
        if ((AModule == wire->outModule) && (APin == wire->outPin)  )
        /*or ( (AModule = wire.outModule) and (APin = wire.outPin) )*/ {
          removeWire(wire);
          delete wire;//wire.Destroy;
        }
        //node = node.next;
      }
    }

    //----------

    // remove all wires connected to a module

    void removeWires(KODE_GraphModule* AModule) {
      //node : KListNode;
      //ire : KODE_GraphWire;
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_GraphWire* wire = (KODE_GraphWire*)node;
        node = node->next();
        if ((AModule == wire->inModule) || (AModule == wire->outModule)) {
          removeWire(wire);
          delete wire;//wire.Destroy;
        }
        //node = node.next;
      }
    }

    //----------

    // find wire, input or output

    KODE_GraphWire* findWire(KODE_GraphModule* AModule, int32_t APin) {
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_GraphWire* wire = (KODE_GraphWire*)node;
        if ( ((AModule == wire->inModule ) && (APin == wire->inPin ))
          || ((AModule == wire->outModule) && (APin == wire->outPin)) ) return wire;//System.exit(wire);
        node = node->next();
      }
      return KODE_NULL;
    }

    //----------

    KODE_GraphWire* findWire(KODE_GraphModule* AInModule, int32_t AInPin, KODE_GraphModule* AOutModule, int32_t AOutPin) {
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_GraphWire* wire = (KODE_GraphWire*)node;
        if ( (AInModule  == wire->inModule)
          && (AInPin     == wire->inPin)
          && (AOutModule == wire->outModule)
          && (AOutPin    == wire->outPin) ) return wire;
        node = node->next();
      }
      return KODE_NULL;
    }

  //----------------------------------------
  // selection
  //----------------------------------------

  public:

    bool hasSelection(void) {
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        if (module->selected) return true;
        node = node->next();
      }
      return false;
    }

    //----------

    void clearSelection(void) {
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        module->selected = false;
        node = node->next();
      }
    }

    //----------

    void selectModule(KODE_GraphModule* AModule) {
      AModule->selected = true;
    }

    //----------

    void deselectModule(KODE_GraphModule* AModule) {
      AModule->selected = false;
    }

    //----------

    void toggleModule(KODE_GraphModule* AModule) {
      AModule->selected = AModule->selected ? false : true;
    }

    //----------

    bool isSelected(KODE_GraphModule* AModule) {
      return AModule->selected;
    }

    //----------

    void selectModules(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
      x1 -= getRect().x;
      y1 -= getRect().y;
      x2 -= getRect().x;
      y2 -= getRect().y;
      int32_t temp;
      if (x1 > x2) {
        temp = x1;
        x1 = x2;
        x2 = temp;
      }
      if (y1 > y2) {
        temp = y1;
        y1 = y2;
        y2 = temp;
      }
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        if ( ( module->xpos                <= x2)
          && ((module->xpos+MODULE_WIDTH)  >= x1)
          && ( module->ypos                <= y2)
          && ((module->ypos+MODULE_HEIGHT) >= y1)) {
          module->selected = true;
        }
        node = node->next();
      } // while
    }


    //----------

    void moveSelectedModules(int32_t AXdelta, int32_t AYdelta) {
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        if (module->selected) {
          module->xpos = module->xpos + AXdelta;
          module->ypos = module->ypos + AYdelta;
        }
        node = node->next();
      }
    }

  //----------------------------------------
  // dragging
  //----------------------------------------

  public:

    void startDragInput(KODE_GraphModule* AModule, int32_t APin) {
      MDraggingWire   = true;
      MDragWireInput  = APin;
      MDragWireOutput = -1;
      MDragWireModule = AModule;
      MDragWireX1     = getRect().x + AModule->xpos + (APin * PIN_XDIST) + (PIN_WIDTH / 2);
      MDragWireY1     = getRect().y + AModule->ypos + (PIN_HEIGHT / 2);
      MDragWireX2     = MDragWireX1;
      MDragWireY2     = MDragWireY1;
    }

    //----------

    void startDragOutput(KODE_GraphModule* AModule, int32_t APin) {
      MDraggingWire   = true;
      MDragWireInput  = -1;
      MDragWireOutput = APin;
      MDragWireModule = AModule;
      MDragWireX1     = getRect().x + AModule->xpos + (APin * PIN_XDIST) + (PIN_WIDTH / 2);
      MDragWireY1     = getRect().y + AModule->ypos + (MODULE_HEIGHT-1) - (PIN_HEIGHT / 2);
      MDragWireX2     = MDragWireX1;
      MDragWireY2     = MDragWireY1;
    }

    //----------

    void startDragSelect(int32_t AXpos, int32_t AYpos) {
      MDraggingSelect = true;
      MDragSelectX1   = AXpos;
      MDragSelectY1   = AYpos;
      MDragSelectX2   = AXpos;
      MDragSelectY2   = AYpos;
    }

  //----------------------------------------
  // draw
  //----------------------------------------

  public:

    void drawModule(KODE_GraphModule* AModule) {

      KODE_FRect module_full_rect = KODE_FRect(
        getRect().x + AModule->xpos,
        getRect().y + AModule->ypos,
        MODULE_WIDTH - 1,
        MODULE_HEIGHT - 1
      );

      KODE_FRect module_body_rect = module_full_rect;
      module_body_rect.shrink(0,PIN_YDIST,0,PIN_YDIST);

      KODE_Color color;

      KODE_FRect input_rect = module_body_rect;
      input_rect.y -= PIN_YDIST;
      input_rect.setSize(PIN_WIDTH,PIN_HEIGHT);

      KODE_FRect output_rect = module_body_rect;
      output_rect.y += (output_rect.h + PIN_YDIST - PIN_HEIGHT);
      output_rect.setSize(PIN_WIDTH,PIN_HEIGHT);

      //int32_t x = getRect().x + AModule->xpos;
      //int32_t y = getRect().y + AModule->ypos;
      //int32_t w = MODULE_WIDTH - 1;
      //int32_t h = MODULE_HEIGHT - 1;

      // body

      if (AModule->selected) color = MSelectedModuleColor; else color = MModuleColor;
      MPainter->fillRectangle( module_body_rect, color );

      // inputs

      for (int32_t i=0; i<AModule->numInputs; i++) {
        // fill
        if (AModule->inputs[i] == kpt_signal) color = MSignalPinColor; else color = MDataPinColor;
        MPainter->fillRectangle( input_rect, color );
        // border
        if ((AModule == MHoverModule) && (i == MHoverInput) && (!MDraggingModules)) {
          if (AModule->inputs[i] == kpt_signal) color = MSignalPinHoverColor; else color = MDataPinHoverColor;
          MPainter->drawRectangle( input_rect, color );
        }
        input_rect.x += PIN_XDIST;
      }

      // outputs

      for (int32_t i=0; i<AModule->numOutputs; i++) {
        // fill
        //if (AModule->outputs[i] == kpt_signal)
        //  MPainter->fillRectangle(KODE_FRect(x+(i*PIN_XDIST),h-PIN_HEIGHT+1,PIN_WIDTH-1,PIN_HEIGHT+1),MSignalPinColor);
        //else
        //  MPainter->fillRectangle(KODE_FRect(x+(i*PIN_XDIST),h-PIN_HEIGHT+1,PIN_WIDTH-1,PIN_HEIGHT+1),MDataPinColor);
        if (AModule->outputs[i] == kpt_signal) color = MSignalPinColor; else color = MDataPinColor;
        MPainter->fillRectangle( output_rect, color );
        // border
        if ((AModule == MHoverModule) && (i == MHoverOutput) && (!MDraggingModules)) {
          //if (AModule->outputs[i] == kpt_signal)
          //  MPainter->drawRectangle(KODE_FRect(x+(i*PIN_XDIST),h-PIN_HEIGHT+1,PIN_WIDTH-1,PIN_HEIGHT+1),MSignalPinHoverColor);
          //else if (AModule->outputs[i] == kpt_data)
          //  MPainter->drawRectangle(KODE_FRect(x+(i*PIN_XDIST),h-PIN_HEIGHT+1,PIN_WIDTH-1,PIN_HEIGHT+1),MDataPinHoverColor);
          if (AModule->outputs[i] == kpt_signal) color = MSignalPinHoverColor; else color = MDataPinHoverColor;
          MPainter->drawRectangle( output_rect, color );
        }
        output_rect.x += PIN_XDIST;
      }

      // name

      //if (AModule->selected)
      //  MPainter->drawText(KODE_FRect(x,y,w,h),AModule->name,KODE_TEXT_ALIGN_CENTER,MSelectedModuleNameColor);
      //else
      //  MPainter->drawText(KODE_FRect(x,y,w,h),AModule->name,KODE_TEXT_ALIGN_CENTER,MModuleNameColor);

      if (AModule->selected) color = MSelectedModuleNameColor; else color = MModuleNameColor;
      MPainter->drawText(module_body_rect,AModule->name,KODE_TEXT_ALIGN_CENTER,color);

      // border

      //if (AModule == MHoverModule) {
      //  MPainter->setDrawColor(KLightRed);
      //  MPainter->drawRectangle( x1, y1+PIN_YDIST, x2, y2-PIN_YDIST );
      //}

    }

    //----------

    void drawWire(KODE_GraphWire* AWire) {

      int32_t x1 = getRect().x + AWire->outModule->xpos + (AWire->outPin * PIN_XDIST);// + 2;
      int32_t y1 = getRect().y + AWire->outModule->ypos;// + 31 - 2;

      int32_t x2 = getRect().x + AWire->inModule->xpos + (AWire->inPin * PIN_XDIST);// + 2;
      int32_t y2 = getRect().y + AWire->inModule->ypos;// + 2;


      x1 += (PIN_WIDTH  / 2);
      y1 += (PIN_HEIGHT / 2) + MODULE_HEIGHT - PIN_YDIST;
      x2 += (PIN_WIDTH  / 2);
      y2 += (PIN_HEIGHT / 2);



      //if (AWire->outModule->outputs[AWire->outPin] == kpt_signal)
      //  MPainter->drawLine(x1,y1,x2,y2,MSignalWireColor,1);
      //else
      //  MPainter->drawLine(x1,y1,x2,y2,MDataWireColor,1);

      KODE_Color color;
      if (AWire->outModule->outputs[AWire->outPin] == kpt_signal) color = MSignalWireColor; else color = MDataWireColor;
      MPainter->drawLine(x1,y1,x2,y2,color,1);

    }

    //----------

    void drawModules(void) {
      KODE_ListNode* node = MModules.head();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        drawModule(module);
        node = node->next();
      }
    }

    //----------

    void drawWires(void) {
      KODE_ListNode* node = MWires.head();
      while (node) {
        KODE_GraphWire* wire = (KODE_GraphWire*)node;
        drawWire(wire);
        node = node->next();
      }
    }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    KODE_PanelWidget::on_widget_paint(APainter,ARect,AMode);
    MPainter    = APainter;
    MPaintRect  = ARect;
    //ACanvas.setDrawColor(KLightGrey);
    //ACanvas.drawRectangle(FRect.x,FRect.y,FRect.x2,FRect.y2);
    APainter->pushClip(getRect());
    drawModules();
    drawWires();
    if (MDraggingWire) {
      APainter->drawLine(MDragWireX1,MDragWireY1,MDragWireX2,MDragWireY2,KODE_COLOR_WHITE,1);
    }
    if (MDraggingSelect) {
      float w = MDragSelectX2 - MDragSelectX1;
      float h = MDragSelectY2 - MDragSelectY1;
      APainter->drawRectangle(KODE_FRect(MDragSelectX1,MDragSelectY1,w,h),KODE_COLOR_LIGHT_YELLOW,1);
    }
    APainter->popClip();
  }

  //----------

  // AXpos, AYpos = 'world' coordinates
  // x,y = local coords (graph)

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
      KODE_ListNode* node;
      KODE_GraphModule* module;
      int32_t pin;
      bool changed;
      int32_t x,y;

      x = AXpos - getRect().x;
      y = AYpos - getRect().y;
      changed = false;
      if (AButton == KODE_BUTTON_LEFT) {
        MMousePrevX = AXpos;
        MMousePrevY = AYpos;
        module = KODE_NULL;
        node = MModules.tail();
        while (node) {
          module = (KODE_GraphModule*)node;
          if (module->inside(x,y)) {

            // inputs

            pin = module->insideInput(x,y);
            if ((pin >= 0) && (!MDraggingModules)) {
              startDragInput(module,pin);
              break;
            } else

            // base

            if (module->insideBase(x,y)) {
              if (AState & KODE_KEY_CTRL) {
                toggleModule(module);
                changed = true;
              }
              else {
                if (!module->selected) clearSelection();
                selectModule(module);
                MDraggingModules = true;
                changed = true;
              }
              break;
            } else

            // outputs

            pin = module->insideOutput(x,y);
            if ((pin >= 0) && (!MDraggingModules)) {
              startDragOutput(module,pin);
              break;
            }

          } // inside module
          node = node->prev();
        } // while

        if (!node) {

          if (hasSelection()) {
            if (!(AState & KODE_KEY_CTRL)) {
              clearSelection();
              changed = true;
            }
          }

          if (!MDraggingSelect) {
            if (!(AState & KODE_KEY_CTRL))
              clearSelection();
            startDragSelect(AXpos,AYpos);
          }

        } // node
      }
      if (changed) do_widget_redraw(this,getRect(),0);
      KODE_PanelWidget::on_widget_mouseClick(AXpos,AYpos,AButton,AState);
  }

  //----------

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
      KODE_GraphWire* wire;
      bool changed;

      //AXpos -= FRect.x;
      //AYpos -= FRect.y;
      changed = false;
      if (AButton == KODE_BUTTON_LEFT) {

        if (MDraggingModules) {
          MDraggingModules = false;
          changed = true;
        }

        if (MDraggingWire) {
          MDraggingWire = false;
          changed = true;

          if (MDragWireModule) {

            if (MDragWireInput >= 0) {
              if (MHoverOutput >= 0) {
                wire = findWire(MDragWireModule,MDragWireInput,MHoverModule,MHoverOutput);
                if (wire) {
                  removeWire(wire);
                  delete wire;
                }
                else {
                  addWire( new KODE_GraphWire(MDragWireModule,MDragWireInput, MHoverModule, MHoverOutput) ); // in -> out
                }
              }
              else {
                if (MDragWireInput != MHoverInput)
                removeInputWires(MDragWireModule,MDragWireInput);
              }

            } // input

            if (MDragWireOutput >= 0) {
              if (MHoverInput >= 0) {
                wire = findWire(MHoverModule,MHoverInput,MDragWireModule,MDragWireOutput);
                if (wire) {
                  removeWire(wire);
                  delete wire;
                }
                else {
                  addWire( new KODE_GraphWire(MHoverModule,MHoverInput,MDragWireModule,MDragWireOutput) ); // in -> out
                }
              } // output
              else {
                if (MDragWireOutput != MHoverOutput)
                  removeOutputWires(MDragWireModule,MDragWireOutput);
              }
            }

          } // dragWireModule
        } // dragging wire

        if (MDraggingSelect) {
          MDraggingSelect = false;
          changed = true;
        }

      }

      if (changed) do_widget_redraw(this,getRect(),0);
      //inherited;
      KODE_PanelWidget::on_widget_mouseRelease(AXpos,AYpos,AButton,AState);
  }

  //----------

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) override {
      int32_t x = AXpos - getRect().x;
      int32_t y = AYpos - getRect().y;
      bool changed = false;
      KODE_ListNode* node = (KODE_GraphModule*)MModules.tail();
      while (node) {
        KODE_GraphModule* module = (KODE_GraphModule*)node;
        if (module->inside(x,y)) {

          // inputs

          int32_t pin = module->insideInput(x,y);
          if (pin >= 0) {
            MHoverModule = module; // used when drawing modules
            if ((MHoverInput != pin) && (!MDraggingSelect)) {
              MHoverInput = pin;
              MHoverOutput = -1;
              changed = true;
            }
            break;
          } else

          // base

          if (module->insideBase(x,y)) {
            MHoverModule = module;
            if (MHoverInput != -1) {
              MHoverInput = -1;
              changed = true;
            }
            if (MHoverOutput != -1) {
              MHoverOutput = -1;
              changed = true;
            }
            break;
          } else

          // outputs

          pin = module->insideOutput(x,y);
          if (pin >= 0) {
            MHoverModule = module;
            if ((MHoverOutput != pin) && !MDraggingSelect) {
              MHoverInput = -1;
              MHoverOutput = pin;
              changed = true;
            }
            break;
          }

        }
        node = node->prev();
      } // while

      if (!node) {
        MHoverModule = KODE_NULL;
        if (MHoverInput != -1) {
          MHoverInput = -1;
          changed = true;
        }
        if (MHoverOutput != -1) {
          MHoverOutput = -1;
          changed = true;
        }
      }

      if (MDraggingWire) {
        MDragWireX2 = AXpos;
        MDragWireY2 = AYpos;
        changed = true;
      } else

      if (MDraggingModules) {
        moveSelectedModules(AXpos-MMousePrevX,AYpos-MMousePrevY);
        changed = true;
      } else

      if (MDraggingSelect) {
        MDragSelectX2 = AXpos;
        MDragSelectY2 = AYpos;
        clearSelection();
        selectModules(MDragSelectX1,MDragSelectY1,MDragSelectX2,MDragSelectY2);
        changed = true;
      }

      if (changed) do_widget_redraw(this,getRect(),0);
      MMousePrevX = AXpos;
      MMousePrevY = AYpos;

      KODE_PanelWidget::on_widget_mouseMove(AXpos,AYpos,AState);
  }

  //----------

//  void on_widget_keyPress(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_keyRelease(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

  //------------------------------
  public:
  //------------------------------

//  void do_widget_realign(KODE_Widget* ASender, bool ARecursive=true) override {
//    //KODE_PRINT;
//    //alignChildren(ARecursive);
//    findContentRect();
//  }

};

//----------------------------------------------------------------------

#undef PIN_WIDTH
#undef PIN_HEIGHT
#undef MODULE_WIDTH
#undef MODULE_HEIGHT
#undef PIN_XDIST
#undef PIN_YDIST
#undef MAX_PINS
#undef BASE_HEIGHT

//----------------------------------------------------------------------
#endif
