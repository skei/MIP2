#ifndef mip_keyboard_widget_included
#define mip_keyboard_widget_included
//----------------------------------------------------------------------

#include "gui/mip_widget.h"

/*

  |  |3| | |  |  | | | | | |  |
  |  |_| |_|  |  |_| |_| |_|  |
  |0  |1  |2  |   |   |   |   |
  |___|___|___|___|___|___|___|

  note_data:

  ofs: 'steps' (note-width/3) to skip before draw/calc, to get to the upper
       left of the next note (from the previous position)
  type: 0..3 = c,d,e,black

*/

//static
const int32_t MIP_KEYBOARD_INTERNAL_NOTE_DATA[12*2] = {
  0, 0, // c
  2, 3, // c#
  1, 1, // d
  2, 3, // d#
  1, 2, // e
  3, 0, // f
  2, 3, // f#
  1, 1, // g
  2, 3, // g#
  1, 1, // a
  2, 3, // a#
  1, 2  // b
};

//----------------------------------------------------------------------

class MIP_KeyboardWidget
: public MIP_Widget {

//------------------------------
private:
//------------------------------

  bool        MActiveMap[12]    = {1,1,1,1,1,1,1,1,1,1,1,1};
  int32_t     MNoteRects1[12*4] = {0};
  int32_t     MNoteRects2[12*4] = {0};

//------------------------------
protected:
//------------------------------

  int32_t     MDist             = 2;
  float       MRatio            = 0.6;
  MIP_Color  MBlackColor        = MIP_Color(0.35);
  MIP_Color  MWhiteColor        = MIP_Color(0.65);
  MIP_Color  MBlackActiveColor  = MIP_Color(0);
  MIP_Color  MWhiteActiveColor  = MIP_Color(1);
  bool        MVertical         = false;

//------------------------------
public:
//------------------------------

  MIP_KeyboardWidget(MIP_DRect ARect=MIP_DRect())
  : MIP_Widget(ARect) {
    setWidgetName("MIP_KeyboardWidget");
    //setWidgetHint("keyboard");
    setWidgetCursor(MIP_CURSOR_FINGER);
    //recalc();
  }

  virtual ~MIP_KeyboardWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setColors(MIP_Color AWhite, MIP_Color ABlack, MIP_Color AWhiteActive, MIP_Color ABlackActive) {
    MWhiteColor       = AWhite;
    MWhiteActiveColor = AWhiteActive;
    MBlackColor       = ABlack;
    MBlackActiveColor = ABlackActive;
  }

  //----------

  virtual void setDist(int32_t ADist) {
    MDist = ADist + 1;
  }

  //----------

  virtual void setRatio(float ARatio) {
    MRatio = ARatio;
  }

  virtual void setVertical(bool AVertical=true) {
    MVertical = AVertical;
  }

  //----------

  virtual void activate(int32_t num) {
    MActiveMap[num] = true;
  }

  //----------

  virtual void deactivate(int32_t num) {
    MActiveMap[num] = false;
  }

//------------------------------
public:
//------------------------------

  virtual void recalc_note_rects(void) {
    MIP_DRect mrect = getRect();
    if (MVertical) {
      int32_t size = mrect.h;
      float   s1 = (float)size / 21.0; // 7 notes, 3 'splits' per note?
      float   s2 = s1 * 2;
      float   s3 = s1 * 3;
      int32_t y1    = 0;
      int32_t x0    = 0;
      int32_t x1    = floorf((float)mrect.w * (1-0 - MRatio));
      int32_t x2    = mrect.w - 1;
      for (int32_t note=0; note<12; note++) {
        int32_t n4    = note * 4;
        int32_t iofs  = MIP_KEYBOARD_INTERNAL_NOTE_DATA[note*2];
        float   ofs   = (float)iofs * s1;
        y1 += ofs;
        int32_t typ   = MIP_KEYBOARD_INTERNAL_NOTE_DATA[(note*2)+1];
        switch(typ) {
          case 0: // C
            MNoteRects1[n4+0] = x0;         MNoteRects1[n4+1] = floorf(y1);
            MNoteRects1[n4+2] = x1;         MNoteRects1[n4+3] = floorf(y1+s3) - MDist;
            MNoteRects2[n4+0] = x1;         MNoteRects2[n4+1] = floorf(y1);
            MNoteRects2[n4+2] = x2;         MNoteRects2[n4+3] = floorf(y1+s2) - MDist;
            break;
          case 1: // D
            MNoteRects1[n4+0] = x0;         MNoteRects1[n4+1] = floorf(y1);
            MNoteRects1[n4+2] = x1;         MNoteRects1[n4+3] = floorf(y1+s3) - MDist;
            MNoteRects2[n4+0] = x1;         MNoteRects2[n4+1] = floorf(y1+s1);
            MNoteRects2[n4+2] = x2;         MNoteRects2[n4+3] = floorf(y1+s2) - MDist;
            break;
          case 2: // E
            MNoteRects1[n4+0] = x0;         MNoteRects1[n4+1] = floorf(y1);
            MNoteRects1[n4+2] = x1;         MNoteRects1[n4+3] = floorf(y1+s3) - MDist;
            MNoteRects2[n4+0] = x1;         MNoteRects2[n4+1] = floorf(y1+s1);
            MNoteRects2[n4+2] = x2;         MNoteRects2[n4+3] = floorf(y1+s3) - MDist;
            break;
          case 3: // black
            MNoteRects1[n4+0] = x1 + MDist; MNoteRects1[n4+1] = floorf(y1);
            MNoteRects1[n4+2] = x2;         MNoteRects1[n4+3] = floorf(y1+s2) - MDist - 1;
            break;
        } // switch
      } // for note
    }
    else {
      int32_t size = mrect.w;
      float   s1 = (float)size / 21.0; // 7 notes, 3 'splits' per note?
      float   s2 = s1 * 2;
      float   s3 = s1 * 3;
      float   x1    = 0;
      int32_t y1    = floorf((float)mrect.h * MRatio);
      int32_t y2    = mrect.h - 1;
      for (int32_t note=0; note<12; note++) {
        int32_t n4    = note * 4;
        int32_t iofs  = MIP_KEYBOARD_INTERNAL_NOTE_DATA[note*2];
        float   ofs   = (float)iofs * s1;
        x1 += ofs;
        int32_t typ   = MIP_KEYBOARD_INTERNAL_NOTE_DATA[(note*2)+1];
        switch(typ) {
          case 0: // C
            MNoteRects1[n4+0] = floorf(x1);             MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1+s2) - MDist;  MNoteRects1[n4+3] = y1;
            MNoteRects2[n4+0] = floorf(x1);             MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1+s3) - MDist;  MNoteRects2[n4+3] = y2;
            break;
          case 1: // D
            MNoteRects1[n4+0] = floorf(x1+s1);          MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1+s2) - MDist;  MNoteRects1[n4+3] = y1;
            MNoteRects2[n4+0] = floorf(x1);             MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1+s3) - MDist;  MNoteRects2[n4+3] = y2;
            break;
          case 2: // E
            MNoteRects1[n4+0] = floorf(x1+s1);          MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1+s3) - MDist;  MNoteRects1[n4+3] = y1;
            MNoteRects2[n4+0] = floorf(x1);             MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1+s3) - MDist;  MNoteRects2[n4+3] = y2;
            break;
          case 3: // black
            MNoteRects1[n4+0] = floorf(x1);             MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1+s2) - MDist;  MNoteRects1[n4+3] = y1 - MDist;
            break;
        } // switch
      } // for note
    } // !vertical
  }

  //----------

  // which = 0..1, top/bottom

  virtual void fill_note_rects(MIP_Painter* APainter, int32_t note, uint32_t which, MIP_Color color) {
    MIP_DRect r;
    float x = getRect().x;
    float y = getRect().y;
    int32_t n = note * 4;
    switch (which) {
      case 1:
        r.x =     x + MNoteRects1[n  ];
        r.y =     y + MNoteRects1[n+1];
        r.setX2(  x + MNoteRects1[n+2] );
        r.setY2(  y + MNoteRects1[n+3] );
        break;
      case 2:
        r.x =     x + MNoteRects2[n  ];
        r.y =     y + MNoteRects2[n+1];
        r.setX2(  x + MNoteRects2[n+2] );
        r.setY2(  y + MNoteRects2[n+3] );
        break;
    }
    //APainter->fillRectangle( r, color );
    APainter->beginPath();
    APainter->rect(r.x,r.y,r.w,r.h);
    APainter->fillColor(color);
    APainter->fill();

  }

  //----------

  virtual void draw_note(MIP_Painter* APainter, int32_t ANote, bool AActive=false) {
    int32_t typ = MIP_KEYBOARD_INTERNAL_NOTE_DATA[(ANote*2)+1];
    if (typ == 3) {
      if (AActive) {
        fill_note_rects(APainter,ANote,1,MBlackActiveColor);
      }
      else {
        fill_note_rects(APainter,ANote,1,MBlackColor);
      }
    }
    else {
      if (AActive) {
        fill_note_rects(APainter,ANote,1,MWhiteActiveColor);
        fill_note_rects(APainter,ANote,2,MWhiteActiveColor);
      }
      else {
        fill_note_rects(APainter,ANote,1,MWhiteColor);
        fill_note_rects(APainter,ANote,2,MWhiteColor);
      }
    } //type=0,1,2
  }

  //----------

  virtual int32_t find_note(float AXpos, float AYpos) {
    MIP_DRect mrect = getRect();
    //int32_t hit = -1;
    for (int32_t note=0; note<12; note++) {
      int32_t n4 = note*4;
      MIP_DRect r;
      r.x =  mrect.x + MNoteRects1[n4  ];
      r.y =  mrect.y + MNoteRects1[n4+1];
      r.w = (mrect.x + MNoteRects1[n4+2]) - r.x;
      r.h = (mrect.y + MNoteRects1[n4+3]) - r.y;
      if (r.contains(AXpos,AYpos)) return note;
      int32_t typ = MIP_KEYBOARD_INTERNAL_NOTE_DATA[/*n2*/(note*2)+1];
      if (typ < 3) { //0,1,2 = white keys
        r.x =  mrect.x + MNoteRects2[n4  ];
        r.y =  mrect.y + MNoteRects2[n4+1];
        r.w = (mrect.x + MNoteRects2[n4+2]) - r.x;
        r.h = (mrect.y + MNoteRects2[n4+3]) - r.y;
        if (r.contains(AXpos,AYpos)) return note;
      }
    }
    return -1;
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(MIP_PaintContext* AContext) override {
    recalc_note_rects();
    for (int32_t note=0; note<12; note++) {
      bool active = MActiveMap[note];
      draw_note(AContext->painter,note,active);
    }
  }

  //----------

  void on_widget_mouse_click(uint32_t AButton, uint32_t AState, double AXpos, double AYpos, uint32_t ATime) override {
    //MIP_DRect mrect = getRect();
    if (AButton == MIP_BUTTON_LEFT) {
      int32_t note = find_note(AXpos,AYpos);
      if (note >= 0) {
        if (MActiveMap[note]) deactivate(note);
        else activate(note);
        do_widget_update(this);
        do_widget_redraw(this,0);
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
