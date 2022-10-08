#ifndef mip_widget_window_included
#define mip_widget_window_included
//----------------------------------------------------------------------

#include "mip.h"
#include "gui/mip_widget.h"
#include "gui/mip_buffered_window.h"
#include "gui/mip_window.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class MIP_WidgetWindow
: public MIP_BufferedWindow
, piblic MIP_Widget {
}

//----------

virtual ~MIP_WidgetWindow() {
}

//----------------------------------------------------------------------
#endif
