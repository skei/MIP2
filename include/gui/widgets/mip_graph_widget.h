#ifndef mip_graph_widget_included
#define mip_graph_widget_included
//----------------------------------------------------------------------

/*
  todo:
  - check all drawRect, etc  -> x1,y1,x2,y2 -> x,m,w,h
  - wire endpoint calculations are wrong..
*/

#include "base/types/mip_list.h"
//#include "gui/mip_widgets.h"
#include "gui/widgets/mip_panel_widget.h"
#include "gui/mip_widget.h"

struct  MIP_GraphPin;
class   MIP_GraphWire;
class   MIP_GraphModule;
struct  MIP_Graph;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define MODULE_WIDTH    120
#define MODULE_HEIGHT   40

#define PIN_WIDTH       10
#define PIN_XDIST       12

#define PIN_HEIGHT      7
#define PIN_YDIST       9

#define MAX_PINS        16
//#define BASE_HEIGHT     (MODULE_HEIGHT - (PIN_YDIST * 2) - 2)
#define BASE_HEIGHT     (MODULE_HEIGHT - (PIN_YDIST * 2))

//----------

enum MIP_PinType {
  MIP_PIN_DATA    = 0,
  MIP_PIN_SIGNAL  = 1,
};

//----------

class MIP_GraphWire : public MIP_ListNode {

//------------------------------
public:
//------------------------------

  MIP_GraphModule* inModule  = nullptr;
  int32_t           inPin     = 0;
  MIP_GraphModule* outModule = nullptr;
  int32_t           outPin    = 0;

//------------------------------
public:
//------------------------------

  MIP_GraphWire(MIP_GraphModule* AInModule, int32_t AInPin, MIP_GraphModule* AOutModule, int32_t AOutPin) {
    inModule  = AInModule;
    inPin     = AInPin;
    outModule = AOutModule;
    outPin    = AOutPin;
  }

};

//----------

class MIP_GraphModule : public MIP_ListNode {

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

class MIP_GraphWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

//MIP_Graph*   MGraph      = nullptr;
  MIP_List     MModules;
  MIP_List     MWires;
  MIP_Painter* MPainter    = nullptr;
  MIP_DRect    MPaintRect  = MIP_DRect(0,0);

//------------------------------
protected:
//------------------------------

    MIP_Color MModuleColor             = MIP_Color(0.75);//MIP_COLOR_LIGHT_GRAY;
    MIP_Color MSelectedModuleColor     = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;

    MIP_Color MModuleBorder            = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;
    MIP_Color MSelectedModuleBorder    = MIP_Color(0.75);//MIP_COLOR_LIGHT_GRAY;

    MIP_Color MSignalPinColor          = MIP_Color(0.75);//MIP_COLOR_LIGHT_GRAY;
    MIP_Color MDataPinColor            = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;
    MIP_Color MSignalPinHoverColor     = MIP_Color(0);//MIP_COLOR_BLACK;
    MIP_Color MDataPinHoverColor       = MIP_Color(1);//MIP_COLOR_WHITE;

    MIP_Color MModuleNameColor         = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;
    MIP_Color MSelectedModuleNameColor = MIP_Color(0.75);//MIP_COLOR_LIGHT_GRAY;

    MIP_Color MSignalWireColor          = MIP_Color(0.75);//MIP_COLOR_LIGHT_GRAY;
    MIP_Color MDataWireColor            = MIP_Color(0.25);//MIP_COLOR_DARK_GRAY;

    bool              MDraggingModules  = false;
    bool              MDraggingWire     = false;
    bool              MDraggingSelect   = false;
    // hovering
    MIP_GraphModule* MHoverModule      = nullptr;
    int32_t           MHoverInput       = -1;
    int32_t           MHoverOutput      = -1;
    int32_t           MMousePrevX       = 0;
    int32_t           MMousePrevY       = 0;
    // wire
    int32_t           MDragWireX1       = 0;
    int32_t           MDragWireY1       = 0;
    int32_t           MDragWireX2       = 0;
    int32_t           MDragWireY2       = 0;
    MIP_GraphModule* MDragWireModule   = nullptr;
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

  MIP_GraphWidget(MIP_DRect ARect=MIP_DRect())
  : MIP_PanelWidget(ARect) {

    setWidgetName("MIP_GraphWidget");
    //setHint("graph");
    clearSelection();
//    setFlag(MIP_WIDGET_HOVER);
//    //setFlag(MIP_WIDGET_CLIP);
    setBackgroundColor( MIP_Color(0.55f) );

  }

  //----------

  virtual ~MIP_GraphWidget() {
    #ifndef MIP_NO_AUTODELETE
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

    //void appendModule(MIP_GraphModule* AModule) {
    //  MModules.append(AModule);
    //}

    //----------

    void deleteModules(void) {
      MIP_ListNode* node = MModules.head();
      while (node) {
        MIP_ListNode* next = node->next();
        delete node;
        node = next;
      }
    }

    //----------

    void addModule(MIP_GraphModule* AModule, int32_t AXpos, int32_t AYpos, const char* AName="") {
      AModule->xpos = AXpos;
      AModule->ypos = AYpos;
      AModule->selected = false;
      AModule->hovering = false;
      AModule->name = AName;
      MModules.insertTail(AModule);
    }

    //----------

    void removeModule(MIP_GraphModule* AModule) {
      MModules.removeNode(AModule);
    }

    //----------

    void removeSelectedModules(void) {
      MIP_ListNode* node = MModules.head();
      while (node) {
        MIP_GraphModule* module = (MIP_GraphModule*)node;
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

    void appendWire(MIP_GraphWire* AWire) {
      MWires.insertTail(AWire);
    }

    //----------

    void deleteWires(void) {
      MIP_ListNode* node = MWires.head();
      while (node) {
        MIP_ListNode* next = node->next();
        delete node;
        node = next;
      }

    }

    //----------

    void addWire(MIP_GraphWire* AWire) {
      MWires.insertTail(AWire);
    }

    //----------

    void removeWire(MIP_GraphWire* AWire) {
      MWires.removeNode(AWire);
    }

    //----------

    // remove all input wires connected to a specific pin on a module

    void removeInputWires(MIP_GraphModule* AModule, int32_t APin) {
      MIP_ListNode* node = MWires.head();
      while (node) {
        MIP_GraphWire* wire = (MIP_GraphWire*)node;
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

    void removeOutputWires(MIP_GraphModule* AModule, int32_t APin) {
      MIP_ListNode* node = MWires.head();
      while (node) {
        MIP_GraphWire* wire = (MIP_GraphWire*)node;
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

    void removeWires(MIP_GraphModule* AModule) {
      //node : KListNode;
      //ire : MIP_GraphWire;
      MIP_ListNode* node = MWires.head();
      while (node) {
        MIP_GraphWire* wire = (MIP_GraphWire*)node;
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

    MIP_GraphWire* findWire(MIP_GraphModule* AModule, int32_t APin) {
      MIP_ListNode* node = MWires.head();
      while (node) {
        MIP_GraphWire* wire = (MIP_GraphWire*)node;
        if ( ((AModule == wire->inModule ) && (APin == wire->inPin ))
          || ((AModule == wire->outModule) && (APin == wire->outPin)) ) return wire;//System.exit(wire);
        node = node->next();
      }
      return nullptr;
    }

    //----------

    MIP_GraphWire* findWire(MIP_GraphModule* AInModule, int32_t AInPin, MIP_GraphModule* AOutModule, int32_t AOutPin) {
      MIP_ListNode* node = MWires.head();
      while (node) {
        MIP_GraphWire* wire = (MIP_GraphWire*)node;
        if ( (AInModule  == wire->inModule)
          && (AInPin     == wire->inPin)
          && (AOutModule == wire->outModule)
          && (AOutPin    == wire->outPin) ) return wire;
        node = node->next();
      }
      return nullptr;
    }

  //----------------------------------------
  // selection
  //----------------------------------------

  public:

    bool hasSelection(void) {
      MIP_ListNode* node = MModules.head();
      while (node) {
        MIP_GraphModule* module = (MIP_GraphModule*)node;
        if (module->selected) return true;
        node = node->next();
      }
      return false;
    }

    //----------

    void clearSelection(void) {
      MIP_ListNode* node = MModules.head();
      while (node) {
        MIP_GraphModule* module = (MIP_GraphModule*)node;
        module->selected = false;
        node = node->next();
      }
    }

    //----------

    void selectModule(MIP_GraphModule* AModule) {
      AModule->selected = true;
    }

    //----------

    void deselectModule(MIP_GraphModule* AModule) {
      AModule->selected = false;
    }

    //----------

    void toggleModule(MIP_GraphModule* AModule) {
      AModule->selected = AModule->selected ? false : true;
    }

    //----------

    bool isSelected(MIP_GraphModule* AModule) {
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
      MIP_ListNode* node = MModules.head();
      while (node) {
        MIP_GraphModule* module = (MIP_GraphModule*)node;
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
      MIP_ListNode* node = MModules.head();
      while (node) {
        MIP_GraphModule* module = (MIP_GraphModule*)node;
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

    void startDragInput(MIP_GraphModule* AModule, int32_t APin) {
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

    void startDragOutput(MIP_GraphModule* AModule, int32_t APin) {
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

    void drawModule(MIP_GraphModule* AModule) {

      MIP_DRect module_full_rect = MIP_DRect(
        getRect().x + AModule->xpos,
        getRect().y + AModule->ypos,
        MODULE_WIDTH - 1,
        MODULE_HEIGHT - 1
      );

      MIP_DRect module_body_rect = module_full_rect;
      module_body_rect.shrink(0,PIN_YDIST,0,PIN_YDIST);

      MIP_Color color;

      MIP_DRect input_rect = module_body_rect;
      input_rect.y -= PIN_YDIST;
      input_rect.setSize(PIN_WIDTH,PIN_HEIGHT);

      MIP_DRect output_rect = module_body_rect;
      output_rect.y += (output_rect.h + PIN_YDIST - PIN_HEIGHT);
      output_rect.setSize(PIN_WIDTH,PIN_HEIGHT);

      //int32_t x = getRect().x + AModule->xpos;
      //int32_t y = getRect().y + AModule->ypos;
      //int32_t w = MODULE_WIDTH - 1;
      //int32_t h = MODULE_HEIGHT - 1;

      // body

      if (AModule->selected) color = MSelectedModuleColor; else color = MModuleColor;
      //MPainter->fillRectangle( module_body_rect, color );

      MPainter->beginPath();
      MPainter->rect(module_body_rect.x,module_body_rect.y,module_body_rect.w,module_body_rect.h);
      MPainter->fillColor(color);
      MPainter->fill();

      // inputs

      for (int32_t i=0; i<AModule->numInputs; i++) {
        // fill
        if (AModule->inputs[i] == MIP_PIN_SIGNAL) color = MSignalPinColor; else color = MDataPinColor;
        //MPainter->fillRectangle( input_rect, color );

        MPainter->beginPath();
        MPainter->rect(input_rect.x,input_rect.y,input_rect.w,input_rect.h);
        MPainter->fillColor(color);
        MPainter->fill();

        // border
        if ((AModule == MHoverModule) && (i == MHoverInput) && (!MDraggingModules)) {
          if (AModule->inputs[i] == MIP_PIN_SIGNAL) color = MSignalPinHoverColor; else color = MDataPinHoverColor;
          //MPainter->drawRectangle( input_rect, color );

        MPainter->beginPath();
          MPainter->rect(input_rect.x,input_rect.y,input_rect.w,input_rect.h);
          MPainter->strokeWidth(1);
          MPainter->strokeColor(color);
          MPainter->stroke();

        }
        input_rect.x += PIN_XDIST;
      }

      // outputs

      for (int32_t i=0; i<AModule->numOutputs; i++) {
        // fill
        //if (AModule->outputs[i] == kpt_signal)
        //  MPainter->fillRectangle(MIP_DRect(x+(i*PIN_XDIST),h-PIN_HEIGHT+1,PIN_WIDTH-1,PIN_HEIGHT+1),MSignalPinColor);
        //else
        //  MPainter->fillRectangle(MIP_DRect(x+(i*PIN_XDIST),h-PIN_HEIGHT+1,PIN_WIDTH-1,PIN_HEIGHT+1),MDataPinColor);
        if (AModule->outputs[i] == MIP_PIN_SIGNAL) color = MSignalPinColor; else color = MDataPinColor;
        //MPainter->fillRectangle( output_rect, color );

        MPainter->beginPath();
        MPainter->rect(output_rect.x,output_rect.y,output_rect.w,output_rect.h);
        MPainter->fillColor(color);
        MPainter->fill();

        // border
        if ((AModule == MHoverModule) && (i == MHoverOutput) && (!MDraggingModules)) {
          //if (AModule->outputs[i] == kpt_signal)
          //  MPainter->drawRectangle(MIP_DRect(x+(i*PIN_XDIST),h-PIN_HEIGHT+1,PIN_WIDTH-1,PIN_HEIGHT+1),MSignalPinHoverColor);
          //else if (AModule->outputs[i] == kpt_data)
          //  MPainter->drawRectangle(MIP_DRect(x+(i*PIN_XDIST),h-PIN_HEIGHT+1,PIN_WIDTH-1,PIN_HEIGHT+1),MDataPinHoverColor);
          if (AModule->outputs[i] == MIP_PIN_SIGNAL) color = MSignalPinHoverColor; else color = MDataPinHoverColor;
          //MPainter->drawRectangle( output_rect, color );

          MPainter->beginPath();
          MPainter->rect(output_rect.x,output_rect.y,output_rect.w,output_rect.h);
          MPainter->strokeColor(color);
          MPainter->strokeWidth(1);
          MPainter->stroke();

        }
        output_rect.x += PIN_XDIST;
      }

      // name

      //if (AModule->selected)
      //  MPainter->drawText(MIP_DRect(x,y,w,h),AModule->name,MIP_TEXT_ALIGN_CENTER,MSelectedModuleNameColor);
      //else
      //  MPainter->drawText(MIP_DRect(x,y,w,h),AModule->name,MIP_TEXT_ALIGN_CENTER,MModuleNameColor);

      if (AModule->selected) color = MSelectedModuleNameColor; else color = MModuleNameColor;

      //MPainter->setColor(color);
      //MPainter->drawText(module_body_rect,AModule->name,MIP_TEXT_ALIGN_CENTER);

      MPainter->fontSize(12);
      MPainter->drawTextBox(module_body_rect, AModule->name, MIP_TEXT_ALIGN_CENTER,color);


      // border

      /*if (AModule->selected) color = MSelectedModuleBorder;
      else*/
      color = MModuleBorder;
      //MPainter->drawRectangle( module_body_rect, color,1 );

      MPainter->beginPath();
      MPainter->rect(module_body_rect.x,module_body_rect.y,module_body_rect.w,module_body_rect.h);
      MPainter->strokeColor(color);
      MPainter->strokeWidth(1);
      MPainter->stroke();
    }

    //----------

    void drawWire(MIP_GraphWire* AWire) {

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

      MIP_Color color;
      if (AWire->outModule->outputs[AWire->outPin] == MIP_PIN_SIGNAL) color = MSignalWireColor;
      else color = MDataWireColor;
      //MPainter->drawLine(x1,y1,x2,y2,color,2);

      MPainter->beginPath();
      MPainter->moveTo(x1,y1);
      MPainter->lineTo(x2,y2);
      MPainter->strokeColor(color);
      MPainter->strokeWidth(2);
      MPainter->stroke();
    }

    //----------

    void drawModules(void) {
      MIP_ListNode* node = MModules.head();
      while (node) {
        MIP_GraphModule* module = (MIP_GraphModule*)node;
        drawModule(module);
        node = node->next();
      }
    }

    //----------

    void drawWires(void) {
      MIP_ListNode* node = MWires.head();
      while (node) {
        MIP_GraphWire* wire = (MIP_GraphWire*)node;
        drawWire(wire);
        node = node->next();
      }
    }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    MIP_Painter* painter = AContext->painter;
    fillBackground(AContext);
    //paintWidgets(APainter,ARect,AMode);
    MPainter = AContext->painter;
    MPaintRect = AContext->updateRect;
    //ACanvas.setDrawColor(KLightGrey);
    //ACanvas.drawRectangle(FRect.x,FRect.y,FRect.x2,FRect.y2);
//    painter->pushClip(getRect());
    drawModules();
    drawWires();
    if (MDraggingWire) {
      //APainter->drawLine(MDragWireX1,MDragWireY1,MDragWireX2,MDragWireY2,MIP_COLOR_BRIGHT_YELLOW,1);

      painter->beginPath();
      painter->moveTo(MDragWireX1,MDragWireY1);
      painter->lineTo(MDragWireX2,MDragWireY2);
      painter->strokeColor( MIP_Color(0) /*MIP_COLOR_BLACK*/ );
      painter->strokeWidth(0.5);
      painter->stroke();

    }
    if (MDraggingSelect) {
      float w = MDragSelectX2 - MDragSelectX1;
      float h = MDragSelectY2 - MDragSelectY1;
      //APainter->drawRectangle(MIP_DRect(MDragSelectX1,MDragSelectY1,w,h),MIP_COLOR_BRIGHT_YELLOW,1);

      painter->beginPath();
      painter->rect(MDragSelectX1,MDragSelectY1,w,h);
      painter->strokeColor( MIP_Color(1) /*MIP_COLOR_WHITE*/ );
      painter->strokeWidth(0.5);
      painter->stroke();

    }
    drawBorder(AContext);
//    painter->popClip();
  }

  //----------

  // AXpos, AYpos = 'world' coordinates
  // x,y = local coords (graph)

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
      MIP_ListNode* node;
      MIP_GraphModule* module;
      int32_t pin;
      bool changed;
      int32_t x,y;

      x = AXpos - getRect().x;
      y = AYpos - getRect().y;
      changed = false;
      if (AButton == MIP_BUTTON_LEFT) {
        MMousePrevX = AXpos;
        MMousePrevY = AYpos;
        module = nullptr;
        node = MModules.tail();
        while (node) {
          module = (MIP_GraphModule*)node;
          if (module->inside(x,y)) {

            // inputs

            pin = module->insideInput(x,y);
            if ((pin >= 0) && (!MDraggingModules)) {
              startDragInput(module,pin);
              break;
            } else

            // base

            if (module->insideBase(x,y)) {
              if (AState & MIP_KEY_CTRL) {
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
            if (!(AState & MIP_KEY_CTRL)) {
              clearSelection();
              changed = true;
            }
          }

          if (!MDraggingSelect) {
            if (!(AState & MIP_KEY_CTRL))
              clearSelection();
            startDragSelect(AXpos,AYpos);
          }

        } // node
      }
      if (changed) do_widget_redraw(this,0);
      MIP_PanelWidget::on_widget_mouse_click(AButton,AState,AXpos,AYpos,ATime);
  }

  //----------

  void on_widget_mouse_release(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {

      MIP_GraphWire* wire;
      bool changed;

      //AXpos -= FRect.x;
      //AYpos -= FRect.y;
      changed = false;
      if (AButton == MIP_BUTTON_LEFT) {

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
                  addWire( new MIP_GraphWire(MDragWireModule,MDragWireInput, MHoverModule, MHoverOutput) ); // in -> out
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
                  addWire( new MIP_GraphWire(MHoverModule,MHoverInput,MDragWireModule,MDragWireOutput) ); // in -> out
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

      if (changed) do_widget_redraw(this,0);
      //inherited;
      MIP_PanelWidget::on_widget_mouse_release(AButton,AState,AXpos,AYpos,ATime);
  }

  //----------

  void on_widget_mouse_move(uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
      int32_t x = AXpos - getRect().x;
      int32_t y = AYpos - getRect().y;
      bool changed = false;
      MIP_ListNode* node = (MIP_GraphModule*)MModules.tail();
      while (node) {
        MIP_GraphModule* module = (MIP_GraphModule*)node;
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
        MHoverModule = nullptr;
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

      if (changed) do_widget_redraw(this,0);
      MMousePrevX = AXpos;
      MMousePrevY = AYpos;

      MIP_PanelWidget::on_widget_mouse_move(AState,AXpos,AYpos,ATime);
  }

  //----------

//  void on_widget_keyPress(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_keyRelease(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

  //------------------------------
  public:
  //------------------------------

//  void do_widget_realign(MIP_Widget* ASender, bool ARecursive=true) override {
//    //MIP_PRINT;
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
