#ifndef mip_debug_crash_handler_included
#define mip_debug_crash_handler_included
//----------------------------------------------------------------------

/*
  compile with: -g -rdynamic
*/

//----------------------------------------------------------------------

#ifdef MIP_DEBUG_CRASH_HANDLER


  #include <signal.h>

  //#include <stdlib.h>
  // struct sigaction act = {0};

  /*
    https://lasr.cs.ucla.edu/vahab/resources/signals.html
    The only two signals for which a handler cannot be defined are SIGKILL and
    SIGSTOP.
  */

  const char* MIP_SignalNames[] = {
    "(kill(pid,0))",
    "SIGHUP (Hangup)",
    "SIGINT (Interactive attention signal)",
    "SIGQUIT (Quit)",
    "SIGILL (Illegal instruction)",
    "SIGTRAP (Trace/breakpoint trap)",
    "SIGABRT (Abnormal termination)",
    "(-)",
    "SIGFPE (Erroneous arithmetic operation)",
    "SIGKILL (Killed)",
    "SIGBUS (Bus error)",
    "SIGSEGV (Invalid access to storage)",
    "SIGSYS (Bad system call)",
    "SIGPIPE (Broken pipe)",
    "SIGALRM (Alarm clock)",
    "SIGTERM (Termination request)",
    "SIGURG (Urgent data is available at a socket)",
    "SIGSTOP (Stop, unblockable)",
    "SIGTSTP (Keyboard stop)",
    "SIGCONT (Continue)",
    "SIGCHLD (Child terminated or stopped)",
    "SIGTTIN (Background read from control terminal)",
    "SIGTTOU (Background write to control terminal)",
    "SIGPOLL (Pollable event occurred)",
    "SIGXCPU (CPU time limit exceeded)",
    "SIGXFSZ (File size limit exceeded)",
    "SIGVTALRM (Virtual timer expired)",
    "SIGPROF (Profiling timer expired)",
    "SIGWINCH (Window size change)",
    "(-)",
    "SIGUSR1 (User-defined signal 1)",
    "SIGUSR2 (User-defined signal 2)"
  };

  //----------

  void mip_crash_handler_callback(int sig) {
    MIP_DPrint("\nmip_crash_handler_callback %i %s\n",sig,MIP_SignalNames[sig]);
    //MIP_DumpCallStackSkip(0); // 2
    MIP_DumpCallStack;
    exit(1); //_exit(1);
  }

  //----------

  void MIP_InitCrashHandler(int sig) {
    signal(SIGSEGV,mip_crash_handler_callback);
  }

  bool MIP_initCrashHandlers() {
    //for (int i=0; i<32; i++) MIP_InitSignalHandler(i);
    MIP_InitCrashHandler(SIGILL);
    MIP_InitCrashHandler(SIGABRT);
    MIP_InitCrashHandler(SIGFPE);
    MIP_InitCrashHandler(SIGSEGV);
    return true;
  }

  //----------

  // uninstall signal handler?
  // done automatically when program exits?

  //------------------------------

  bool mip_is_crash_handler_initialized = MIP_initCrashHandlers();

#endif // MIP_DEBUG_CRASH_HANDLER

//----------------------------------------------------------------------
#endif
