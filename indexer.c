#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/error.h"
#include "lib/indexer.h"

int
main (int argc, char **argv) {
  int opt, field = -1;
  FILE * input = NULL, * output = NULL;
  t_index_format format = undefined;

  while ((opt = getopt(argc, argv, "i:f:t:o:h")) != -1) {
    switch (opt) {
      case 'i':
        if ( ( input = fopen(optarg, "r")) == NULL ) {
          prg_error("Error opening file %s", optarg);
        }
        break;
      case 'f':
        if ( sscanf(optarg, "%d", &field) != 1 ) prg_error("Field must be a number, %s given", optarg);
        break;
      case 't':
        format = strcasecmp(optarg, "number") == 0 ? number :
          ( strcasecmp(optarg, "date") == 0 ? date : strcasecmp(optarg, "string") == 0 ? string : undefined );
        if ( format == undefined ) prg_error("Undefined index type %s. Supported types are number, string, date", optarg);
        break;
      case 'o':
        if ( ( output = fopen(optarg, "w")) == NULL ) {
          prg_error("Error opening file %s", optarg);
        }
        break;
      case 'h':
      default: /* '?' */
        prg_error("Usage: %s -i input.csv -f field_number -t field_type -o index-output.idx", argv[0]);
    }
  }
  if ( format == undefined ) prg_error("Must define a field_type with -t");
  if ( output == NULL)  prg_error("Must specify a file to write to with -o");
  if ( input == NULL)  prg_error("Must specify a file to read from with -i");
  index_file(input, output, field, format);
  fclose(input);
  fclose(output);
  return 0;
}

