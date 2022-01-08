#if 0

#ifndef kode_timeline_widget_included
#define kode_timeline_widget_included
//----------------------------------------------------------------------

#include "base/kode_list.h"
#include "gui/kode_widget.h"

//----------

#define TRACK_HEIGHT      20
#define TRACK_WIDTH       100
#define PIXELS_PER_SECOND 10
#define SECONDS_PER_PIXEL (1.0f/(float)PIXELS_PER_SECOND)
#define DRAG_HANDLE_SIZE  4

//----------

//----------------------------------------------------------------------
// segment
//----------------------------------------------------------------------

class KODE_TimelineSegment
: public KODE_ListNode {

  public:

    const char* MName       = "segment";
    float       MStartPos   = 0.0f;
    float       MEndPos     = 1.0f;
    bool        MSelected   = false;

  public:

    void setName(const char* AName)   { MName = AName; }
    void setStart(float APos)         { MStartPos = APos; }
    void setEnd(float APos)           { MEndPos = APos; }
    void select(float ASelected=true) { MSelected = ASelected; }

    const char* getName(void)     { return MName; }
    float       getStart(void)    { return MStartPos; }
    float       getEnd(void)      { return MEndPos; }
    bool        isSelected(void)  { return MSelected; }

  public:

    KODE_TimelineSegment(const char* AName, float AStart, float AEnd)
    : KODE_ListNode() {
      MName     = AName;
      MStartPos = AStart;
      MEndPos   = AEnd;
      MSelected = false;
    }

    //----------

    virtual ~KODE_TimelineSegment() {
    }

    //----------

    bool isInside(float ATime) {
      if ((ATime >= MStartPos) && (ATime <= MEndPos)) return true;
      else return false;
    }

};

//typedef list<KODE_TimelineSegment*> KODE_TimelineSegments;
//typedef KODE_Array<KODE_TimelineSegment*> KODE_TimelineSegments;

//----------------------------------------------------------------------
// track
//----------------------------------------------------------------------

class KODE_TimelineTrack
: public KODE_ListNode {

  private:

    const char* MName     = "track";
    bool        MSelected = false;
    KODE_List   MSegments;
    //KODE_TimelineSegments MSegments;

  public:

    const char*     getName(void) { return MName; }
    void            setName(char* AName) { MName = AName; }
    void            select(float ASelected=true) { MSelected = ASelected; }
    bool            isSelected(void) { return MSelected; }
    KODE_ListNode*  getSegments(void) { return MSegments.head(); }
    //KODE_TimelineSegments*  getSegments(void) { return &MSegments; }

  public:

    KODE_TimelineTrack(const char* AName)
    : KODE_ListNode() {
      MName = AName;
      MSelected = false;
    }

    //----------

    virtual ~KODE_TimelineTrack() {
      #ifndef KODE_NO_AUTODELETE
      deleteSegments();
      #endif
    }

    //----------

    void addSegment(KODE_TimelineSegment* ASegment) {
      MSegments.insertTail(ASegment);
      //MSegments.push_back(ASegment);
    }

    //----------

    void deleteSegments(void) {
      //KListNode* node = MSegments.head();
      //while (node) {
      //  KTimeline_Segment* segment = (KTimeline_Segment*)node;
      //  node = node->next();
      //  delete segment;
      //}
      MSegments.clear();
    }

};

//typedef KODE_Array<KODE_TimelineTrack*> KODE_TimelineTracks;

//----------------------------------------------------------------------
// track
//----------------------------------------------------------------------

class KODE_TimelineWidget
: public KODE_Widget {

  private:
    float                 MZoom             = 1.0f;
    float                 MStartPos         = 0.0f;
    int32_t               MPrevMouseX       = 0;
    int32_t               MPrevMouseY       = 0;
  private:
    KODE_TimelineTrack*   MHoverTrack       = KODE_NULL;
    KODE_TimelineSegment* MHoverSegment     = KODE_NULL;
    float                 MHoverTime        = -1.0f;
    bool                  MHoverLeft        = false;
    bool                  MHoverRight       = false;
  private:
    KODE_TimelineSegment* MDraggingSegment  = KODE_NULL;
    bool                  MDraggingLeft     = false;
    bool                  MDraggingRight    = false;
  protected:
    KODE_Color  MBackColor        = KODE_Grey;
    KODE_Color  MTrackBackColor   = KODE_DarkGrey;
    KODE_Color  MTrackTextColor   = KODE_LightGrey;
    KODE_Color  MSegmentBackColor = KODE_LightGrey;
    KODE_Color  MSegmentTextColor = KODE_DarkGrey;
    KODE_Color  MBorderColor      = KODE_Black;
    KODE_Color  MTextColor        = KODE_Red;

  private:

    //KODE_TimelineTracks   MTracks;

    KODE_List MTracks;
//    int32_t   MCursor;

  //------------------------------
  //
  //------------------------------

  public:

    //KODE_TimelineTracks*  getTracks(void)            { return &MTracks; }
    KODE_ListNode*  getTracks(void)            { return MTracks.head(); }
//    int32_t         getCursor(void)            { return MCursor; }
    float           getZoom(void)              { return MZoom; }
    float           getStartpos(void)          { return MStartPos; }

//    void            setCursor(int32_t ACursor) { MCursor=ACursor; }
    void            setZoom(float AZoom)       { MZoom = AZoom; }
    void            setStartpos(float APos)    { MStartPos = APos; }

  //------------------------------
  //
  //------------------------------

  public:

    KODE_TimelineWidget(KODE_Rect ARect)
    : KODE_Widget(ARect) {
      MName = "KODE_TimelineWidget";
      setFlag(KODE_WIDGET_HOVER);
      //setFlag(KODE_WIDGET_CLIP);
    }

    //----------

    virtual ~KODE_TimelineWidget() {
      #ifndef KODE_NO_AUTODELETE
      deleteTracks();
      #endif
    }

  //------------------------------
  //
  //------------------------------

  public:

    void addTrack(KODE_TimelineTrack* ATrack) {
      //MTracks.append(ATrack);
      //MTracks.push_back(ATrack);
      MTracks.insertTail(ATrack);
    }

    //----------

    void deleteTracks(void) {
      //KListNode* node = MTracks.head();
      //while (node) {
      //  KTimeline_Track* track = (KTimeline_Track*)node;
      //  node = node->next();
      //  //track->deleteSegments();
      //  delete track;
      //}
      MTracks.clear();
    }

  //------------------------------
  //
  //------------------------------

  private:

    float calcLength(int32_t APixels) {
      float time = (float)( APixels * SECONDS_PER_PIXEL );
      return time / MZoom;
    }

    //----------

    int32_t calcPixels(int32_t ATime) {
      float pixel = ATime * PIXELS_PER_SECOND;
      return KODE_Trunc( pixel*MZoom );
    }

    //----------

    float calcTime(int32_t AXpos) {
      //KTrace("%i\n",AXpos);
      if (MZoom <= 0) return -1;
      float time = (float)( AXpos - TRACK_WIDTH );
      //KTrace("%.3f\n",time);
      if (time < 0) return -1;
      time *= SECONDS_PER_PIXEL;
      //KTrace("%.3f\n",time);
      float result = (MStartPos + time) / MZoom;
      return result;
    }

    //----------

    float calcTimeDiff(int32_t AXpos, int32_t APrev) {
      if (MZoom <= 0) return 0;
      float diff = (float)( AXpos - APrev );
      //if time < 0 then exit(0);
      diff *= SECONDS_PER_PIXEL;
      return diff / MZoom;
    }

    //----------

    int32_t calcXpos(float ATime) {
      if (ATime >= 0) {
        float pixel = ((ATime*MZoom) * PIXELS_PER_SECOND);
        return  TRACK_WIDTH + KODE_Trunc(pixel);
      }
      return -1;
    }

    //----------

    /*
      TODO:
      - each track has individual height

    */

    KODE_TimelineTrack* findTrack(int32_t AYpos) {
      int32_t y = AYpos - MRect.y;
      if (y < 0) return KODE_NULL;
      int32_t t = y / TRACK_HEIGHT;

      KODE_ListNode* node = (KODE_ListNode*)MTracks.head();
      while (node) {
        if (t == 0) return (KODE_TimelineTrack*)node;
        t -= 1;
        node = node->next();
      }
      return nullptr;

      //KODE_TimelineTracks::iterator iter = MTracks.begin();
      //while (iter != MTracks.end()) {
      //  KODE_TimelineTrack* track = *iter;
      //  if (t == 0) return track;
      //  t -= 1;
      //}

    }

    //----------

    KODE_TimelineSegment* findSegment(KODE_TimelineTrack* ATrack, float ATime) {

      //KODE_ListNode* node = (KODE_ListNode*)ATrack->getSegments()->head();
      KODE_ListNode* node = ATrack->getSegments();
      while (node) {
        KODE_TimelineSegment* segment = (KODE_TimelineSegment*)node;
        if ( (ATime >= segment->getStart()) && (ATime <= segment->getEnd()) ) return segment;
        node = node->next();
      }
      return KODE_NULL;

      //KODE_TimelineSegments* segments = ATrack->getSegments();
      //KODE_TimelineSegments::iterator iter = segments->begin();
      //while (iter != segments->end()) {
      //  KODE_TimelineSegment* segment = *iter;
      //  if ( (ATime >= segment->getStart()) && (ATime <= segment->getEnd()) ) return segment;
      //}
      //return nullptr;
    }

  //------------------------------
  //
  //------------------------------

  public:

    void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
      bool changed = false;
      if (AButton == KODE_BUTTON_LEFT) {
        MPrevMouseX = AXpos;
        MPrevMouseY = AYpos;
        if (!MHoverSegment) return;
        MDraggingSegment = MHoverSegment;
        MDraggingLeft = MHoverLeft;
        MDraggingRight = MHoverRight;
      }
      if (changed) do_redraw(this,MRect);
      //KWidget::on_widgetMouseDown(AWidget,AXpos,AYpos,AButton,AState);
    }

    //----------

    void on_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
      bool changed = false;
      if (AButton == KODE_BUTTON_LEFT) {
        //if (!MDraggingSegment) return;
        MDraggingSegment = nullptr;
        MDraggingLeft = false;
        MDraggingRight = false;
      }
      if (changed) do_redraw(this,MRect);
      //KWidget::on_widgetMouseUp(AWidget,AXpos,AYpos,AButton,AState);
    }

    //----------

    void on_mouseMove(float AXpos, float AYpos, uint32_t AState) override {

      //AXpos -= MRect.x;
      //AYpos -= MRect.y;
      //KTrace("AXpos %i AYpos %i\n",AXpos,AYpos);

      //AXpos -= MPadding.x;
      //AYpos -= MPadding.y;

      bool changed = false;
      KODE_TimelineTrack* track = findTrack(AYpos);
      KODE_TimelineSegment* segment = nullptr;

      MHoverTime = calcTime(AXpos - MRect.x);
      //KTrace("MHoverTime %.3f\n",MHoverTime);

      if (MDraggingSegment) {

        float mintime = calcLength(10); // minimum 10 pixels
        float diff = calcTimeDiff(AXpos,MPrevMouseX);
        float st = MDraggingSegment->getStart();
        float en = MDraggingSegment->getEnd();

        if (MDraggingLeft) {
          st += diff;
          st = KODE_Max(st,0.0f);
          if (st > (en-mintime)) st = (en-mintime);
        }
        else if (MDraggingRight) {
          en += diff;
          if (en <  (st+mintime)) en = (st+mintime);
        }
        else {
          // dragging segment
          float le = en - st;
          st += diff;
          st = KODE_Max(st,0.0f);
          en = st + le;
        }

//        KODE_TimelineSegment* prv = (KODE_TimelineSegment*)MDraggingSegment->prev();
//        if (prv) {
//          if (st <= prv->getEnd()) {
//            float le = en - st;
//            st = prv->getEnd(); //+ 0.0001;
//            if (!MDraggingLeft) en = st + le;
//          }
//        }

//        KODE_TimelineSegment* nxt = (KODE_TimelineSegment*)MDraggingSegment->next();
//        if (nxt) {
//          if (en >= nxt->getStart()) {
//            float le = en - st;
//            en = nxt->getStart(); //- 0.0001;
//            if (!MDraggingRight) st = en - le;
//          }
//        }

        MDraggingSegment->setStart(st);
        MDraggingSegment->setEnd(en);
        changed = true;
      } // dragging

      else { // not dragging

        bool hoverleft = false;
        bool hoverright = false;
        if (track) {
          segment = findSegment(track,MHoverTime);
          if (segment) {

            int32_t leftx  = calcXpos(segment->getStart());
            int32_t rightx = calcXpos(segment->getEnd());

            //KTrace("leftx %i AXpos %i DRAG_HANDLE_SIZE %i\n",leftx,AXpos,DRAG_HANDLE_SIZE);

            if ((AXpos-MRect.x) < (leftx+DRAG_HANDLE_SIZE)) hoverleft = true;
            if ((AXpos-MRect.x) >= (rightx-DRAG_HANDLE_SIZE)) hoverright = true;
          }
        }
        if (track != MHoverTrack) {
          MHoverTrack = track;
          changed = true;
        }
        if (segment != MHoverSegment) {
          MHoverSegment = segment;
          changed = true;
        }
        if (hoverleft != MHoverLeft) {
          MHoverLeft = hoverleft;
          changed = true;
        }
        if (hoverright != MHoverRight) {
          MHoverRight = hoverright;
          changed = true;
        }
      } // not dragging
      if (changed) do_redraw(this,MRect);
      MPrevMouseX = AXpos;
      MPrevMouseY = AYpos;
      //KWidget::on_widgetMouseMove(AWidget,AXpos,AYpos,AState);
    }

    //----------

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
      APainter->setFillColor(MBackColor);
      APainter->fillRectangle(MRect.x,MRect.y,MRect.x2(),MRect.y2());
      APainter->setDrawColor(MBorderColor);
      //APainter->drawLine(MRect.x,MRect.y,MRect.x2(),MRect.y);
      APainter->drawHLine(MRect.x,MRect.y,MRect.x2());
      int32_t x1 = MRect.x;
      //int32_t x2 := MRect.x2;
      int32_t y1 = MRect.y;

      //KODE_ListNode* tnode = (KODE_ListNode*)MTracks.head();
      //while (tnode) {
        //KODE_TimelineTrack* track = (KODE_TimelineTrack*)tnode;

      //KODE_TimelineTrack* track = MTracks.head();
      KODE_TimelineTrack* track = (KODE_TimelineTrack*)MTracks.head();
      while (track) {
        //KODE_Trace("track %p\n",track);

        int32_t x2 = x1 + TRACK_WIDTH - 1;
        int32_t y2 = y1 + TRACK_HEIGHT - 1;
        // { track background }
        if (track == MHoverTrack) APainter->setFillColor( KODE_Color(0.3,0.3,0.3) );
        else APainter->setFillColor(MTrackBackColor);
        APainter->fillRectangle(x1,y1,x2,y2);
        // { track name }
        APainter->setTextColor(MTrackTextColor);
        APainter->drawText(x1+2,y1,x2-2,y2,track->getName(), KODE_TEXT_ALIGN_CENTER);
        // { track border }
        APainter->setDrawColor(MBorderColor);   // below

        //APainter->drawLine(x1,y2,MRect.x2(),y2);  // below
        //APainter->drawLine(x2,y1,x2,y2);        // right

        APainter->drawHLine(x1,y2,MRect.x2());  // below
        APainter->drawVLine(x2,y1,y2);        // right


        // { track segments }

        //KODE_ListNode* snode = (KODE_ListNode*)track->getSegments()->head();
        //while (snode) {
        //  KODE_TimelineSegment* segment = (KODE_TimelineSegment*)snode;

        //KODE_TimelineSegment* segment = track->getSegments()->head();
      KODE_TimelineSegment* segment = (KODE_TimelineSegment*)track->getSegments();

        while (segment) {
          //KODE_Trace("track %p\n",track);

          float ss = segment->getStart() * MZoom * PIXELS_PER_SECOND;
          float se = segment->getEnd() * MZoom * PIXELS_PER_SECOND;
          // { back }
          if (segment == MHoverSegment) APainter->setFillColor( KODE_Color(0.7,0.7,0.7) );
          else APainter->setFillColor(MSegmentBackColor);
          APainter->fillRectangle(x2+1+trunc(ss),y1,x2+1+trunc(se),y2-1);
          // { name }
          APainter->setTextColor(MSegmentTextColor);
          APainter->drawText(x2+1+KODE_Trunc(ss),y1,x2+1+KODE_Trunc(se),y2,segment->getName(),KODE_TEXT_ALIGN_CENTER);
          // { border }
          APainter->setDrawColor(MBorderColor);
          //APainter->drawRectangle(x2+1+trunc(ss),y1,x2+1+trunc(se),y2);

          //APainter->drawLine(x2+1+KTrunc(ss),y1,x2+1+KTrunc(ss),y2);
          //APainter->drawLine(x2+1+KTrunc(se),y1,x2+1+KTrunc(se),y2);
          APainter->drawVLine(x2+1+KODE_Trunc(ss),y1,y2);
          APainter->drawVLine(x2+1+KODE_Trunc(se),y1,y2);

          // { resize indicators }
          if (segment == MHoverSegment) {
            APainter->setFillColor( KODE_DarkRed );
            if (MHoverLeft)   APainter->fillRectangle(x2+1+KODE_Trunc(ss),y1,x2+1+KODE_Trunc(ss)+(DRAG_HANDLE_SIZE-1),y2);
            if (MHoverRight)  APainter->fillRectangle(x2+1+KODE_Trunc(se)-(DRAG_HANDLE_SIZE-1),y1,x2+1+KODE_Trunc(se),y2);
          }

          //snode = snode->next();
          segment = (KODE_TimelineSegment*)segment->next();
        } // while segment

        y1 += TRACK_HEIGHT;

        //tnode = tnode->next();
        track = (KODE_TimelineTrack*)track->next();
      } // while tnode

      float cur = MCursor * MZoom * PIXELS_PER_SECOND;
      APainter->setDrawColor( KODE_LightRed );

      //APainter->drawLine( MRect.x + KTrunc(cur), MRect.y, MRect.x + KTrunc(cur), MRect.y2() );
      APainter->drawVLine( MRect.x + KODE_Trunc(cur), MRect.y, MRect.y2() );

      //KWidget::on_widgetPaint(AWidget,APainter,ARect,AMode);
    }

    //----------

    void on_leave(float AXpos, float AYpos, KODE_Widget* AFrom=nullptr) override {
      bool changed = false;
      if (MHoverTrack) {
        MHoverTrack = KODE_NULL;
        changed = true;
      }
      if (MHoverSegment) {
        MHoverSegment = KODE_NULL;
        changed = true;
      }
      if (changed) do_redraw(this,MRect);
      //KWidget::on_widgetLeave(AWidget, int32_t AXpos, int32_t AYpos);
    }

  //----------

};

//----------

#undef TRACK_HEIGHT
#undef TRACK_WIDTH
#undef PIXELS_PER_SECOND
#undef SECONDS_PER_PIXEL
#undef DRAG_HANDLE_SIZE

//----------------------------------------------------------------------
#endif

#endif // 0
