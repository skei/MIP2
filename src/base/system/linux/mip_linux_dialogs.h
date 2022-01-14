#ifndef mip_dialogs_included
#define mip_dialogs_included
//----------------------------------------------------------------------

#include "base/utils/mip_convert.h"

// uses zenity
// gnome?

static char mip_zenity_buffer[1024];
static char mip_zenity_filename[1024];

//----------

const char* mip_do_zenity(void) {
  FILE *f = popen(mip_zenity_buffer,"r");
  if ( fgets(mip_zenity_filename,1024,f) ) {
    return mip_zenity_filename;
  }
  else {
    return "";//nullptr;
  }
}

//----------

const char* MIP_Dialog_ColorSelect(const char* ATitle, bool APalette=true) {
  strcpy(mip_zenity_buffer,"zenity --color-selection");
  strcat(mip_zenity_buffer," --title='");    strcat(mip_zenity_buffer,ATitle);    strcat(mip_zenity_buffer,"'");
  if (APalette) strcat(mip_zenity_buffer," --show-palette");
  return mip_do_zenity();
}

const char* Dialog_Error(const char* ATitle, const char* AText) {
  strcpy(mip_zenity_buffer,"zenity --error");
  strcat(mip_zenity_buffer," --text='");    strcat(mip_zenity_buffer,AText);    strcat(mip_zenity_buffer,"'");
  return mip_do_zenity();
}

const char* MIP_Dialog_FileOpen(const char* ATitle, bool AMultiple=false) {
  strcpy(mip_zenity_buffer,"zenity --file-selection");
  strcat(mip_zenity_buffer," --title='");    strcat(mip_zenity_buffer,ATitle);    strcat(mip_zenity_buffer,"'");
  if (AMultiple) {
    strcat(mip_zenity_buffer," --multiple");
    strcat(mip_zenity_buffer," --separator=':'");
  }
  return mip_do_zenity();
}

const char* MIP_Dialog_FileSave(const char* ATitle, const char* AFilename) {
  strcpy(mip_zenity_buffer,"zenity --file-selection");
  strcat(mip_zenity_buffer," --save");
  strcat(mip_zenity_buffer," --title='");    strcat(mip_zenity_buffer,ATitle);    strcat(mip_zenity_buffer,"'");
  strcat(mip_zenity_buffer," --filename='"); strcat(mip_zenity_buffer,AFilename); strcat(mip_zenity_buffer,"'");
  return mip_do_zenity();
}

const char* MIP_Dialog_Info(const char* ATitle, const char* AText) {
  strcpy(mip_zenity_buffer,"zenity --info");
  strcat(mip_zenity_buffer," --text='");    strcat(mip_zenity_buffer,AText);    strcat(mip_zenity_buffer,"'");
  return mip_do_zenity();
}

const char* MIP_Dialog_Question(const char* ATitle, const char* AText) {
  strcpy(mip_zenity_buffer,"zenity --question");
  strcat(mip_zenity_buffer," --text='");    strcat(mip_zenity_buffer,AText);    strcat(mip_zenity_buffer,"'");
  return mip_do_zenity();
}

const char* MIP_Dialog_Text(const char* ATitle, const char* AText, const char* AEntryText) {
  strcpy(mip_zenity_buffer,"zenity --entry");
  strcat(mip_zenity_buffer," --text='");    strcat(mip_zenity_buffer,AText);            strcat(mip_zenity_buffer,"'");
  strcat(mip_zenity_buffer," --entry-text='");    strcat(mip_zenity_buffer,AEntryText); strcat(mip_zenity_buffer,"'");
  return mip_do_zenity();
}

// segmentation fault

const char* MIP_Dialog_TextInfo(const char* ATitle, const char* AFilename, bool AEditable=false, bool AHtml=false, const char* AUrl=nullptr) {
  strcpy(mip_zenity_buffer,"zenity --text-info");
  strcat(mip_zenity_buffer," --filename='");       strcat(mip_zenity_buffer,AFilename);   strcat(mip_zenity_buffer,"'");
  if (AEditable) strcat(mip_zenity_buffer," --editable");
  if (AHtml) strcat(mip_zenity_buffer," --html");
  if (AUrl) {
    strcat(mip_zenity_buffer," --url='");
    strcat(mip_zenity_buffer,AUrl);
    strcat(mip_zenity_buffer,"'");
  }
  return mip_do_zenity();
}

const char* MIP_Dialog_Value(const char* ATitle, const char* AText, int AVal=0, int AMin=0, int AMax=100, int AStep=1) {
  char temp[256];
  strcpy(mip_zenity_buffer,"zenity --scale");
  strcat(mip_zenity_buffer," --text='");       strcat(mip_zenity_buffer,AText); strcat(mip_zenity_buffer,"'");
  strcat(mip_zenity_buffer," --value='");      strcat(mip_zenity_buffer, MIP_IntToString(temp,AVal));  strcat(mip_zenity_buffer,"'");
  strcat(mip_zenity_buffer," --min-value='");  strcat(mip_zenity_buffer, MIP_IntToString(temp,AMin));  strcat(mip_zenity_buffer,"'");
  strcat(mip_zenity_buffer," --max-value='");  strcat(mip_zenity_buffer, MIP_IntToString(temp,AMax));  strcat(mip_zenity_buffer,"'");
  strcat(mip_zenity_buffer," --step='");       strcat(mip_zenity_buffer, MIP_IntToString(temp,AStep)); strcat(mip_zenity_buffer,"'");
  //strcat(mip_zenity_buffer," --print-partial");
  //strcat(mip_zenity_buffer," --hide-value");
  return mip_do_zenity();
}

const char* MIP_Dialog_Warning(const char* ATitle, const char* AText) {
  strcpy(mip_zenity_buffer,"zenity --warning");
  strcat(mip_zenity_buffer," --text='");    strcat(mip_zenity_buffer,AText);    strcat(mip_zenity_buffer,"'");
  return mip_do_zenity();
}

//----------------------------------------------------------------------
#endif
