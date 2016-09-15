#include <getopt.h>
#include <stdio.h>
#include <string.h>

#include "lib/error.h"

void print_sorted_by_index(FILE *input, FILE * index, int order) {
  prg_error("To be continued...");
}


int
main (int argc, char **argv) {
  int opt;
  FILE * input = NULL, * index = NULL;
  enum sort_order { asc, desc, undefined } order = asc;

  while ((opt = getopt(argc, argv, "d:i:o:h")) != -1) {
    switch (opt) {
      case 'i':
        if ( ( index = fopen(optarg, "r")) == NULL ) {
          prg_error("Error opening file %s", optarg);
        }
        break;
      case 'd':
        if ( ( input = fopen(optarg, "r")) == NULL ) {
          prg_error("Error opening file %s", optarg);
        }
        break;
      case 'o':
        order = strcasecmp(optarg, "asc") == 0 ? asc :
          ( strcasecmp(optarg, "desc") == 0 ? desc : undefined );
        if ( order == undefined ) prg_error("Undefined sort order %s. Supported orders are asc, desc", optarg);
        break;
      case 'h':
      default: /* '?' */
        prg_error("Usage: %s -d data.csv -i index.idx [-o ASC|DESC]", argv[0]);
    }
  }
  if ( order == undefined ) prg_error("Must define a field_type with -t");
  if ( index == NULL)  prg_error("Must specify an index file with -i");
  if ( input == NULL)  prg_error("Must specify a file to read from with -d");
  print_sorted_by_index(input, index, order);
  fclose(input);
  fclose(index);
  return 0;
}

