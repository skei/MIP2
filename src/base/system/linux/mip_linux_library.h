#ifndef mip_linux_library_included
#define mip_linux_library_included
//----------------------------------------------------------------------

#include <dlfcn.h>

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class MIP_Library {
  protected:
    bool  MLibLoaded;
    void* MLibHandle;

  public:

    MIP_Library() {
      MLibLoaded = false;
      MLibHandle = nullptr;
    }

    ~MIP_Library() {
      MLibLoaded = false;
      MLibHandle = nullptr;
    }

  //public:
  //  bool  loaded(void) { return MLibLoaded; }

  public:

    virtual
    bool loadLib(const char* AFilename) {
      MLibHandle = dlopen(AFilename,RTLD_LAZY | RTLD_LOCAL ); // RTLD_NOW, RTLD_LAZY
      if (!MLibHandle) return false;
      MLibLoaded = true;
      return true;
    }

    virtual
    void unloadLib(void) {
      if (MLibLoaded && MLibHandle) {
        dlclose(MLibHandle);
        MLibLoaded = false;
        MLibHandle = nullptr;
      }
    }

    virtual
    void* getLibSymbol(const char* ASymbol) {
      void* result = dlsym(MLibHandle,ASymbol);
      return result;
    }

};

//----------------------------------------------------------------------
#endif
