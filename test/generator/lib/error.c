#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

void error(int exit_status, int errno, const char * str, ...) {
  va_list va;
  va_start(va, str);
  fprintf(stderr, ">>ERROR  %s<<\n", strerror(errno));
  vfprintf(stderr, str, va);
  fprintf(stderr, "\n");
  va_end(va);
  exit(exit_status);
}
