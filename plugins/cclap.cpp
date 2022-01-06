
/*
  idea:

  tcc (tiny c compiler)
  expose clap api (and mip utils, gui, dsp) to tcc
  and plugin hosting
  "file-watch", auto-recompile
  clap entry checks main c file
  clap factory compiles plugin code
  rest is glue/wrapper
  ala jesusonic, but in c

  next:
    * export code in a way that is simple to include in a 'real' plugin
      (compiled with gcc, optimized)
    * modular graph.. modules = codegen
      generates small snippets of code, depending on settings, connections,
      etc, compile on-the-fly,
      (later, export src to gcc)
*/
