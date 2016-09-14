#define _XOPEN_SOURCE
#define _GNU_SOURCE

#include <time.h>   /* Imports struct tm */
#include <string.h> /* Imports strcmp */

#include <stdio.h>
#include "error.h"
#include "index_data.h"


void index_data_initialize(t_index_data * id) {
  id->data = NULL;
  id->file_pos = -1;
}

int index_data_compare_by_number(t_index_data a, t_index_data b) {
  long long a_data, b_data;
  if ( sscanf(a.data, "%lld", &a_data) != 1 ) prg_error("Error converting %s to number", a.data);
  if ( sscanf(b.data, "%lld", &b_data) != 1 ) prg_error("Error converting %s to number", b.data);
  return a_data < b_data ? -1 : (a_data > b_data ? 1 : 0 );
}

int index_data_compare_by_string(t_index_data a, t_index_data b) {
  return strcmp(a.data, b.data);
}

int index_data_compare_by_date(t_index_data a, t_index_data b) {
  struct tm a_date, b_date;
  time_t a_time, b_time;
  double diff;

  memset (&a_date, '\0', sizeof (struct tm));
  memset (&b_date, '\0', sizeof (struct tm));

  if ( !strptime(a.data, "%d/%m/%Y", &a_date ) ) prg_error("Error converting %s to date (DD/MM/YYYY)", a.data);
  if ( !strptime(b.data, "%d/%m/%Y", &b_date ) ) prg_error("Error converting %s to date (DD/MM/YYYY)", b.data);
  a_time = mktime(&a_date);
  b_time = mktime(&b_date);
  diff = difftime(a_time, b_time);
  return diff < 0 ? -1 : (diff > 0 ? 1 : 0);
}

