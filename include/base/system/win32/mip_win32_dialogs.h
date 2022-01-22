#ifndef mip_win32_dialogs_included
#define mip_win32_dialogs_included
//----------------------------------------------------------------------

#include "base/utils/mip_convert.h"

//----------------------------------------------------------------------
// win32
//----------------------------------------------------------------------

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms645524%28v=vs.85%29.aspx
#ifdef MIP_WIN32

//TODO?

#include <windows.h>
#include <commdlg.h>

// link with comdlg32

//----------

const char* MIP_Dialog_ColorSelect(const char* ATitle, bool APalette=true) {
  CHOOSECOLOR choose_color;
  COLORREF custom_colors[16];
  ZeroMemory(&choose_color, sizeof(choose_color));
  choose_color.lStructSize = sizeof (CHOOSECOLOR);
  choose_color.rgbResult = RGB(0,0,0);//old_attr;
  choose_color.lpCustColors = custom_colors;
  choose_color.Flags = CC_ANYCOLOR | CC_RGBINIT | CC_FULLOPEN;
  if (ChooseColor(&choose_color) == TRUE) {
    MIP_Trace("%08x\n",choose_color.rgbResult); // ABGR
    //return (int) cc.rgbResult;
  } else {
    MIP_Trace("---\n");
    //return -1;
  }
  return nullptr;
}

const char* MIP_Dialog_Error(const char* ATitle, const char* AText) {
//  strcpy(mip_zenity_buffer,"zenity --error");
//  strcat(mip_zenity_buffer," --text='");    strcat(mip_zenity_buffer,AText);    strcat(mip_zenity_buffer,"'");
//  return mip_do_zenity();
  return nullptr;
}

const char* SDialog_FileOpen(const char* ATitle, bool AMultiple=false) {
//  strcpy(mip_zenity_buffer,"zenity --file-selection");
//  strcat(mip_zenity_buffer," --title='");    strcat(mip_zenity_buffer,ATitle);    strcat(mip_zenity_buffer,"'");
//  if (AMultiple) {
//    strcat(mip_zenity_buffer," --multiple");
//    strcat(mip_zenity_buffer," --separator=':'");
//  }
//  return mip_do_zenity();
  return nullptr;
}

const char* MIP_Dialog_FileSave(const char* ATitle, const char* AFilename) {
//  strcpy(mip_zenity_buffer,"zenity --file-selection");
//  strcat(mip_zenity_buffer," --save");
//  strcat(mip_zenity_buffer," --title='");    strcat(mip_zenity_buffer,ATitle);    strcat(mip_zenity_buffer,"'");
//  strcat(mip_zenity_buffer," --filename='"); strcat(mip_zenity_buffer,AFilename); strcat(mip_zenity_buffer,"'");
//  return mip_do_zenity();
  return nullptr;
}

const char* MIP_Dialog_Info(const char* ATitle, const char* AText) {
//  strcpy(mip_zenity_buffer,"zenity --info");
//  strcat(mip_zenity_buffer," --text='");    strcat(mip_zenity_buffer,AText);    strcat(mip_zenity_buffer,"'");
//  return mip_do_zenity();
  return nullptr;
}

const char* MIP_Dialog_Question(const char* ATitle, const char* AText) {
//  strcpy(mip_zenity_buffer,"zenity --question");
//  strcat(mip_zenity_buffer," --text='");    strcat(mip_zenity_buffer,AText);    strcat(mip_zenity_buffer,"'");
//  return mip_do_zenity();
  return nullptr;
}

const char* MIP_Dialog_Text(const char* ATitle, const char* AText, const char* AEntryText) {
//  strcpy(mip_zenity_buffer,"zenity --entry");
//  strcat(mip_zenity_buffer," --text='");    strcat(mip_zenity_buffer,AText);            strcat(mip_zenity_buffer,"'");
//  strcat(mip_zenity_buffer," --entry-text='");    strcat(mip_zenity_buffer,AEntryText); strcat(mip_zenity_buffer,"'");
//  return mip_do_zenity();
  return nullptr;
}

// segmentation fault

const char* MIP_Dialog_TextInfo(const char* ATitle, const char* AFilename, bool AEditable=false, bool AHtml=false, const char* AUrl=nullptr) {
//  strcpy(mip_zenity_buffer,"zenity --text-info");
//  strcat(mip_zenity_buffer," --filename='");       strcat(mip_zenity_buffer,AFilename);   strcat(mip_zenity_buffer,"'");
//  if (AEditable) strcat(mip_zenity_buffer," --editable");
//  if (AHtml) strcat(mip_zenity_buffer," --html");
//  if (AUrl) strcat(mip_zenity_buffer," --url='");  strcat(mip_zenity_buffer,AUrl);        strcat(mip_zenity_buffer,"'");
//  return mip_do_zenity();
  return nullptr;
}

const char* MIP_Dialog_Value(const char* ATitle, const char* AText, int AVal=0, int AMin=0, int AMax=100, int AStep=1) {
//  char temp[256];
//  strcpy(mip_zenity_buffer,"zenity --scale");
//  strcat(mip_zenity_buffer," --text='");       strcat(mip_zenity_buffer,AText); strcat(mip_zenity_buffer,"'");
//  strcat(mip_zenity_buffer," --value='");      strcat(mip_zenity_buffer, SIntToString(temp,AVal));  strcat(mip_zenity_buffer,"'");
//  strcat(mip_zenity_buffer," --min-value='");  strcat(mip_zenity_buffer, SIntToString(temp,AMin));  strcat(mip_zenity_buffer,"'");
//  strcat(mip_zenity_buffer," --max-value='");  strcat(mip_zenity_buffer, SIntToString(temp,AMax));  strcat(mip_zenity_buffer,"'");
//  strcat(mip_zenity_buffer," --step='");       strcat(mip_zenity_buffer, SIntToString(temp,AStep)); strcat(mip_zenity_buffer,"'");
//  //strcat(mip_zenity_buffer," --print-partial");
//  //strcat(mip_zenity_buffer," --hide-value");
//  return mip_do_zenity();
  return nullptr;
}

const char* MIP_Dialog_Warning(const char* ATitle, const char* AText) {
//  strcpy(mip_zenity_buffer,"zenity --warning");
//  strcat(mip_zenity_buffer," --text='");    strcat(mip_zenity_buffer,AText);    strcat(mip_zenity_buffer,"'");
//  return mip_do_zenity();
  return nullptr;
}

//----------------------------------------------------------------------
#endif

