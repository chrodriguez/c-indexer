#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int prg_error(char *str, ...) {
  va_list va;
  va_start(va, str);
  fprintf(stderr, ">>ERROR<<\n");
  vfprintf(stderr, str, va);
  fprintf(stderr, "\n");
  va_end(va);
  exit(EXIT_FAILURE);
}

