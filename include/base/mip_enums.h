#ifndef mip_enums_included
#define mip_enums_included
//----------------------------------------------------------------------

// hex = ored bitflags
// dec = select one

//----------------------------------------------------------------------
//
// general
//
//----------------------------------------------------------------------

enum MIP_EDirections {
  MIP_UP                         = 0,
  MIP_DOWN                       = 1,
  MIP_LEFT                       = 2,
  MIP_RIGHT                      = 3,
  MIP_HORIZONTAL                 = 0,
  MIP_VERTICAL                   = 1
};

enum MIP_EEdges {
  MIP_EDGE_NONE                  = 0x00,
  MIP_EDGE_TOP                   = 0x01,
  MIP_EDGE_BOTTOM                = 0x02,
  MIP_EDGE_LEFT                  = 0x04,
  MIP_EDGE_RIGHT                 = 0x08,
  MIP_EDGES_ALL                  = 0x0f
};

enum MIP_ECorners {
  MIP_CORNER_NONE                = 0x00,
  MIP_CORNER_LEFT_TOP            = 0x01,
  MIP_CORNER_LEFT_BOTTOM         = 0x02,
  MIP_CORNER_RIGHT_TOP           = 0x04,
  MIP_CORNER_RIGHT_BOTTOM        = 0x08
};

//----------------------------------------------------------------------
//
// base
//
//----------------------------------------------------------------------

enum MIP_EThreadIds {
  MIP_THREAD_ID_NONE             = 0,
  MIP_THREAD_ID_KILL             = 666,
  MIP_THREAD_ID_TIMER            = 667,
  MIP_THREAD_ID_IDLE             = 668
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

enum MIP_EParameterUpdateModes {
  MIP_PARAMETER_UPDATE_NORMAL    = 0,
  MIP_PARAMETER_UPDATE_ALL       = 1//,
  //MIP_PARAMETER_UPDATE_PROCESS   = 2
};

enum MIP_EPluginEventTypes {
  MIP_PLUGIN_EVENT_NONE          = 0,
  MIP_PLUGIN_EVENT_PARAMETER     = 1,
  MIP_PLUGIN_EVENT_MIDI          = 2
};

enum MIP_EPluginFormat {
  MIP_PLUGIN_FORMAT_NONE         = 0,
  MIP_PLUGIN_FORMAT_EXE          = 1,
  MIP_PLUGIN_FORMAT_LADSPA       = 2,
  MIP_PLUGIN_FORMAT_DSSI         = 3,
  MIP_PLUGIN_FORMAT_LV2          = 4,
  MIP_PLUGIN_FORMAT_VST2         = 5,
  MIP_PLUGIN_FORMAT_VST3         = 6,
  MIP_PLUGIN_FORMAT_CLAP         = 7
};


enum MIP_EPluginPlayStates {
  MIP_PLUGIN_PLAYSTATE_NONE      = 0x00,
  MIP_PLUGIN_PLAYSTATE_PLAYING   = 0x01,
  MIP_PLUGIN_PLAYSTATE_RECORDING = 0x02,
  MIP_PLUGIN_PLAYSTATE_LOOPING   = 0x04
};

enum MIP_EPluginStates {
  MIP_PLUGIN_STATE_NONE          = 0,
  MIP_PLUGIN_STATE_OPEN          = 1,
  MIP_PLUGIN_STATE_INITIALIZE    = 2,
  MIP_PLUGIN_STATE_ACTIVATE      = 3,
  MIP_PLUGIN_STATE_DEACTIVATE    = 4,
  MIP_PLUGIN_STATE_TERMINATE     = 5,
  MIP_PLUGIN_STATE_CLOSE         = 6
};

//----------------------------------------------------------------------
//
// gui
//
//----------------------------------------------------------------------

enum MIP_EMouseCursors {
  MIP_CURSOR_GRAB                = -4,
  MIP_CURSOR_RELEASE             = -3,
  MIP_CURSOR_SHOW                = -2,
  MIP_CURSOR_HIDE                = -1,
  MIP_CURSOR_DEFAULT             = 0,
  MIP_CURSOR_ARROW               = 1,
  MIP_CURSOR_ARROWUP             = 2,
  MIP_CURSOR_ARROWDOWN           = 3,
  MIP_CURSOR_ARROWLEFT           = 4,
  MIP_CURSOR_ARROWRIGHT          = 5,
  MIP_CURSOR_ARROWUPDOWN         = 6,
  MIP_CURSOR_ARROWLEFTRIGHT      = 7,
  MIP_CURSOR_ARROWDIAGLEFT       = 8,
  MIP_CURSOR_ARROWDIAGRIGHT      = 9,
  MIP_CURSOR_MOVE                = 10,
  MIP_CURSOR_WAIT                = 11,
  MIP_CURSOR_ARROWWAIT           = 12,
  MIP_CURSOR_HAND                = 13,
  MIP_CURSOR_FINGER              = 14,
  MIP_CURSOR_CROSS               = 15,
  MIP_CURSOR_PENCIL              = 16,
  MIP_CURSOR_PLUS                = 17,
  MIP_CURSOR_QUESTION            = 18,
  MIP_CURSOR_IBEAM               = 19
};

enum MIP_EMouseButtons {
  MIP_BUTTON_NONE                = 0,
  MIP_BUTTON_LEFT                = 1,
  MIP_BUTTON_MIDDLE              = 2,
  MIP_BUTTON_RIGHT               = 3,
  MIP_BUTTON_SCROLL_UP           = 4,
  MIP_BUTTON_SCROLL_DOWN         = 5,
  MIP_BUTTON_BACKWARD            = 8,
  MIP_BUTTON_FORWARD             = 9
};

enum MIP_EKeyState {
  MIP_KEY_NONE                   = 0x00,
  MIP_KEY_SHIFT                  = 0x01,
  MIP_KEY_CAPS                   = 0x02,
  MIP_KEY_CTRL                   = 0x04,
  MIP_KEY_ALT                    = 0x08,
  MIP_KEY_ALTGR                  = 0x10
};

enum MIP_EKeyChar {
  MIP_KEY_ENTER      = 13,
  MIP_KEY_ESC        = 27,
  MIP_KEY_HOME       = 36,
  MIP_KEY_END        = 35,
  MIP_KEY_LEFT       = 37,
  MIP_KEY_RIGHT      = 39,
  MIP_KEY_DELETE     = 46,
  MIP_KEY_BACKSPACE  = 8
};

enum MIP_EWidgetPaintModes {
  MIP_WIDGET_PAINT_NORMAL        = 0x00,
  MIP_WIDGET_PAINT_FULL          = 0x00,
  MIP_WIDGET_PAINT_VALUE         = 0x01,
  MIP_WIDGET_PAINT_MODULATION    = 0x02
};


enum MIP_EWidgetAlignment {
  MIP_WIDGET_ALIGN_NONE                = 0,  // relative to window
  MIP_WIDGET_ALIGN_PARENT              = 1,  // relative to parent
  MIP_WIDGET_ALIGN_CLIENT              = 2,  // relative to current client area
  MIP_WIDGET_ALIGN_LEFT                = 3,
  MIP_WIDGET_ALIGN_RIGHT               = 4,
  MIP_WIDGET_ALIGN_TOP                 = 5,
  MIP_WIDGET_ALIGN_BOTTOM              = 6,
  MIP_WIDGET_ALIGN_LEFT_TOP            = 7,
  MIP_WIDGET_ALIGN_LEFT_CENTER         = 8,
  MIP_WIDGET_ALIGN_LEFT_BOTTOM         = 9,
  MIP_WIDGET_ALIGN_RIGHT_TOP           = 10,
  MIP_WIDGET_ALIGN_RIGHT_CENTER        = 11,
  MIP_WIDGET_ALIGN_RIGHT_BOTTOM        = 12,
  MIP_WIDGET_ALIGN_TOP_LEFT            = 13,
  MIP_WIDGET_ALIGN_TOP_CENTER          = 14,
  MIP_WIDGET_ALIGN_TOP_RIGHT           = 15,
  MIP_WIDGET_ALIGN_BOTTOM_LEFT         = 16,
  MIP_WIDGET_ALIGN_BOTTOM_CENTER       = 17,
  MIP_WIDGET_ALIGN_BOTTOM_RIGHT        = 18,
  MIP_WIDGET_ALIGN_FILL_CLIENT         = 19,
  MIP_WIDGET_ALIGN_FILL_LEFT           = 20,
  MIP_WIDGET_ALIGN_FILL_LEFT_TOP       = 21,
  MIP_WIDGET_ALIGN_FILL_LEFT_CENTER    = 22,
  MIP_WIDGET_ALIGN_FILL_LEFT_BOTTOM    = 23,
  MIP_WIDGET_ALIGN_FILL_RIGHT          = 24,
  MIP_WIDGET_ALIGN_FILL_RIGHT_TOP      = 25,
  MIP_WIDGET_ALIGN_FILL_RIGHT_CENTER   = 26,
  MIP_WIDGET_ALIGN_FILL_RIGHT_BOTTOM   = 27,
  MIP_WIDGET_ALIGN_FILL_TOP            = 28,
  MIP_WIDGET_ALIGN_FILL_TOP_LEFT       = 29,
  MIP_WIDGET_ALIGN_FILL_TOP_CENTER     = 30,
  MIP_WIDGET_ALIGN_FILL_TOP_RIGHT      = 31,
  MIP_WIDGET_ALIGN_FILL_BOTTOM         = 32,
  MIP_WIDGET_ALIGN_FILL_BOTTOM_LEFT    = 33,
  MIP_WIDGET_ALIGN_FILL_BOTTOM_CENTER  = 34,
  MIP_WIDGET_ALIGN_FILL_BOTTOM_RIGHT   = 35,
  MIP_WIDGET_ALIGN_CENTER              = 36,
  MIP_WIDGET_ALIGN_CENTER_HORIZ        = 37,
  MIP_WIDGET_ALIGN_CENTER_VERT         = 38,
  MIP_WIDGET_ALIGN_STACK_HORIZ         = 39,
  MIP_WIDGET_ALIGN_STACK_VERT          = 40

};

//enum MIP_EWidgetAnchors {
//  MIP_WIDGET_ANCHOR_NONE         = 0x00,
//  MIP_WIDGET_ANCHOR_LEFT         = 0x01,
//  MIP_WIDGET_ANCHOR_TOP          = 0x02,
//  MIP_WIDGET_ANCHOR_RIGHT        = 0x04,
//  MIP_WIDGET_ANCHOR_BOTTOM       = 0x08
//};

//enum MIP_EWidgetTypes {
//  MIP_WIDGET_TYPE_NONE           = 0,
//  MIP_WIDGET_TYPE_WIDGET         = 1,
//  MIP_WIDGET_TYPE_WINDOW         = 2
//};

//----------------------------------------------------------------------
//
// painting
//
//----------------------------------------------------------------------

enum MIP_EFontSlants {
  MIP_FONT_SLANT_NORMAL          = 0,
  MIP_FONT_SLANT_ITALIC          = 1,
  MIP_FONT_SLANT_OBLIQUE         = 2
};

enum MIP_EFfontWeights {
  MIP_FONT_WEIGHT_NORMAL         = 0,
  MIP_FONT_WEIGHT_BOLD           = 1
};

enum MIP_ELineCaps {
  MIP_LINE_CAP_BUTT              = 0,
  MIP_LINE_CAP_ROUND             = 1,
  MIP_LINE_CAP_SQUARE            = 2
};

enum MIP_ELineJoins {
  MIP_LINE_JOIN_MITER            = 0,
  MIP_LINE_JOIN_ROUND            = 1,
  MIP_LINE_JOIN_BEVEL            = 2
};

enum MIP_ESymbols {
  MIP_SYMBOL_NONE                = 0,
  MIP_SYMBOL_HLINE               = 1,
  MIP_SYMBOL_VLINE               = 2,
  MIP_SYMBOL_LINE                = 3,
  MIP_SYMBOL_RECT                = 4,
  MIP_SYMBOL_FILL_RECT           = 5,
  MIP_SYMBOL_TRI_DOWN            = 6,
  MIP_SYMBOL_FILL_TRI_DOWN       = 7,
  MIP_SYMBOL_ELLIPSE             = 8,
  MIP_SYMBOL_FILL_ELLIPSE        = 9
};

enum MIP_ETextAlignment {
  MIP_TEXT_ALIGN_NONE            = 0x00,
  MIP_TEXT_ALIGN_TOP             = 0x01,
  MIP_TEXT_ALIGN_BOTTOM          = 0x02,
  MIP_TEXT_ALIGN_LEFT            = 0x04,
  MIP_TEXT_ALIGN_RIGHT           = 0x08,
  MIP_TEXT_ALIGN_CENTER          = 0
};

//----------------------------------------------------------------------
//
// audio
//
//----------------------------------------------------------------------

enum MIP_EPluginPortType {
  MIP_PLUGIN_PORT_AUDIO = 0,
  MIP_PLUGIN_PORT_MIDI  = 1,
};

enum MIP_EPluginPortDirection {
  MIP_PLUGIN_PORT_INPUT   = 0,
  MIP_PLUGIN_PORT_OUTPUT  = 1,
};

enum MIP_EVoiceState {
  MIP_VOICE_OFF      = 0,
  MIP_VOICE_PLAYING  = 1,
  MIP_VOICE_RELEASED = 2,
  MIP_VOICE_FINISHED = 3
};

enum MIP_ENoteExpression {
  MIP_NOTE_EXPRESSION_VOLUME      = 0,
  MIP_NOTE_EXPRESSION_PAN         = 1,
  MIP_NOTE_EXPRESSION_TUNING      = 2,
  MIP_NOTE_EXPRESSION_VIBRATO     = 3,
  MIP_NOTE_EXPRESSION_BRIGHTNESS  = 4,
  MIP_NOTE_EXPRESSION_BREATH      = 5,
  MIP_NOTE_EXPRESSION_PRESSURE    = 6,
  MIP_NOTE_EXPRESSION_TIMBRE      = 7
};

//----------------------------------------------------------------------
#endif
