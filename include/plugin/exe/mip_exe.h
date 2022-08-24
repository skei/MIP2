#ifndef mip_exe_included
#define mip_exe_included
//----------------------------------------------------------------------

#include "mip.h"

#define MIP_EXECUTABLE_SHARED_LIBRARY
//-Wl,-e,entry_point


#ifdef MIP_PLUGIN
#ifdef MIP_EXECUTABLE_SHARED_LIBRARY

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int main(int argc, char** argv, char** env);

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

extern "C" {

  int my_test_global = 123;

  int test_func() {
    printf("> test_func()\n");
    return 42;
  }

  // not called?
  int test_func_result = test_func();

  // not called?
  __attribute__((constructor))
  void ctor_test_func() {
    MIP_Print("> ctor_test_func()\n");
    test_func_result = 41;
  }

  int main_result;

  int* main_trampoline(int argc, char** argv, char** env) {
    MIP_Print("> main_trampoline()\n");
    MIP_Print("(my_test_global = %i)\n",my_test_global);
    MIP_Print("(test_func_result = %i)\n",test_func_result);
    MIP_Print("calling real main()..\n");
    main_result = main(argc,argv,env);
    MIP_Print("..and we're back..\n");
    return &main_result;
  }

}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  https://stackoverflow.com/questions/15265295/understanding-the-libc-init-array

  These symbols are related to the C / C++ constructor and destructor startup
  and tear down code that is called before / after main(). Sections named
  .init, .ctors, .preinit_array, and .init_array are to do with initialization
  of C/C++ objects, and sections .fini, .fini_array, and .dtors are for tear
  down. The start and end symbols define the beginning and end of code sections
  related to such operations and might be referenced from other parts of the
  runtime support code.

  The .preinit_array and .init_array sections contain arrays of pointers to
  functions that will be called on initialization. The .fini_array is an array
  of functions that will be called on destruction. Presumably the start and end
  labels are used to walk these lists.

  A good example of code that uses these symbols is to be found here
  http://static.grumpycoder.net/pixel/uC-sdk-doc/initfini_8c_source.html
  for initfini.c. You can see that on startup, __libc_init_array() is called
  and this first calls all the function pointers in section .preinit_array by
  referring to the start and end labels. Then it calls the _init() function in
  the .init section. Lastly it calls all the function pointers in section
  .init_array. After main() is complete the teardown call to
  __libc_fini_array() causes all the functions in .fini_array to be called,
  before finally calling _fini().

  Note that there seems to be a cut-and-paste bug in this code when it
  calculates the count of functions to call at teardown. Presumably they were
  dealing with a real time micro controller OS and never encountered this
  section.
*/

//----------

/*
  see also:
  https://refspecs.linuxbase.org/LSB_3.1.0/LSB-generic/LSB-generic/baselib---libc-start-main-.html
  --enable-initfini-array ??
*/

/*
  #ifdef SIXTY_FOUR_BIT
    const char my_interp[] __attribute__((section(".interp"))) = "/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2";
  #else
    const char my_interp[] __attribute__((section(".interp"))) = "/lib/ld-linux.so.2";
  #endif
*/

const char interp_section[] __attribute__((section(".interp"))) = "/lib64/ld-linux-x86-64.so.2";

#include <sys/types.h>

extern "C" {

  //------------------------------
  // __libc_init_array
  //------------------------------

  /*
    According to this comment from Nominal Animal here:
    stackoverflow.com/questions/32700494/executing-init-and-fini,
    using the attribute constructor would add the function in the .init_array,
    and it depends of the ABI; if you're on x86, it should work.
  */

  // https://stackoverflow.com/questions/13734745/why-do-i-have-an-undefined-reference-to-init-in-libc-init-array

  /*
  extern void _init (void);
  extern void (*__preinit_array_start[]) (void) __attribute__((weak));
  extern void (*__preinit_array_end[])   (void) __attribute__((weak));
  extern void (*__init_array_start[])    (void) __attribute__((weak));
  extern void (*__init_array_end[])      (void) __attribute__((weak));

  void __libc_init_array(void) {
    MIP_Print("* __libc_init_array()\n");
    MIP_Print("  (__preinit_array_start: %p)\n",__preinit_array_start);
    MIP_Print("  (__preinit_array_end: %p)\n",__preinit_array_end);
    MIP_Print("  (__init_array_start: %p)\n",__init_array_start);
    MIP_Print("  (__init_array_end: %p)\n",__init_array_end);
    size_t count;
    size_t i;
    count = __preinit_array_end - __preinit_array_start;
    MIP_Print("   count: %i\n",(int)count);
    for (i = 0; i < count; i++) __preinit_array_start[i]();
    _init();
    count = __init_array_end - __init_array_start;
    for (i = 0; i < count; i++) __init_array_start[i]();
  }
  */

  //------------------------------
  // __libc_csu_init
  //------------------------------

  /*
  extern void _init (void);
  extern void (*__preinit_array_start[]) (int argc, char **argv, char **envp) __attribute__((weak));
  extern void (*__preinit_array_end[])   (int argc, char **argv, char **envp) __attribute__((weak));
  extern void (*__init_array_start[])    (int argc, char **argv, char **envp) __attribute__((weak));
  extern void (*__init_array_end[])      (int argc, char **argv, char **envp) __attribute__((weak));

  void __libc_csu_init(int argc, char **argv, char **envp) {
    MIP_Print("* __libc_init_array()\n");
    MIP_Print("  (__preinit_array_start: %p)\n",__preinit_array_start);
    MIP_Print("  (__preinit_array_end: %p)\n",__preinit_array_end);
    MIP_Print("  (__init_array_start: %p)\n",__init_array_start);
    MIP_Print("  (__init_array_end: %p)\n",__init_array_end);
    size_t count;
    size_t i;
    count = __preinit_array_end - __preinit_array_start;
    MIP_Print("   preinit count: %i\n",(int)count);
    for (i=0; i<count; i++) (*__preinit_array_start[i])(argc, argv, envp);
    MIP_Print("  calling _init\n");
    _init ();
    count = __init_array_end - __init_array_start;
    MIP_Print("   init count: %i\n",(int)count);
    for (i=0; i<count; i++) (*__init_array_start [i]) (argc, argv, envp);
  }
  */

  //------------------------------
  // __libc_start_main
  //------------------------------

  extern int __libc_start_main(
    int  *(main)(int, char**, char**),
    int    argc,
    char** ubp_av,
    void (*init)(void),
    void (*fini)(void),
    void (*rtld_fini)(void),
    void (*stack_end)
  );

  //------------------------------
  // entry_point
  //------------------------------

  __attribute__((force_align_arg_pointer))
  void entry_point() {
    MIP_Print("* entry_point()\n");
    int argc = 0;
    const char* argv[] = { "", 0 };
    MIP_Print("> argc %i argv %s\n",argc,argv[0]);
    MIP_Print("calling __libc_start_main..\n");
    //__libc_start_main(main_trampoline,argc,(char**)argv,my_init,my_fini,my_rtld_fini,0);//&my_stack[500000]);
    MIP_REGISTRY.initialize();
    __libc_start_main(main_trampoline,argc,(char**)argv,nullptr,nullptr,nullptr,0);//&my_stack[500000]);
    MIP_Print("..back from __libc_start_main\n");
    MIP_Print("calling _exit..\n");
    _exit(EXIT_SUCCESS);
  }


}




#endif // MIP_EXECUTABLE_SHARED_LIBRARY
#endif // MIP_PLUGIN

//----------------------------------------------------------------------
#endif
