#ifndef mip_file_included
#define mip_file_included
//----------------------------------------------------------------------

//TODO: c-ish (functions)

#include <stdio.h>

//----------------------------------------------------------------------

//#define MIP_FILE_RB    (char*)"rb"
//#define MIP_FILE_R     (char*)"r"
//#define MIP_FILE_WB    (char*)"wb"
//#define MIP_FILE_W     (char*)"w"
//#define MIP_FILE_AB    (char*)"ab"
//#define MIP_FILE_A     (char*)"a"

// file modes

#define MIP_FILE_READ_BINARY    (char*)"rb"
#define MIP_FILE_READ_TEXT      (char*)"r"
#define MIP_FILE_WRITE_BINARY   (char*)"wb"
#define MIP_FILE_WRITE_TEXT     (char*)"w"
#define MIP_FILE_APPEND_BINARY  (char*)"ab"
#define MIP_FILE_APPEND_TEXT    (char*)"w"

//----------------------------------------------------------------------

class MIP_File {

  protected:

    FILE*       MHandle;
    const char* MName;
    const char* MMode;

  public:

    /*
      does not open the file!
    */

    MIP_File() {
      MHandle = nullptr;
      MName   = "";
      MMode   = "";
    }

    MIP_File(const char* AFilename, const char* AMode=MIP_FILE_READ_BINARY) {
      open(AFilename,AMode);
    }

    //----------

    virtual ~MIP_File() {
      close();
    }

    //----------

    virtual
    bool exists(const char* AName) {
      if (MHandle) return true;
      FILE* fp = fopen(AName,MIP_FILE_READ_TEXT);
      if (fp) {
        close();
        return true;
      }
      else {
        return false;
      }
    }

    //----------

    virtual
    uint32_t length(void) {
      uint32_t len = 0;
      if (MHandle) {
        int32_t pos = ftell(MHandle);
        fseek(MHandle,0,SEEK_END);
        len = ftell(MHandle);
        fseek(MHandle,pos,SEEK_SET);
      }
      else {
        FILE* fp = fopen(MName,MIP_FILE_READ_TEXT);
        if (!fp) return 0;
        fseek(fp,0,SEEK_END);
        len = ftell(fp);
        //fseek(fp,0,SEEK_SET);
        fclose(fp);
      }
      return len;
    }

    //----------

    virtual
    bool open(const char* AFilename, const char* AMode=MIP_FILE_READ_BINARY) {
      MName   = AFilename;
      MMode   = AMode;
      MHandle = fopen(MName,MMode);
      if (MHandle) return true;
      return false;
    }

    //----------

    virtual
    void close(void) {
      if (MHandle) fclose(MHandle);
      MHandle = nullptr;
    }

    //----------

    virtual
    uint32_t read(void* ABuffer, uint32_t ALength, uint32_t ASize=1) {
      uint32_t num_read = fread(ABuffer,ASize,ALength,MHandle);
      return num_read;
    }

    //----------

    virtual
    uint32_t write(void* ABuffer, uint32_t ALength, uint32_t ASize=1) {
      uint32_t num_written = fwrite(ABuffer,ASize,ALength,MHandle);
      return num_written;
    }

};

//----------------------------------------------------------------------


/*

#include <stdio.h>
#include "mip_basepath.h"
#include "mip_const.h"
#include "mip_memory.h"
#include "mip_string.h"
#include "mip_utils.h"

//----------

#define KODE_FILE_RB    (char*)"rb"
#define KODE_FILE_R     (char*)"r"

#define KODE_FILE_WB    (char*)"wb"
#define KODE_FILE_W     (char*)"w"
#define KODE_FILE_AB    (char*)"ab"
#define KODE_FILE_A     (char*)"a"

#define KODE_FILE_RAUTO 0

class KODE_File {

  private:

    char*   MMode;
    FILE*   MFile;
    char    MFilePath[KODE_MAX_PATH_SIZE];
    void*   MBuffer;
    uint32  MLength;
    uint32  MSize;

  public:

    void*   getBuffer(void) { return MBuffer; }
    uint32  getLength(void) { return MLength; }

  public:

    KODE_File() {
      MFile = KODE_NULL;
      MBuffer = KODE_NULL;
    }

    //----------

    virtual ~KODE_File() {
      close();
      freebuf();
    }

    //----------

    void close(void) {
      if (MFile) fclose(MFile);
      MFile = KODE_NULL;
    }

    //----------

    int test(const char* AFilePath, char* AMode) {
      MFile = fopen(AFilePath, AMode);
      if (MFile) {
        close();
        return 1;
      }
      else {
        return 0;
      }
    }

    //----------

    void freebuf(void) {
      if (MBuffer) SFree(MBuffer);
      MBuffer = KODE_NULL;
    }

    //----------

    char* path(const char* AFileName) {
      MFilePath[0] = '\0';
      SGetBasePath(MFilePath);
      SStrcat(MFilePath, (char*)AFileName);
      return MFilePath;
    }

    //----------

    uint32 length(const char* AFileName, char* AMode = KODE_FILE_RB) {
      MMode = AMode;
      //path(AFileName);
      FILE* tmp_file = fopen(MFilePath, MMode);
      if (!tmp_file) return 0;
      fseek(tmp_file, 0, SEEK_END);
      MLength = ftell(tmp_file);
      fseek(tmp_file, 0, SEEK_SET);
      fclose(tmp_file);
      return MLength;
    }

    //----------

    int32 read(const char* AFileName, void* ABuffer, const uint32 ALength = KODE_FILE_RAUTO,
                       const uint32 ASize = 1, char* AMode = KODE_FILE_RB) {
      MFile   = KODE_NULL;
      MBuffer = ABuffer;
      MLength = ALength;
      MSize   = ASize;
      MMode   = AMode;
      //path(AFileName);
      SStrcpy(MFilePath,(char*)AFileName);
      MFile = fopen(MFilePath,MMode);
      if (!MFile) return -1;
      if (ALength == KODE_FILE_RAUTO) {
        MLength = length(AFileName,AMode);
        if (!MLength) return -2;
        MBuffer = SMalloc(MLength+1); // +1 for final 0
      }
      unsigned long result = fread(MBuffer,MSize,MLength,MFile);
      if (!result) return -3;
      close();
      char* ptr = (char*)MBuffer;
      ptr[MLength] = '\0';
      return result;
    }

    //----------

    int32 write(const char* AFileName, void* ABuffer, const uint32 ALength = 0,
                        const uint32 ASize = 1, char* AMode = KODE_FILE_WB) {
      if (ABuffer == KODE_NULL) {
        //DTrace("h_File.write, #ERR no buffer for: "<< MFilePath);
        return 0;
      }
      MFile   = KODE_NULL;
      MBuffer = ABuffer;
      MLength = ALength;
      MSize   = ASize;
      MMode   = AMode;
      //path(AFileName);
      MFile = fopen(MFilePath, MMode);
      if (!MFile) return -1;
      if (!MLength) return -2;
      if (!fwrite(MBuffer, MSize, MLength, MFile)) {
        return -3;
      }
      close();
      return 1;
    }

};

*/

//----------------------------------------------------------------------
#endif
