#ifndef mip_circular_waveform_widget_included
#define mip_circular_waveform_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"

class MIP_CircularWaveformWidget
: public MIP_PanelWidget {

//------------------------------
private:
//------------------------------

    float*      MBuffer       = nullptr;
    int32_t     MBufferSize   = 0;
    //float       MRadius       = 100.0f;
    float       MOuterRadius  = 1.0f;
    float       MInnerRadius  = 0.6f;
    bool        MBipolar      = false;
    MIP_Color  MLineColor    = MIP_COLOR_DARK_GRAY;

//------------------------------
public:
//------------------------------

  MIP_CircularWaveformWidget(MIP_FRect ARect)
  : MIP_PanelWidget(ARect) {
    setName("MIP_CircularWaveformWidget");
    setHint("circularwaveform");
  }

  //----------

  virtual ~MIP_CircularWaveformWidget() {
    //if (MBuffer) SFree(MBuffer);
  }

//------------------------------
public:
//------------------------------

  public:

    float*    getBuffer(void)                         { return MBuffer; }
    float     getBuffer(int32_t AIndex)               { return MBuffer[AIndex]; }
    int32_t   getBufferSize(void)                     { return MBufferSize; }

    void      setBuffer(int32_t AIndex, float AValue) { MBuffer[AIndex] = AValue; }
    void      setBuffer(float* ABuffer)               { MBuffer = ABuffer; }
    void      setBufferSize(int32_t ASize)            { MBufferSize = ASize; }
    //void      setRadius(float ARadius)              { MRadius = ARadius; }
    void      setOuterRadius(float ARadius)           { MOuterRadius = ARadius; }
    void      setInnerRadius(float ARadius)           { MInnerRadius = ARadius; }
    void      setBipolar(bool ABipolar=true)          { MBipolar = ABipolar; }

//------------------------------
public:
//------------------------------

//#define NUMSTEPS 1024

  void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode) override {

    fillBackground(APainter,ARect,AMode);

    MIP_FRect mrect = getRect();
    if (MBufferSize > 0) {

      float xcenter = mrect.x + (mrect.w / 2);
      float ycenter = mrect.y + (mrect.h / 2);
      float radius = MIP_Min(mrect.w,mrect.h);
      float r_range  = ( MOuterRadius - MInnerRadius );
      //float r_center = MInnerRadius + ( r_range / 2.0f );
      float angle = 0;
      float angleadd = 1.0f / MBufferSize;
      //float prevx = cosf(angle*MIP_PI2) * r_center;
      //float prevy = sinf(angle*MIP_PI2) * r_center;
      float vv;//  = 0.5f;
      float xx;//  = 1.0f;//cosf(0.0f);
      float yy;//  = 0.0f;//sinf(0.0f);
      float v   = MInnerRadius + (0.5f * r_range);
      float prevx = v * radius * 0.5f;
      float prevy = 0;

      MIP_FRect rec2 = mrect;
      float r2 = 0.0f;
      if (MBipolar) { r2 = 1.0f - (MInnerRadius + (r_range * 0.5f)); }
      else          { r2 = 1.0f -  MInnerRadius; }
      r2 *= radius;// * 0.5f;
      rec2.shrink( r2 * 0.5f );

      APainter->drawEllipse(rec2,MIP_COLOR_WHITE,1);

      if (MBuffer) {
        if (MBipolar) {
          for (int32_t i=0; i<MBufferSize; i++) {
            vv = (MBuffer[i] + 1.0f) * 0.5f;      // 0..1
            v  = MInnerRadius + (vv * r_range);   // 0.5 + (vv*0.5) = 0.5+0..0.5+0.5 = 0.5..1
            xx = cosf(angle*MIP_PI2);
            yy = sinf(angle*MIP_PI2);
            float x = xx * v * radius * 0.5;
            float y = yy * v * radius * 0.5;
            APainter->drawLine(xcenter+prevx,ycenter+prevy,xcenter+x,ycenter+y,MLineColor);
            prevx = x;
            prevy = y;
            angle += angleadd;
          }
        }
        else {
          for (int32_t i=0; i<MBufferSize; i++) {
            v  = MInnerRadius;
            xx = cosf(angle*MIP_PI2);
            yy = sinf(angle*MIP_PI2);
            float x1 = xx * v * radius * 0.5;
            float y1 = yy * v * radius * 0.5;
            vv = MBuffer[i];      // 0..1
            v  = MInnerRadius + (vv * r_range);   // 0.5 + (vv*0.5) = 0.5+0..0.5+0.5 = 0.5..1
            xx = cosf(angle*MIP_PI2);
            yy = sinf(angle*MIP_PI2);
            float x2 = xx * v * radius * 0.5;
            float y2 = yy * v * radius * 0.5;
            APainter->drawLine(xcenter+x1,ycenter+y1,xcenter+x2,ycenter+y2,MLineColor);
            //APainter->drawPoint(xcenter+x1,ycenter+y1);
            //prevx = x;
            //prevy = y;
            angle += angleadd;
          }
        } // bipolar
      } // buffer

//      APainter->drawEllipse(rec2,MIP_COLOR_WHITE,1);

//      MIP_FRect rec2 = mrect;
//      float r2 = 0.0f;
//      if (MBipolar) { r2 = 1.0f - (MInnerRadius + (r_range * 0.5f)); }
//      else          { r2 = 1.0f -  MInnerRadius; }
//      r2 *= radius;// * 0.5f;
//      rec2.shrink( r2 * 0.5f );
//      APainter->drawEllipse(rec2,MIP_COLOR_WHITE,1);

    } // size > 0


    drawBorder(APainter,ARect,AMode);
  }

};

#undef NUMSTEPS

//----------------------------------------------------------------------
#endif
