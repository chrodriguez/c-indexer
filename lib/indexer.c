#include <stdint.h> /* Imports uint16_t */
#include <endian.h> /* Imports htobe16  and htobe64 */
#include <string.h> /* Imports strlen and strtok */

#include <stdio.h>
#include <stdlib.h>

#include "indexer.h"
#include "index_data.h"
#include "error.h"

#define MAGIC_NUMBER "IDX_CSV"
#define FIELD_SEPARATOR ";\n"

#define MAX_LINE 2048

static void index_file_write_header(FILE * f, uint16_t field, uint16_t format) {
  rewind(f);
  field = htobe16(field);
  format = htobe16(format);
  fwrite(MAGIC_NUMBER, sizeof(char), strlen(MAGIC_NUMBER), f);
  fwrite(&field, sizeof(uint16_t), 1, f);
  fwrite(&format, sizeof(uint16_t), 1, f);
}

void index_file_write_data(FILE * f, t_index_data id) {
  id.file_pos = htobe64(id.file_pos);
  id.strlen = htobe16(id.strlen);
  fwrite(&id.file_pos, sizeof(uint64_t), 1, f);
  fwrite(&id.strlen, sizeof(uint16_t), 1, f);
  fwrite(id.data, sizeof(char), strlen(id.data), f);
}

static char ** str_to_fields(char * str) {
    char ** result = malloc(sizeof(char **));
    size_t size = 1;
    *result = NULL;
    char * field = strtok(str, FIELD_SEPARATOR);
    if (field) {
      do {
        result = realloc(result, sizeof(char **)*(size+1));
        result[size-1] = strdup(field);
        result[size++] = NULL;
      }while ( (field = strtok(NULL,FIELD_SEPARATOR) ));
    }
    return result;
}

static t_index_data index_data_from_string(char * str, long file_pos, int field) {
  t_index_data field_data;
  char ** fields, **first_field;
  size_t current = 1;

  index_data_initialize( &field_data);
  fields = first_field = str_to_fields(str);

  while( *fields ) {
    if ( current++ == field){
      field_data.data = strdup(*fields);
      field_data.strlen = strlen(*fields);
      field_data.file_pos = file_pos;
    }
    free(*fields);
    fields++;
  }
  free(first_field);
  if ( !field_data.data ) prg_error("Field data can't be NULL. Is field number correct?");
  if ( field_data.file_pos == -1 ) prg_error("File position can't be -1. Is field number correct?");
  return field_data;
}


void index_file(FILE * input, FILE * output, int field, t_index_format format) {
  char line[MAX_LINE];
  long file_pos = 0L;

  index_file_write_header(output, field, format);

  while (fgets(line, MAX_LINE-1, input)) {
    t_index_data index_data = index_data_from_string(line, file_pos, field);
    index_file_write_data(output, index_data);
    free(index_data.data);
    file_pos = ftell(input);
  }
}
