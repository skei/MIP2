#!/usr/bin/tcc -run

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

//----------------------------------------------------------------------

void *handle;
char *error;
void (*lv2_dumpttl)(void);

//----------------------------------------------------------------------

int main(int argc, char **argv) {

  if (argc != 2) {
    printf("usage: dump_ttl <plugin.so>\n");
    exit(EXIT_FAILURE);
  }

  //printf("dumping %s\n",argv[1]);

  handle = dlopen(argv[1],RTLD_LAZY);
  if (!handle) {
    printf("%s\n",dlerror());
    exit(EXIT_FAILURE);
  }

  dlerror();  // Clear any existing error
  *(void **)(&lv2_dumpttl) = dlsym(handle,"lv2_export_ttl");
  if ((error = dlerror()) != NULL) {
    printf("%s\n", error);
    exit(EXIT_FAILURE);
  }

  (*lv2_dumpttl)();

  dlclose(handle);
  exit(EXIT_SUCCESS);

  return 0;
}

//----------------------------------------------------------------------

/*
  Writing: cosine = (double (*)(double)) dlsym(handle, "cos");
  would seem more natural, but the C99 standard leaves
  casting from "void *" to a function pointer undefined.
  The assignment used below is the POSIX.1-2003 (Technical
  Corrigendum 1) workaround; see the Rationale for the
  POSIX specification of dlsym().
*/

