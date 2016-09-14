#ifndef PERSON_H
#define PERSON_H
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define STR_LENGTH 128


typedef uint32_t    t_person_id;

typedef struct
{
  t_person_id    id;
  char        surname[STR_LENGTH];
  char        name[STR_LENGTH];
  struct tm   birth_date; 
} t_person;

/* Initialize a t_person record with received data */
t_person 
person_init(t_person_id id, const char * name, const char * surname, struct tm date);


void 
person_write ( t_person , FILE *);

void 
person_write_csv ( t_person , FILE *);

char * 
person_to_string(t_person p);

char * 
person_to_csv_string(t_person p);

int
person_read (t_person *, FILE * );

#endif
