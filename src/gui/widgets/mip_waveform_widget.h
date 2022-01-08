#ifndef mip_waveform_widget_included
#define mip_waveform_widget_included
//----------------------------------------------------------------------

#include "audio/mip_audio_math.h"
#include "gui/mip_widgets.h"

//----------------------------------------------------------------------

struct MIP_WaveformMarker {
  bool        visible = true;
  //int32_t     pos;
  float       pos   = 0.0;;
  MIP_Color  color = MIP_COLOR_BLACK;;
};

struct MIP_WaveformArea {
  bool        visible = true;
  float       pos   = 0.0;
  float       size  = 0.0;;
  MIP_Color  color = MIP_COLOR_DARK_RED;
};

//typedef KArray<KWaveformMarker> KWaveformMarkers;
//typedef KArray<KWaveformArea> KWaveformAreas;

//----------------------------------------------------------------------

class MIP_WaveformWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

    float*                MBuffer         = nullptr;
    int32_t               MBufferSize     = 0;
    int32_t               MNumChannels    = 0;
    uint32_t              MStereoMode     = 1; // left

//------------------------------
protected:
//------------------------------

    MIP_Color            MWaveColor      = MIP_COLOR_LIGHT_GRAY;
    MIP_Color            MGridColor      = MIP_COLOR_GRAY;
    MIP_Color            MGridSubColor   = MIP_Color(0.9);
    MIP_Color            MZeroColor      = MIP_COLOR_GRAY;
    int32_t               MNumMarkers     = 0;
    int32_t               MNumAreas       = 0;
    MIP_WaveformMarker*  MMarkers        = nullptr;
    MIP_WaveformArea*    MAreas          = nullptr;
    int32_t               MNumGrid        = 0;
    int32_t               MNumGridSub     = 4;
    //bool                  MMono           = false;

    uint32_t MMarkerWidth = 1;

//------------------------------
public:
//------------------------------

  MIP_WaveformWidget(MIP_FRect ARect=MIP_FRect())
  : MIP_PanelWidget(ARect) {
    setName("MIP_WaveformWidget");
    setHint("waveform");
  }

  //----------

  virtual ~MIP_WaveformWidget() {
    if (MMarkers) free(MMarkers);
    if (MAreas) free(MAreas);
  }

//------------------------------
public:
//------------------------------

  int32_t getNumGrid()      { return MNumGrid; }
  int32_t getNumGridSub()   { return MNumGridSub; }
  int32_t getNumMarkers()   { return MNumMarkers; }
  int32_t getNumAreas()     { return MNumAreas; }

  //----------

  void setBuffer(float* ABuffer) {
    MBuffer = ABuffer;
  }

  void setBufferSize(int32_t ASize) {
    MBufferSize = ASize;
  }

  void setWaveColor(MIP_Color AColor) {
    MWaveColor = AColor;
  }

  //

  void setNumMarkers(int32_t ANum) {
    MNumMarkers = ANum;
    //SetLength(MMarkers,ANum);
    if (MMarkers) free(MMarkers);
    MMarkers = (MIP_WaveformMarker*)malloc(ANum * sizeof(MIP_WaveformMarker));
    for (int32_t i=0; i<MNumMarkers; i++) {
      MMarkers[i].visible = true;
      MMarkers[i].pos     = 0.0;
      MMarkers[i].color   = MIP_COLOR_BLACK;
    }
  }

  //void setMarker(int32_t AIndex, int32_t APos, MIP_Color AColor) {
  void setMarker(int32_t AIndex, float APos, MIP_Color AColor) {
    MMarkers[AIndex].pos = APos;
    MMarkers[AIndex].color = AColor;
  }

  //void setMarkerPos(int32_t AIndex, int32_t APos) {
  void setMarkerPos(int32_t AIndex, float APos) {
    MMarkers[AIndex].pos = APos;
  }

  void setMarkerColor(int32_t AIndex, MIP_Color AColor) {
    MMarkers[AIndex].color = AColor;
  }

  void setMarkerVisible(int32_t AIndex,bool AVisible = true) {
    MMarkers[AIndex].visible = AVisible;
  }

  //

  void setNumAreas(int32_t ANum) {
    MNumAreas = ANum;
    //SetLength(MAreas,ANum);
    if (MAreas) free(MAreas);
    MAreas = (MIP_WaveformArea*)malloc(ANum * sizeof(MIP_WaveformArea));
    for (int32_t i=0; i<MNumAreas; i++) {
      MAreas[i].visible = true;
      MAreas[i].pos     = 0.0;
      MAreas[i].size    = 0.0;
      MAreas[i].color   = MIP_COLOR_BLACK;
    }
  }

  void setArea(int32_t AIndex, float APos, float ASize, MIP_Color AColor) {
    MAreas[AIndex].pos = APos;
    MAreas[AIndex].size = ASize;
    MAreas[AIndex].color = AColor;
  }

  void setAreaPos(int32_t AIndex, float APos) {
    MAreas[AIndex].pos = APos;
  }

  void setAreaPosSize(int32_t AIndex, float APos, float ASize) {
    MAreas[AIndex].pos = APos;
    MAreas[AIndex].size = ASize;
  }

  void setAreaSize(int32_t AIndex, float ASize) {
    MAreas[AIndex].size = ASize;
  }

  void setAreaColor(int32_t AIndex, MIP_Color AColor) {
    MAreas[AIndex].color = AColor;
  }

  void setAreaVisible(int32_t AIndex, bool AVisible = true) {
    MAreas[AIndex].visible = AVisible;
  }

  void setNumGrid(int32_t ANum) {
    MNumGrid = ANum;
  }

  void setNumGridSub(int32_t ANum) {
    MNumGridSub = ANum;
  }

  //void setMono(bool AMono=true) {
  //  MMono = AMono;
  //}

  void setGridColor(MIP_Color AColor) {
    MGridColor = AColor;
  }

  void setGridSubColor(MIP_Color AColor) {
    MGridSubColor = AColor;
  }

  void setZeroColor(MIP_Color AColor) {
    MZeroColor = AColor;
  }

//------------------------------
public:
//------------------------------

  virtual void drawAreas(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    MIP_FRect mrect = getRect();
    for (int32_t i=0; i<MNumAreas; i++) {
      //MIP_DPrint("area %i\n",i);
      if (MAreas[i].visible) {
        //MIP_DPrint("- visible\n");
        if (MAreas[i].size > 0.0f) {
          //MIP_DPrint("- size > 0\n");
          float x = MAreas[i].pos;// / (float)MBufferSize;
          float w = MAreas[i].size;// / (float)MBufferSize;
          float ix = mrect.x + ( x * mrect.w );
          //int32_t iw = ix + ( w * mrect.w );
          float iw =  ( w * mrect.w );
          //APainter->fillRectangle(MIP_FRect(ix,mrect.y,iw,mrect.y2()), MAreas[i].color);
          APainter->fillRectangle(MIP_FRect(ix,mrect.y,iw,mrect.h), MAreas[i].color);
          //MIP_DPrint("%.2f, %.2f, %.2f, %.2f\n",(float)ix,mrect.y,(float)iw,mrect.h);
        }
      }
    }
  }

  //----------

  virtual void drawWaveform(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    MIP_FRect mrect = getRect();
    if (mrect.w > 0 ) {
      float h2 = (float)mrect.h * 0.5;
      int32_t midy = mrect.y + (mrect.h * 0.5f);
      if (MBufferSize > 0) {
        if (MBuffer) {
          float xadd = (float)MBufferSize / (float)mrect.w;
          float x = 0;
          #ifdef MIP_GUI_CAIRO
            APainter->setColor(MWaveColor);
            APainter->moveTo(mrect.x,midy);
          #endif
          for (int32_t i=0; i<mrect.w; i++) {
            int32_t index = x;
            //float s;// = 0;
            //if (MMono) s = MBuffer[index];
            //else s = ( MBuffer[ index*2 ] + MBuffer[ index*2 + 1 ] ) * 0.5;
            float s = 0.0f;
            switch (MStereoMode) {
              case 0: s =  MBuffer[ index * 2     ];                                    break; // mono
              case 1: s = (MBuffer[ index * 2     ] + MBuffer[(index * 2) + 1]) * 0.5f; break; // stereo mixdown
              case 2: s =  MBuffer[ index * 2     ];                                    break; // stereo left
              case 3: s =  MBuffer[(index * 2) + 1] ;                                   break; // stereo right
            }
            // todo: if (MDrawDb) s = KVolumeToDb(s);

            //s = 20.0 * log10(s);
            //s = MIP_VolumeToDb(s) * (1.0/90.0);
            //s = MIP_Curve(s,0.33);

            s *=  h2;
            int32_t ix = mrect.x + i;
            int32_t iy = s;
            #ifdef MIP_GUI_CAIRO
              APainter->lineTo(ix,midy-iy);
            #else
              APainter->drawLine( ix, midy, ix, midy-iy, MWaveColor,1 );
            #endif
            x += xadd;
          } // for w
          #ifdef MIP_GUI_CAIRO
            APainter->strokePath();
          #endif
        } // buffer
      } // buffersize > 0
    } // w > 0
  }

  //----------

  virtual void drawMarkers(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    MIP_FRect mrect = getRect();
    for (int32_t i=0; i<MNumMarkers; i++) {
      if (MMarkers[i].visible) {
        float x = MMarkers[i].pos;// / (float)MBufferSize;
        if ((x >= 0.0f) && (x <= 1.0f)) {
          float ix = mrect.x + ( x * mrect.w );
          APainter->drawLine( ix, mrect.y, ix, mrect.y2(), MMarkers[i].color, MMarkerWidth );
        }
      }
    }
  }

  //----------

  virtual void drawGrid(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    MIP_FRect mrect = getRect();
    if (MNumGrid > 1) {
      float xadd = (float)mrect.w / (float)MNumGrid;
      float x = mrect.x + xadd;

      //APainter->setDrawColor(MGridColor);

      for (int32_t i=1; i<MNumGrid; i++) {
        int32_t ix = x;

        //if ((i % MNumGridSub) == 0) APainter->setColor(MGridSubColor);
        //else APainter->setColor(MGridColor);
        MIP_Color color = MGridColor;
        if ((i % MNumGridSub) == 0) color = MGridSubColor;
        //APainter->moveTo( ix, mrect.y);
        //APainter->lineTo( ix, mrect.y2());
        //APainter->strokePath();
        APainter->drawLine( ix, mrect.y, ix, mrect.y2(), color );
        x += xadd;
      }

    }
  }

  //----------

  virtual void drawZeroLine(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) {
    MIP_FRect mrect = getRect();
    int32_t midy = mrect.y + (mrect.h * 0.5f);
    APainter->drawLine( mrect.x, midy,mrect.x2(),midy, MZeroColor );
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {
//    APainter->setLineWidth(1);
    fillBackground(APainter,ARect,AMode);
    drawAreas(APainter,ARect,AMode);
    drawWaveform(APainter,ARect,AMode);
    drawMarkers(APainter,ARect,AMode);
    drawGrid(APainter,ARect,AMode);
    drawZeroLine(APainter,ARect,AMode);
    //MIP_Widget::on_widget_paint(APainter,ARect,0);
    drawBorder(APainter,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif

