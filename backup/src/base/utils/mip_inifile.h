#ifndef mip_inifile_included
#define mip_inifile_included
//----------------------------------------------------------------------

#include "base/mip_file.h"
#include "base/mip_parser.h"
#include "base/mip_paths.h"

//----------------------------------------------------------------------

#define MIP_INI_KEY_NAME_LENGTH      127
#define MIP_INI_KEY_VALUE_LENGTH     127
#define MIP_INI_SECTION_NAME_LENGTH  127

//----------

// -> MIP_IniKeyValuePair

struct MIP_IniKey {
  char name[MIP_INI_KEY_NAME_LENGTH+1] = {0};
  char value[MIP_INI_KEY_VALUE_LENGTH+1] = {0};
};

typedef MIP_Array<MIP_IniKey*> MIP_IniKeys;

//----------

struct MIP_IniSection {
  char          name[MIP_INI_SECTION_NAME_LENGTH+1] = {0};
  MIP_IniKeys  keys;
};

typedef MIP_Array<MIP_IniSection*> MIP_IniSections;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class MIP_IniFile {

//------------------------------
private:
//------------------------------

  MIP_IniSections  MSections;

//------------------------------
public:
//------------------------------

  MIP_IniFile() {
  }

  //----------

  ~MIP_IniFile() {
    #ifndef MIP_NO_AUTODELETE
      deleteSections();
    #endif
  }

//------------------------------
public:
//------------------------------

  void appendSection(const char* ASection) {
    MIP_IniSection* section = MIP_New MIP_IniSection();
    MIP_Strncpy(section->name,ASection,MIP_INI_SECTION_NAME_LENGTH);
    MSections.append(section);
  }

  //----------

  void deleteSections() {
    for (uint32_t i=0; i<MSections.size(); i++) {
      MIP_IniSection* section = MSections[i];
      for (uint32_t j=0; j<section->keys.size(); j++) {
        MIP_Delete section->keys[j];
      }
      delete MSections[i];
    }
  }

  //----------

  MIP_IniSection* findSection(const char* ASection) {
    for (uint32_t i=0; i<MSections.size(); i++) {
      MIP_IniSection* section = MSections[i];
      if (MIP_Strcmp(section->name, ASection) == 0) {
        return section;
      }
    }
    return  nullptr;
  }

  //----------

  void appendKey(const char* ASection, const char* AName, const char* AValue) {
    MIP_IniSection* section = findSection(ASection);
    MIP_IniKey* key = MIP_New MIP_IniKey();
    MIP_Strncpy(key->name,AName,MIP_INI_KEY_NAME_LENGTH);
    MIP_Strncpy(key->value,AValue,MIP_INI_KEY_VALUE_LENGTH);
    section->keys.append(key);
  }

  //----------

  MIP_IniKey* findKey(const char* ASection, const char* AKey) {
    MIP_IniSection* section = findSection(ASection);
    if (section) {
      for (uint32_t i=0; i<section->keys.size(); i++) {
        MIP_IniKey* key =section->keys[i];
        if (MIP_Strcmp(key->name, AKey) == 0) return key;
      }
    }
    return nullptr;
  }

  //----------

  uint32_t getNumSections() {
    return MSections.size();
  }

  MIP_IniSection* getSection(uint32_t AIndex) {
    return MSections[AIndex];
  }

  uint32_t getNumKeys(MIP_IniSection* ASection) {
    return ASection->keys.size();
  }

  MIP_IniKey* getKey(MIP_IniSection* ASection, uint32_t AIndex) {
    return ASection->keys[AIndex];
  }

//------------------------------
public:
//------------------------------

  const char* getString(const char* ASection, const char* AKey) {
    MIP_IniKey* key = findKey(ASection,AKey);
    if (key) {
      return key->value;
    }
    return nullptr;
  }

  //----------

  float getFloat(const char* ASection, const char* AKey) {
    MIP_IniKey* key = findKey(ASection,AKey);
    if (key) {
      float f = MIP_StringToFloat((char*)key->value);
      return f;
    }
    return 0.0f;
  }

  //----------

  int32_t getInt(const char* ASection, const char* AKey) {
    MIP_IniKey* key = findKey(ASection,AKey);
    if (key) {
      int32_t i = MIP_StringToInt((char*)key->value);
      return i;
    }
    return 0;
  }

//------------------------------
public:
//------------------------------

  bool load(void) {
    char filename[256];
    MIP_GetBaseFile(filename);
    MIP_StripFileExt(filename);
    MIP_Strcat(filename,".ini");
    return load(filename);
  }

  //----------

  bool load(const char* filename) {
    MIP_File file;
    if (file.exists(filename)) {
      file.open(filename,MIP_FILE_READ_BINARY);
      void* buffer = nullptr;
      uint32_t buffersize = file.length();
      if (buffersize>0) {
        buffer = MIP_Malloc(buffersize);
        file.read(buffer,buffersize);
      }
      file.close();
      parse(buffer,buffersize);
      MIP_Free(buffer);
      return true;
    }
    return false;
  }

  //----------

  bool save(void) {
    char filename[256];
    MIP_GetBaseFile(filename);
    MIP_StripFileExt(filename);
    MIP_Strcat(filename,".ini");
    return save(filename);
  }

  //----------

  bool save(const char* filename) {
    MIP_File file;
    if (file.exists(filename)) {
      MIP_DTrace("%s already exists. overwriting\n",filename);
    }
    file.open(filename,MIP_FILE_WRITE_TEXT);
    for (uint32_t i=0; i<MSections.size(); i++) {
      MIP_IniSection* section = MSections[i];
      // [
      file.write((void*)"[",1);
      // section name
      const char* txt = section->name;
      uint32_t len = MIP_Strlen(txt);
      file.write((void*)txt,len);
      // ]
      file.write((void*)"]\n",2);
      for (uint32_t j=0; j<section->keys.size(); j++) {
        MIP_IniKey* key =section->keys[j];
        // key.name
        const char* txt = key->name;
        uint32_t len = MIP_Strlen(txt);
        bool has_spaces = false;
        for (uint32_t s=0; s<len; s++) {
          if (txt[s] == ' ') {
            has_spaces = true;
            break;
          }
        }
        if (has_spaces) {
          // "key"
          file.write((void*)"\"",1);
          file.write((void*)txt,len);
          file.write((void*)"\"",1);
        }
        else {
          // key
          file.write((void*)txt,len);
        }
        // =
        file.write((void*)" = ",3);
        txt = key->value;
        len = MIP_Strlen(txt);
        has_spaces = false;
        for (uint32_t s=0; s<len; s++) {
          if (txt[s] == ' ') {
            has_spaces = true;
            break;
          }
        }
        if (has_spaces) {
          // "value"
          file.write((void*)"\"",1);
          file.write((void*)txt,len);
          file.write((void*)"\"",1);
        }
        else {
          // value
          file.write((void*)txt,len);
        }
        file.write((void*)"\n",1);
      }
    }
    file.close();
    return false;
  }

  //----------

  //void unload(void) {
  //  if (MBuffer) MIP_Free(MBuffer);
  //  MBuffer = nullptr;
  //}

  //----------

  // todo:
  // parser->getNameValuePair(..)

  void parse(void* ABuffer, uint32_t ABufferSize) {
    MIP_Parser* parser = MIP_New MIP_Parser();
    parser->tokenize((const char*)ABuffer,ABufferSize);

    const char* section = "";
    uint32_t t = 0;
    while (t < parser->getNumTokens()) {
      char* token = parser->getToken(t++);
      uint32_t tlen = MIP_Strlen(token);
      if ((token[0] == '[') && (token[tlen-1] == ']')) {
        token[tlen-1] = 0;
        section = token + 1;
        appendSection(section);
      }
      else {
        const char* equal = parser->getToken(t++);
        if ((equal[0] == '=') && (equal[1] == 0)) {
          const char* value = parser->getToken(t++);
          appendKey(section,token,value);
        }
      }
    }

  }

  //----------

};

//----------------------------------------------------------------------
#endif
