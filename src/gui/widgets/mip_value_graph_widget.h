#ifndef kode_value_graph_widget_included
#define kode_value_graph_widget_included
//----------------------------------------------------------------------

#include "base/utils/kode_random.h"
#include "gui/kode_widget.h"

//----------

#define KODE_NUM_WIDGETNODES 1024

struct KODE_ValueGraphNode {
  float     value;
  uint32_t  flags;
};

//----------------------------------------------------------------------

class KODE_ValueGraphWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

  KODE_Color      MBackColor;
  KODE_Color      MLineColor;
  KODE_Color      MNodeColor;
  int32_t         MNumNodes;
  KODE_ValueGraphNode MNodes[KODE_NUM_WIDGETNODES];
  int32_t         MSelected;

//------------------------------
public:
//------------------------------

  KODE_ValueGraphWidget(KODE_FRect ARect, int32_t ANum)
  : KODE_Widget(ARect) {
    setName("KODE_ValueGraphWidget");
    setHint("valuegraph");
    MBackColor = KODE_COLOR_DARK_GRAY;
    MLineColor = KODE_COLOR_LIGHT_GRAY;
    MNodeColor = KODE_COLOR_BRIGHT_YELLOW;
    MNumNodes = ANum;
    memset(MNodes,0,sizeof(MNodes));
    MSelected = -1;
//    setFlag(KODE_WIDGET_HOVER);
//    //setFlag(kwf_clip);
    for (int32_t i=0; i<KODE_NUM_WIDGETNODES; i++) {
      MNodes[i].value = KODE_RandomSigned();
      MNodes[i].flags = 0;
    }
  }

//------------------------------
public:
//------------------------------

    void numNodes(int32_t ANum) {
      MNumNodes = ANum;
    }

    void setNode(int32_t AIndex, float AValue, uint32_t AFlags) {
      MNodes[AIndex].value = AValue;
      MNodes[AIndex].flags = AFlags;
    }

    void appendNode(float AValue, uint32_t AFlags) {
      MNodes[MNumNodes].value = AValue;
      MNodes[MNumNodes].flags = AFlags;
      MNumNodes++;
    }

    int32_t findNode(int32_t AXpos, int32_t AYpos) {
      float w = (float)(AXpos - getRect().x) / (float)getRect().w; // 0..1
      w *= (MNumNodes-1);
      int32_t index = roundf(w);// SMin( MNumNodes-1, roundf(w) );
      //STrace("w %f index %i\n",w,index);
      return index;//-1;
    }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    KODE_FRect mrect = getRect();
      APainter->fillRectangle(mrect,MBackColor);
      //APainter->setFillColor(MNodeColor);
      if (MNumNodes>0) {
        //int32 halfw = (mrect.w/2);
        int32_t halfh = (mrect.h/2);
        if (MNumNodes==1) {
          //STrace("NumNodes: %i\n",MNumNodes);
          int32_t h = halfh - ( (MNodes[0].value * halfh ) );
          APainter->drawLine(mrect.x,mrect.y+h,mrect.x2(),mrect.y+h,MLineColor);
        }
        else if (MNumNodes==2) {
          int32_t h1 = halfh - ( (MNodes[0].value * halfh ) );
          int32_t h2 = halfh - ( (MNodes[1].value * halfh ) );
          APainter->drawLine(mrect.x,mrect.y+h1,mrect.x2(),mrect.y+h2,MLineColor);
        }
        else {
          float w = (float)mrect.w / (float)(MNumNodes-1);
          float x = mrect.x;
          for (int32_t i=0; i<MNumNodes; i++) {
            int32_t h1,h2;
            h1 = halfh - ( (MNodes[i].value * halfh ) );
            if (i<MNumNodes-1) {
              h2 = halfh - ( (MNodes[i+1].value * halfh ) );
              APainter->drawLine(x,mrect.y+h1,x+w,mrect.y+h2,MLineColor);
            }

            if (i==MSelected) {
              APainter->fillRectangle( KODE_FRect(x-3, mrect.y+h1-3, 6, 6), MNodeColor );
            }

            x += w;
          }
        }
      }
  }

//  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

//  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) override {
//  }

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) override {
      //SWidget::on_mouseMove(AXpos,AYpos,AState);

      //float w = (float)(AXpos - mrect.x) / (float)mrect.w; // 0..1
      //int32 index = (w * (float)MNumNodes);

      int32_t index = findNode(AXpos,AYpos);
      if (index != MSelected) {
        MSelected = index;
        //STrace("MSelected: %i\n",MSelected);
        do_widget_redraw(this,getRect(),0);
      }
  }

//  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom, uint32_t ATimeStamp=0) override {
//    //if (flags.autoCursor) do_widget_setMouseCursor(this,MCursor);
//    //if (flags.autoHint) do_widget_setHint(this,MHint);
//  }

  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) override {
//    //if (flags.autoCursor) do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
//    //if (flags.autoHint) do_widget_setHint(this,"");
      //SWidget::on_leave(AWidget);
      if (MSelected>=0) {
        MSelected = -1;
        do_widget_redraw(this,getRect(),0);
      }
      KODE_Widget::on_widget_leave(AXpos,AYpos,ATo,ATimeStamp);
  }

};

#undef KODE_NUM_WIDGETNODES

//----------------------------------------------------------------------
#endif
