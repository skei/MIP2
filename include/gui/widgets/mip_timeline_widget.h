#ifndef mip_timeline_widget_included
#define mip_timeline_widget_included
//----------------------------------------------------------------------

#include "base/types/mip_list.h"
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
    MIP_TimelineTrack*   MHoverTrack       = nullptr;
    MIP_TimelineSegment* MHoverSegment     = nullptr;
    float                 MHoverTime        = -1.0f;
    bool                  MHoverLeft        = false;
    bool                  MHoverRight       = false;
  private:
    MIP_TimelineSegment* MDraggingSegment  = nullptr;
    bool                  MDraggingLeft     = false;
    bool                  MDraggingRight    = false;
  protected:
    MIP_Color  MBackColor        = MIP_COLOR_GRAY;
    MIP_Color  MTrackBackColor   = MIP_COLOR_DARK_GRAY;
    MIP_Color  MTrackTextColor   = MIP_COLOR_LIGHT_GRAY;
    MIP_Color  MSegmentBackColor = MIP_COLOR_LIGHT_GRAY;
    MIP_Color  MSegmentTextColor = MIP_COLOR_DARK_GRAY;
    MIP_Color  MBorderColor      = MIP_COLOR_BLACK;
    MIP_Color  MTextColor        = MIP_COLOR_RED;

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

    MIP_TimelineWidget(MIP_FRect ARect)
    : MIP_Widget(ARect) {
      MName = "MIP_TimelineWidget";
      //setFlag(MIP_WIDGET_HOVER);
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
      if (y < 0) return nullptr;
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
      return nullptr;

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

    void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
      bool changed = false;
      if (AButton == MIP_BUTTON_LEFT) {
        MPrevMouseX = AXpos;
        MPrevMouseY = AYpos;
        if (!MHoverSegment) return;
        MDraggingSegment = MHoverSegment;
        MDraggingLeft = MHoverLeft;
        MDraggingRight = MHoverRight;
      }
      if (changed) do_widget_redraw(this,MRect);
      //KWidget::on_widgetMouseDown(AWidget,AXpos,AYpos,AButton,AState);
    }

    //----------

    void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp) override {
      bool changed = false;
      if (AButton == MIP_BUTTON_LEFT) {
        //if (!MDraggingSegment) return;
        MDraggingSegment = nullptr;
        MDraggingLeft = false;
        MDraggingRight = false;
      }
      if (changed) do_widget_redraw(this,MRect);
      //KWidget::on_widgetMouseUp(AWidget,AXpos,AYpos,AButton,AState);
    }

    //----------

    void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp) override {
      bool changed = false;
      MIP_TimelineTrack* track = findTrack(AYpos);
      MIP_TimelineSegment* segment = nullptr;
      MHoverTime = calcTime(AXpos - MRect.x);
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
      if (changed) do_widget_redraw(this,MRect);
      MPrevMouseX = AXpos;
      MPrevMouseY = AYpos;
    }

    //----------

    void on_widget_paint(MIP_Painter* APainter, MIP_FRect ARect, uint32_t AMode=0) override {

      APainter->setLineWidth(1);

      APainter->setColor(MBackColor);
      APainter->rectangle(MRect);
      APainter->fillPath();
      APainter->setColor(MBorderColor);
      APainter->horizLine(MRect.x,MRect.y,MRect.x2());
      APainter->strokePath();

      float x1 = MRect.x;
      float y1 = MRect.y;
      float x2 = x1 + TRACK_WIDTH;
      float y2 = y1 + TRACK_HEIGHT;

      MIP_TimelineTrack* track = (MIP_TimelineTrack*)MTracks.head();
      while (track) {

        MIP_FRect rect;

        // { track background }

        if (track == MHoverTrack) APainter->setColor(0.3);
        else APainter->setColor(MTrackBackColor);
        rect = MIP_FRect(x1,y1,TRACK_WIDTH,TRACK_HEIGHT);
        APainter->rectangle(rect);
        APainter->fillPath();

        // { track name }

        APainter->setColor(MTrackTextColor);
        rect.x += 5;
        rect.w -= 5;
        APainter->drawText(rect,track->getName(), MIP_TEXT_ALIGN_LEFT);//CENTER);

        // { track border }

        APainter->setColor(MBorderColor);       // below
        APainter->horizLine(x1,y2,MRect.x2());  // below
        APainter->vertLine(x2,y1,y2);           // right
        APainter->strokePath();

        // { track segments }

        MIP_TimelineSegment* segment = (MIP_TimelineSegment*)track->getSegments();
        while (segment) {

          float ss = segment->getStart() * MZoom * PIXELS_PER_SECOND;
          float se = segment->getEnd() * MZoom * PIXELS_PER_SECOND;
          float sw = se - ss;

          rect = MIP_FRect(x2+ss,y1,sw,TRACK_HEIGHT);

          // { back }

          if (segment == MHoverSegment) APainter->setColor(0.7);
          else APainter->setColor(MSegmentBackColor);
          APainter->rectangle(rect);
          APainter->fillPath();

          // { name }

          APainter->setColor(MSegmentTextColor);
          MIP_FRect rect2 = rect;
          rect2.x += 5;
          rect2.w -= 5;
          APainter->drawText(rect2,segment->getName(),MIP_TEXT_ALIGN_LEFT);//CENTER);

          // { border }

          //APainter->setColor(MBorderColor);
          //APainter->vertLine(x2+ss,y1,y2);
          //APainter->strokePath();
          //APainter->vertLine(x2+se,y1,y2);
          //APainter->strokePath();

          APainter->setColor(MBorderColor);
          APainter->rectangle(rect);
          APainter->strokePath();

          // { resize indicators }

          if (segment == MHoverSegment) {
            APainter->setColor( MIP_COLOR_BRIGHT_BLUE1 );
            if (MHoverLeft) {
              rect = MIP_FRect(x2+ss,y1,DRAG_HANDLE_SIZE,TRACK_HEIGHT);
              APainter->rectangle(rect);
              APainter->fillPath();
            }
            if (MHoverRight) {
              rect = MIP_FRect(x2+1+MIP_Trunc(se)-(DRAG_HANDLE_SIZE),y1,DRAG_HANDLE_SIZE,TRACK_HEIGHT);
              APainter->rectangle(rect);
              APainter->fillPath();
            }
          }

          segment = (MIP_TimelineSegment*)segment->next();
        } // while segment
        y1 += TRACK_HEIGHT;
        track = (MIP_TimelineTrack*)track->next();
      } // while tnode
      float cur = MCursor * MZoom * PIXELS_PER_SECOND;
      APainter->setColor( MIP_COLOR_LIGHT_RED );
      APainter->vertLine( x2 + cur, MRect.y, MRect.y2() );
      APainter->strokePath();
    }

    //----------

    void on_widget_mouseLeave(float AXpos, float AYpos, MIP_Widget* ATo=nullptr) override {
      bool changed = false;
      if (MHoverTrack) {
        MHoverTrack = nullptr;
        changed = true;
      }
      if (MHoverSegment) {
        MHoverSegment = nullptr;
        changed = true;
      }
      if (changed) do_widget_redraw(this,MRect);
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
