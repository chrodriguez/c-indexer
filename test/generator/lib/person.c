#include "person.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <error.h>
#include <errno.h>

#define DATE_FORMAT "%d/%m/%Y"

extern int errno;

static char _PERSON_STR_BUFFER[4096];

const uint16_t PERSON_FILE_MAGIC = 0xFE9C;

char * person_to_string(t_person p)
{ char date[256];
  strftime(date, 256, DATE_FORMAT, &p.birth_date);
  sprintf(_PERSON_STR_BUFFER,"%s - %lld - %s, %s",
            date,
            (long long int) p.id,
            p.surname,
            p.name);
  return _PERSON_STR_BUFFER;
}

char * person_to_csv_string(t_person p)
{ char date[256];
  strftime(date, 256, DATE_FORMAT, &p.birth_date);
  sprintf(_PERSON_STR_BUFFER,"%lld;%s;%s;%s",
            (long long int) p.id,
            date,
            p.surname,
            p.name);
  return _PERSON_STR_BUFFER;
}

t_person 
person_init(t_person_id id, const char * name, const char * surname, struct tm date)
{
  t_person p;
  p.id=id;
  strncpy(p.name, name,STR_LENGTH-1);
  strncpy(p.surname, surname,STR_LENGTH-1);
  //person_set_birth_date(&p,birth_day, birth_month, birth_year);
  p.birth_date = date;
  log_debug("Inicializando Persona <%s>",person_to_string(p));
  return p;
}

void 
person_write(t_person p, FILE * f)
{
  if ( ftell(f) == 0L )
  {
    if ( fwrite(&PERSON_FILE_MAGIC,sizeof(PERSON_FILE_MAGIC),1,f) != 1 )
    {
      error(EXIT_FAILURE,errno,"Error writing person magic number");
    }
    log_debug("Primer persona a guardar, se almacena el magic number: 0x%X",PERSON_FILE_MAGIC);
  }
  if ( fwrite(&p,sizeof(p),1,f) != 1 )
  {
    error(EXIT_FAILURE,errno,"Error writing person to file");
  }
  log_debug("Persona guardada <%s>",person_to_string(p));
}

void 
person_write_csv(t_person p, FILE * f)
{
  if ( fprintf(f,"%s\n",person_to_csv_string(p)) < 0 )
  {
    error(EXIT_FAILURE,errno,"Error writing person to file");
  }
  log_debug("Persona guardada <%s>",person_to_csv_string(p));
}

int
person_read (t_person *p, FILE * f)
{
  uint16_t magic;
  if ( ftell(f) == 0L )
  {
    if ( fread(& magic, sizeof(magic),1,f) != 1 )
    {
      error(EXIT_FAILURE,errno,"Error reading person magic number");
    }
    log_debug("Se verifica el magic number sea: 0x%X",PERSON_FILE_MAGIC);
    if ( magic != PERSON_FILE_MAGIC )
    {
      error(EXIT_FAILURE,errno,"Magic number mismatch");
    }
    log_debug("Magic number verificado");
  
  }
  if ( fread(p,sizeof(*p),1,f) != 1 )
  {
    log_debug("La persona no pudo leerse: %s",feof(f)?"EOF":"ERROR");
    return 0;
  }
  log_debug("Persona leida <%s>",person_to_string(*p));
  return 1;
}
