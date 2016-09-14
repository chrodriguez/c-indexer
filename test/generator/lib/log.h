#ifndef _LOG_H
#define _LOG_H

#define LOG_DEBUG_PREFIX ">>Debug: "
#define LOG_INFO_PREFIX ""
 
int log_debug(const char * format, ...);
int log_info(const char * format, ...);

#endif
