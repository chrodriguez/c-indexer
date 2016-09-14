#ifndef __INDEX_DATA_H
#define __INDEX_DATA_H
#include <stdint.h>
typedef enum t_format { number, date, string, undefined } t_index_format;

typedef struct {
  int64_t file_pos;
  uint16_t strlen;
  char * data;
} t_index_data;

/* Initializes a t_index_data variable with: NULL data and file_pos set to -1 */
void index_data_initialize(t_index_data *);

/* Compares two variables by data field interpreted as numbers */
int index_data_compare_by_number(t_index_data, t_index_data);

/* Compares tow variables by data field interpreted as strings */
int index_data_compare_by_string(t_index_data, t_index_data);

/* Compares tow variables by data field interpreted as dates */
int index_data_compare_by_date(t_index_data, t_index_data);

#endif
