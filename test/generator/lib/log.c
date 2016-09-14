#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <strings.h>
#include <string.h>
#include "log.h"

#define LOG_UNWANTED_CHARS "\n\r\t"
 
static int 
_log(FILE * f, const char * prefix, const char * format, va_list  ap)
{ char * nftok, * nf, * token;
    int ret;
    nftok = strdup(format);
    nf= strdup(format);
    bzero(nf,strlen(format));

    fprintf(f,"\n%s",prefix);
    /* 
     * Eliminamos los caracteres contenidos en LOG_UNWANTED_CHARS 
     * reemplazandolos por espacio
     */
    token = strtok(nftok,LOG_UNWANTED_CHARS); 
    while ( token != NULL )
    {
      nf = (char *) realloc(nf, ( strlen(nf) + 1 + strlen(token) + 1) * sizeof(char));
      sprintf(nf+strlen(nf),"%s ",token);
      token = strtok(NULL,LOG_UNWANTED_CHARS); 
    }
    ret=vfprintf(f,nf, ap);
    free(nftok);
    free(nf);
    fprintf(f,"\n");
    return ret;
}

int 
log_debug(const char * format, ...)
{ int ret=0;
  #ifdef LOG_DEBUG
  va_list ap;
    va_start(ap, format);
    ret = _log(stderr, LOG_DEBUG_PREFIX,format,ap);
    va_end(ap);
  #endif
  return ret;
}

int 
log_info(const char * format, ...)
{
  int ret;
  va_list ap;
    va_start(ap, format);
    ret = _log(stderr, LOG_INFO_PREFIX,format,ap);
    va_end(ap);
    return ret;
}

