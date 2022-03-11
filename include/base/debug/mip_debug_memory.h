#ifndef mip_debug_memory_included
#define mip_debug_memory_included
//----------------------------------------------------------------------

// http://wyw.dcweb.cn/leakage.htm
// http://stevehanov.ca/blog/?id=10

//----------------------------------------------------------------------

//  char      kode_delete_file[KODE_MAX_PATH_LENGTH] = {0};
//  char      kode_delete_func[KODE_MAX_STRING_LENGTH] = {0};
//  uint32_t  kode_delete_line = 0;
//
//  bool KODE_SetupDeletePrefix(const char* file, unsigned int line, const char* func) {
//    //kode_delete_file = (char*)file;
//    //kode_delete_line = line;
//    //kode_delete_func = (char*)func;
//    KODE_Strcpy(kode_delete_file,file);
//    KODE_Strcpy(kode_delete_func,func);
//    kode_delete_line = line;
//    return true;
//  }
//----------------------------------------------------------------------

#ifdef MIP_DEBUG

  void* operator new(size_t size, const char* file, int line) /*throw(std::bad_alloc)*/ {
    void *ptr = (void *)malloc(size);
    MIP_GLOBAL_DEBUG.addMemoryNode(ptr, size, file, line);
    return(ptr);
  };

  void* operator new [] (size_t size, const char* file, int line) /*throw(std::bad_alloc)*/ {
    void *ptr = (void *)malloc(size);
    MIP_GLOBAL_DEBUG.addMemoryNode(ptr, size, file, line);
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

  // ..popening an addr2line process..

//  void* operator new(size_t size) throw(std::bad_alloc) {
//    char temp[16] = {0};
//    void* addr = __builtin_return_address(0);
//    sprintf(temp,"<0x%08P>",addr);
//    //return operator new(size, __builtin_return_address(0), 0);
//    return operator new(size, temp, 0);
//  }

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

#endif

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

/*
  __FILE__
  __LINE__
  __FUNCTION__
*/

//#define malloc(A) _dbgmalloc(__FILE,__LINE, (A) )
//#define free(A) _dbgfree( __FILE__, __LINE__, (A) )
// calloc, realloc, strdup, etc.

#ifdef MIP_DEBUG
  #define DEBUG_NEW new(__FILE__, __LINE__)
#else
  #define DEBUG_NEW new
#endif

//

#define new DEBUG_NEW

//

//  #define KODE_New               new(__FILE__, __LINE__,__FUNCTION__)
//  #define KODE_Delete            if (KODE_SetupDeletePrefix(__FILE__, __LINE__, __FUNCTION__)) delete

//----------------------------------------------------------------------
#endif
