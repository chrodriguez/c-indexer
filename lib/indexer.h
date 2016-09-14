#ifndef __INDEXER_H
#define __INDEXER_H
#include "index_data.h"

/* Index a csv file (input) into a special index file (output) considering field and format:
 * field is the number of the field to index. First field is 1
 * format allows to sort file easily knowing its type
 * */
void index_file(FILE * input, FILE * output, int field, t_index_format format);
#endif
