#if 0

#ifndef mip_timeline_widget_included
#define mip_timeline_widget_included
//----------------------------------------------------------------------

#include "base/mip_list.h"
#include "gui/mip_widget.h"

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

class MIP_TimelineSegment
: public MIP_ListNode {

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

    MIP_TimelineSegment(const char* AName, float AStart, float AEnd)
    : MIP_ListNode() {
      MName     = AName;
      MStartPos = AStart;
      MEndPos   = AEnd;
      MSelected = false;
    }

    //----------

    virtual ~MIP_TimelineSegment() {
    }

    //----------

    bool isInside(float ATime) {
      if ((ATime >= MStartPos) && (ATime <= MEndPos)) return true;
      else return false;
    }

};

//typedef list<MIP_TimelineSegment*> MIP_TimelineSegments;
//typedef MIP_Array<MIP_TimelineSegment*> MIP_TimelineSegments;

//----------------------------------------------------------------------
// track
//----------------------------------------------------------------------

class MIP_TimelineTrack
: public MIP_ListNode {

  private:

    const char* MName     = "track";
    bool        MSelected = false;
    MIP_List   MSegments;
    //MIP_TimelineSegments MSegments;

  public:

    const char*     getName(void) { return MName; }
    void            setName(char* AName) { MName = AName; }
    void            select(float ASelected=true) { MSelected = ASelected; }
    bool            isSelected(void) { return MSelected; }
    MIP_ListNode*  getSegments(void) { return MSegments.head(); }
    //MIP_TimelineSegments*  getSegments(void) { return &MSegments; }

  public:

    MIP_TimelineTrack(const char* AName)
    : MIP_ListNode() {
      MName = AName;
      MSelected = false;
    }

    //----------

    virtual ~MIP_TimelineTrack() {
      #ifndef MIP_NO_AUTODELETE
      deleteSegments();
      #endif
    }

    //----------

    void addSegment(MIP_TimelineSegment* ASegment) {
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

//typedef MIP_Array<MIP_TimelineTrack*> MIP_TimelineTracks;

//----------------------------------------------------------------------
// track
//----------------------------------------------------------------------

class MIP_TimelineWidget
: public MIP_Widget {

  private:
    float                 MZoom             = 1.0f;
    float                 MStartPos         = 0.0f;
    int32_t               MPrevMouseX       = 0;
    int32_t               MPrevMouseY       = 0;
  private:
    MIP_TimelineTrack*   MHoverTrack       = MIP_NULL;
    MIP_TimelineSegment* MHoverSegment     = MIP_NULL;
    float                 MHoverTime        = -1.0f;
    bool                  MHoverLeft        = false;
    bool                  MHoverRight       = false;
  private:
    MIP_TimelineSegment* MDraggingSegment  = MIP_NULL;
    bool                  MDraggingLeft     = false;
    bool                  MDraggingRight    = false;
  protected:
    MIP_Color  MBackColor        = MIP_Grey;
    MIP_Color  MTrackBackColor   = MIP_DarkGrey;
    MIP_Color  MTrackTextColor   = MIP_LightGrey;
    MIP_Color  MSegmentBackColor = MIP_LightGrey;
    MIP_Color  MSegmentTextColor = MIP_DarkGrey;
    MIP_Color  MBorderColor      = MIP_Black;
    MIP_Color  MTextColor        = MIP_Red;

  private:

    //MIP_TimelineTracks   MTracks;

    MIP_List MTracks;
//    int32_t   MCursor;

  //------------------------------
  //
  //------------------------------

  public:

    //MIP_TimelineTracks*  getTracks(void)            { return &MTracks; }
    MIP_ListNode*  getTracks(void)            { return MTracks.head(); }
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

    MIP_TimelineWidget(MIP_Rect ARect)
    : MIP_Widget(ARect) {
      MName = "MIP_TimelineWidget";
      setFlag(MIP_WIDGET_HOVER);
      //setFlag(MIP_WIDGET_CLIP);
    }

    //----------

    virtual ~MIP_TimelineWidget() {
      #ifndef MIP_NO_AUTODELETE
      deleteTracks();
      #endif
    }

  //------------------------------
  //
  //------------------------------

  public:

    void addTrack(MIP_TimelineTrack* ATrack) {
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
      return MIP_Trunc( pixel*MZoom );
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
        return  TRACK_WIDTH + MIP_Trunc(pixel);
      }
      return -1;
    }

    //----------

    /*
      TODO:
      - each track has individual height

    */

    MIP_TimelineTrack* findTrack(int32_t AYpos) {
      int32_t y = AYpos - MRect.y;
      if (y < 0) return MIP_NULL;
      int32_t t = y / TRACK_HEIGHT;

      MIP_ListNode* node = (MIP_ListNode*)MTracks.head();
      while (node) {
        if (t == 0) return (MIP_TimelineTrack*)node;
        t -= 1;
        node = node->next();
      }
      return nullptr;

      //MIP_TimelineTracks::iterator iter = MTracks.begin();
      //while (iter != MTracks.end()) {
      //  MIP_TimelineTrack* track = *iter;
      //  if (t == 0) return track;
      //  t -= 1;
      //}

    }

    //----------

    MIP_TimelineSegment* findSegment(MIP_TimelineTrack* ATrack, float ATime) {

      //MIP_ListNode* node = (MIP_ListNode*)ATrack->getSegments()->head();
      MIP_ListNode* node = ATrack->getSegments();
      while (node) {
        MIP_TimelineSegment* segment = (MIP_TimelineSegment*)node;
        if ( (ATime >= segment->getStart()) && (ATime <= segment->getEnd()) ) return segment;
        node = node->next();
      }
      return MIP_NULL;

      //MIP_TimelineSegments* segments = ATrack->getSegments();
      //MIP_TimelineSegments::iterator iter = segments->begin();
      //while (iter != segments->end()) {
      //  MIP_TimelineSegment* segment = *iter;
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
      if (AButton == MIP_BUTTON_LEFT) {
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
      if (AButton == MIP_BUTTON_LEFT) {
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
      MIP_TimelineTrack* track = findTrack(AYpos);
      MIP_TimelineSegment* segment = nullptr;

      MHoverTime = calcTime(AXpos - MRect.x);
      //KTrace("MHoverTime %.3f\n",MHoverTime);

      if (MDraggingSegment) {

        float mintime = calcLength(10); // minimum 10 pixels
        float diff = calcTimeDiff(AXpos,MPrevMouseX);
        float st = MDraggingSegment->getStart();
        float en = MDraggingSegment->getEnd();

        if (MDraggingLeft) {
          st += diff;
          st = MIP_Max(st,0.0f);
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
          st = MIP_Max(st,0.0f);
          en = st + le;
        }

//        MIP_TimelineSegment* prv = (MIP_TimelineSegment*)MDraggingSegment->prev();
//        if (prv) {
//          if (st <= prv->getEnd()) {
//            float le = en - st;
//            st = prv->getEnd(); //+ 0.0001;
//            if (!MDraggingLeft) en = st + le;
//          }
//        }

//        MIP_TimelineSegment* nxt = (MIP_TimelineSegment*)MDraggingSegment->next();
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

    void on_paint(MIP_Painter* APainter, MIP_Rect ARect) override {
      APainter->setFillColor(MBackColor);
      APainter->fillRectangle(MRect.x,MRect.y,MRect.x2(),MRect.y2());
      APainter->setDrawColor(MBorderColor);
      //APainter->drawLine(MRect.x,MRect.y,MRect.x2(),MRect.y);
      APainter->drawHLine(MRect.x,MRect.y,MRect.x2());
      int32_t x1 = MRect.x;
      //int32_t x2 := MRect.x2;
      int32_t y1 = MRect.y;

      //MIP_ListNode* tnode = (MIP_ListNode*)MTracks.head();
      //while (tnode) {
        //MIP_TimelineTrack* track = (MIP_TimelineTrack*)tnode;

      //MIP_TimelineTrack* track = MTracks.head();
      MIP_TimelineTrack* track = (MIP_TimelineTrack*)MTracks.head();
      while (track) {
        //MIP_Trace("track %p\n",track);

        int32_t x2 = x1 + TRACK_WIDTH - 1;
        int32_t y2 = y1 + TRACK_HEIGHT - 1;
        // { track background }
        if (track == MHoverTrack) APainter->setFillColor( MIP_Color(0.3,0.3,0.3) );
        else APainter->setFillColor(MTrackBackColor);
        APainter->fillRectangle(x1,y1,x2,y2);
        // { track name }
        APainter->setTextColor(MTrackTextColor);
        APainter->drawText(x1+2,y1,x2-2,y2,track->getName(), MIP_TEXT_ALIGN_CENTER);
        // { track border }
        APainter->setDrawColor(MBorderColor);   // below

        //APainter->drawLine(x1,y2,MRect.x2(),y2);  // below
        //APainter->drawLine(x2,y1,x2,y2);        // right

        APainter->drawHLine(x1,y2,MRect.x2());  // below
        APainter->drawVLine(x2,y1,y2);        // right


        // { track segments }

        //MIP_ListNode* snode = (MIP_ListNode*)track->getSegments()->head();
        //while (snode) {
        //  MIP_TimelineSegment* segment = (MIP_TimelineSegment*)snode;

        //MIP_TimelineSegment* segment = track->getSegments()->head();
      MIP_TimelineSegment* segment = (MIP_TimelineSegment*)track->getSegments();

        while (segment) {
          //MIP_Trace("track %p\n",track);

          float ss = segment->getStart() * MZoom * PIXELS_PER_SECOND;
          float se = segment->getEnd() * MZoom * PIXELS_PER_SECOND;
          // { back }
          if (segment == MHoverSegment) APainter->setFillColor( MIP_Color(0.7,0.7,0.7) );
          else APainter->setFillColor(MSegmentBackColor);
          APainter->fillRectangle(x2+1+trunc(ss),y1,x2+1+trunc(se),y2-1);
          // { name }
          APainter->setTextColor(MSegmentTextColor);
          APainter->drawText(x2+1+MIP_Trunc(ss),y1,x2+1+MIP_Trunc(se),y2,segment->getName(),MIP_TEXT_ALIGN_CENTER);
          // { border }
          APainter->setDrawColor(MBorderColor);
          //APainter->drawRectangle(x2+1+trunc(ss),y1,x2+1+trunc(se),y2);

          //APainter->drawLine(x2+1+KTrunc(ss),y1,x2+1+KTrunc(ss),y2);
          //APainter->drawLine(x2+1+KTrunc(se),y1,x2+1+KTrunc(se),y2);
          APainter->drawVLine(x2+1+MIP_Trunc(ss),y1,y2);
          APainter->drawVLine(x2+1+MIP_Trunc(se),y1,y2);

          // { resize indicators }
          if (segment == MHoverSegment) {
            APainter->setFillColor( MIP_DarkRed );
            if (MHoverLeft)   APainter->fillRectangle(x2+1+MIP_Trunc(ss),y1,x2+1+MIP_Trunc(ss)+(DRAG_HANDLE_SIZE-1),y2);
            if (MHoverRight)  APainter->fillRectangle(x2+1+MIP_Trunc(se)-(DRAG_HANDLE_SIZE-1),y1,x2+1+MIP_Trunc(se),y2);
          }

          //snode = snode->next();
          segment = (MIP_TimelineSegment*)segment->next();
        } // while segment

        y1 += TRACK_HEIGHT;

        //tnode = tnode->next();
        track = (MIP_TimelineTrack*)track->next();
      } // while tnode

      float cur = MCursor * MZoom * PIXELS_PER_SECOND;
      APainter->setDrawColor( MIP_LightRed );

      //APainter->drawLine( MRect.x + KTrunc(cur), MRect.y, MRect.x + KTrunc(cur), MRect.y2() );
      APainter->drawVLine( MRect.x + MIP_Trunc(cur), MRect.y, MRect.y2() );

      //KWidget::on_widgetPaint(AWidget,APainter,ARect,AMode);
    }

    //----------

    void on_leave(float AXpos, float AYpos, MIP_Widget* AFrom=nullptr) override {
      bool changed = false;
      if (MHoverTrack) {
        MHoverTrack = MIP_NULL;
        changed = true;
      }
      if (MHoverSegment) {
        MHoverSegment = MIP_NULL;
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
