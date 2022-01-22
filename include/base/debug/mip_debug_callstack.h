#ifndef mip_debug_callstack_included
#define mip_debug_callstack_included
//----------------------------------------------------------------------

#ifdef MIP_DEBUG_CALLSTACK

  #include <stdio.h>
  #include <sys/unistd.h>
  #include <cxxabi.h>
  #include <execinfo.h>

  //#include "debug/mip_debug_trace.h"

  struct MIP_CallStackSymbol {
    const char*  func;    // name of function containing address of function.
    const char*  file;    // file where symbol is defined, might not work on all platforms.
    unsigned int line;    // line in file where symbol is defined, might not work on all platforms.
    unsigned int ofs;     // offset from start of function where call was made.
  };

  //----------

  struct MIP_CallStackStringBuffer {
    char*       out_ptr;
    const char* end_ptr;
  };

  //----------

  const char* mip_callstack_alloc_string( MIP_CallStackStringBuffer* buf, const char* str, size_t str_len ) {
    char* res;
    if ((size_t)(buf->end_ptr-buf->out_ptr)<(str_len+1)) return "out of memory";
    res = buf->out_ptr;
    buf->out_ptr += str_len + 1;
    memcpy(res,str,str_len);
    res[str_len] = '\0';
    return res;
  }

  //----------

  FILE* mip_run_addr2line( void** addresses, int num_addresses, char* tmp_buffer, size_t tmp_buf_len ) {
    const char addr2line_run_string[] = "addr2line -e /proc/%u/exe";
    size_t start = (size_t)snprintf(tmp_buffer,tmp_buf_len,addr2line_run_string,getpid());
    for (int i=0; i<num_addresses; ++i) {
      start += (size_t)snprintf(tmp_buffer+start,tmp_buf_len-start," %p",addresses[i]);
    }
    return popen(tmp_buffer,"r");
  }

  //----------

  char* mip_demangle_symbol( char* symbol, char* buffer, size_t buffer_size ) {
    int status;
    char* demangled_symbol = abi::__cxa_demangle( symbol, buffer, &buffer_size, &status );
    return status != 0 ? symbol : demangled_symbol;
  }

  //----------

  /*
    Generate a callstack from the current location in the code.
    skip_frames:    number of frames to skip in output to addresses.
    addresses:      is a pointer to a buffer where to store addresses in callstack.
    num_addresses:  size of addresses.
    return:         number of addresses in callstack.
  */

  int mip_callstack(int skip_frames, void** addresses, int num_addresses) {
    ++skip_frames;
    void* trace[256];
    int fetched = backtrace(trace,num_addresses+skip_frames)-skip_frames;
    memcpy(addresses,trace+skip_frames,(size_t)fetched*sizeof(void*));
    return fetched;
  }

  //----------

  /*
    Translate addresses from, for example, callstack to symbol-names.
    addresses:      list of pointers to translate.
    out_syms:       list of mip_callstack_symbol_t to fill with translated data, need to fit as many strings as there are ptrs in addresses.
    num_addresses:  number of addresses in addresses
    memory:         memory used to allocate strings stored in out_syms.
    mem_size:       size of addresses.
    return:         number of addresses translated.
  */

  int MIP_CallStackSymbols(void** addresses, MIP_CallStackSymbol* out_syms, int num_addresses, char* memory, int mem_size) {
    int num_translated = 0;
    MIP_CallStackStringBuffer outbuf = { memory, memory + mem_size };
    memset(out_syms,0x0,(size_t)num_addresses*sizeof(MIP_CallStackSymbol));
    char** syms = backtrace_symbols(addresses,num_addresses);
    size_t tmp_buf_len = 1024 * 32;
    char*  tmp_buffer  = (char*)malloc(tmp_buf_len);
    FILE* addr2line = mip_run_addr2line(addresses,num_addresses,tmp_buffer,tmp_buf_len);
    for (int i=0; i<num_addresses; ++i) {
      char* symbol = syms[i];
      unsigned int offset = 0;
      // find function name and offset
      char* name_start = strchr(symbol,'(');
      char* offset_start = name_start ? strchr(name_start,'+') : 0x0;
      if (name_start && offset_start) {
        // zero terminate all strings
        ++name_start;
        *offset_start = '\0';
        ++offset_start;
      }
      if (name_start && offset_start ) {
        offset = (unsigned int)strtoll(offset_start,0x0,16);
        symbol = mip_demangle_symbol(name_start,tmp_buffer,tmp_buf_len);
      }
      out_syms[i].func  = mip_callstack_alloc_string(&outbuf,symbol,strlen(symbol));
      out_syms[i].ofs   = offset;
      out_syms[i].file  = "failed to lookup file";
      out_syms[i].line  = 0;
      if( addr2line != 0x0 ) {
        if ( fgets(tmp_buffer,(int)tmp_buf_len, addr2line) != 0x0) {
          char* line_start = strchr(tmp_buffer,':');
          *line_start = '\0';
          if (tmp_buffer[0] != '?' && tmp_buffer[1] != '?') {
            out_syms[i].file = mip_callstack_alloc_string(&outbuf,tmp_buffer,strlen(tmp_buffer));
          }
          out_syms[i].line = (unsigned int)strtoll(line_start+1,0x0,10);
        }
      }
      ++num_translated;
    }
    free(syms);
    free(tmp_buffer);
    fclose(addr2line);
    return num_translated;
  }

  //----------

  /*
    two first entries (0,1) = crash handler itself (and glib?), #2 = positiof of crash..
    You can't call exit() safely from a signal handler. Use _exit() or _Exit()
  */

  void mip_debug_callstack_print(void** AAddresses=nullptr, int ANumAddresses=0, int skip_last=0, int skip_first=0) {
    MIP_CallStackSymbol  symbols[256];
    char              symbols_buffer[1024];
    int               num_addresses;
    if (AAddresses && (ANumAddresses>0)) {
      num_addresses = MIP_CallStackSymbols(AAddresses,symbols,ANumAddresses,symbols_buffer,1024);
    }
    else {
      void* adr[256];
      int num = mip_callstack(0,adr,256);
      num_addresses = MIP_CallStackSymbols(adr,symbols,num,symbols_buffer,1024);
    }
    MIP_DPrint("\n----------------------------------------------------------------------\n");
    MIP_DPrint("callstack\n");
    MIP_DPrint("----------------------------------------------------------------------\n");

    /*
      the last two entries are
      - __libc_start_main
      - mip_debug()
    */
    num_addresses -= skip_last;

    for (int i=skip_first; i<num_addresses; i++) {
      MIP_DPrint("%3i : %s\n", i-skip_first, symbols[i].func);
      MIP_DPrint("      %s\n", symbols[i].file);
      MIP_DPrint("      line %i offset %i\n", symbols[i].line, symbols[i].ofs);
    }
    MIP_DPrint("----------------------------------------------------------------------\n\n");
  }

  //----------

  /*
  #define KDumpCallStackAll                     \
    {                                           \
      void* adr[256];                           \
      int num = callstack(0,adr,256);           \
      mip_debug_callstack_print(adr,num);      \
    }
  */

  #define MIP_DumpCallStack                    \
    {                                           \
      void* adr[256];                           \
      int num = mip_callstack(0,adr,256);           \
      mip_debug_callstack_print(adr,num,2);   \
    }

  #define MIP_DumpCallStackSkip(s)             \
    {                                           \
      void* adr[256];                           \
      int num = mip_callstack(0,adr,256);     \
      mip_debug_callstack_print(adr,num,2,s); \
    }

#else // MIP_DEBUG_CALLSTACK

  struct MIP_CallStackSymbol {};
  struct MIP_CallStackStringBuffer {};
  /*_MIP_ALWAYS_INLINE*/ int mip_callstack(int skip_frames, void** addresses, int num_addresses) { return 0; }
  /*_MIP_ALWAYS_INLINE*/ int MIP_CallStackSymbols(void** addresses, MIP_CallStackSymbol* out_syms, int num_addresses, char* memory, int mem_size) { return 0; }
  /*_MIP_ALWAYS_INLINE*/ void MIP_PrintCallStack(void** AAddresses=nullptr, int ANumAddresses=0) {}

  #define MIP_DumpCallStack {}
  #define MIP_DumpCallStackSkip(s) {}

#endif // MIP_DEBUG_CALLSTACK

//----------------------------------------------------------------------
#endif



