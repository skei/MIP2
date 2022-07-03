#ifndef mip_debug_memory_included
#define mip_debug_memory_included
//----------------------------------------------------------------------

// http://wyw.dcweb.cn/leakage.htm
// http://stevehanov.ca/blog/?id=10

#ifdef MIP_DEBUG_MEMORY

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

  /*
    this is not very safe.. let's hope there's not another delete going on in
    another thread or something.. :-/
  */

  char      mip_debug_memory_delete_file[512] = {0};
  char      mip_debug_memory_delete_func[256] = {0};
  uint32_t  mip_debug_memory_delete_line = 0;

  //----------

  bool MIP_setupDeletePrefix(const char* file, const char* func, unsigned int line) {
    strcpy(mip_debug_memory_delete_file,file);
    strcpy(mip_debug_memory_delete_func,func);
    mip_debug_memory_delete_line = line;
    return true;
  }

  //----------------------------------------------------------------------
  //
  //
  //
  //----------------------------------------------------------------------

  void* operator new(size_t size, const char* file, const char* func, int line) /*throw(std::bad_alloc)*/ {
    void *ptr = (void *)malloc(size);
    MIP_GLOBAL_DEBUG.addMemoryNode(ptr,size,file,func,line);
    return(ptr);
  };

  void* operator new [] (size_t size, const char* file, const char* func, int line) /*throw(std::bad_alloc)*/ {
    void *ptr = (void *)malloc(size);
    MIP_GLOBAL_DEBUG.addMemoryNode(ptr,size,file,func,line);
    return(ptr);
  };

  //----------

//    KODE_GLOBAL_DEBUG_MEMORY._delete(ptr,kode_delete_file,kode_delete_line,kode_delete_func);
//    kode_delete_file[0] = 0;
//    kode_delete_func[0] = 0;
//    kode_delete_line = 0;

  void operator delete(void *p) /*_GLIBCXX_USE_NOEXCEPT*/ {
    MIP_GLOBAL_DEBUG.removeMemoryNode(p);
    free(p);
  };

  void operator delete [] (void *p) /*_GLIBCXX_USE_NOEXCEPT*/ {
    MIP_GLOBAL_DEBUG.removeMemoryNode(p);
    free(p);
  };

  //----------

  // ..popening an addr2line process..

  //void* operator new(size_t size) throw(std::bad_alloc) {
  //  char temp[16] = {0};
  //  void* addr = __builtin_return_address(0);
  //  sprintf(temp,"<0x%08P>",addr);
  //  //return operator new(size, __builtin_return_address(0), 0);
  //  return operator new(size, temp, 0);
  //}

  /*

  void operator delete(void*, std::size_t) _GLIBCXX_USE_NOEXCEPT {
    printf("delete\n");
  }

  void operator delete[](void*, std::size_t) _GLIBCXX_USE_NOEXCEPT {
    printf("delete[]\n");
  }

  void operator delete(void*, const std::nothrow_t&) _GLIBCXX_USE_NOEXCEPT {
    printf("delete 1\n");
  }

  void operator delete[](void*, const std::nothrow_t&) _GLIBCXX_USE_NOEXCEPT {
    printf("delete 1\n");
  }

  inline void operator delete  (void*, void*) _GLIBCXX_USE_NOEXCEPT {
    printf("delete 1\n");
  }

  inline void operator delete[](void*, void*) _GLIBCXX_USE_NOEXCEPT {
    printf("delete 1\n");
  }

  */

  //----------

  //#define MIP_DEBUG_NEW    new(__FILE__, __PRETTY_FUNCTION__,__LINE__)
  #define MIP_DEBUG_NEW    new(__FILE__, __FUNCTION__,__LINE__)
  #define MIP_DEBUG_DELETE if (MIP_setupDeletePrefix(__FILE__,__PRETTY_FUNCTION__,__LINE__)) delete

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

  #define new     MIP_DEBUG_NEW
  #define delete  MIP_DEBUG_DELETE

#else // MIP_DEBUG_MEMORY

#endif

//----------


//

//  #define KODE_New               new(__FILE__, __LINE__,__FUNCTION__)
//  #define KODE_Delete            if (KODE_SetupDeletePrefix(__FILE__, __LINE__, __FUNCTION__)) delete




//----------------------------------------------------------------------
#endif
