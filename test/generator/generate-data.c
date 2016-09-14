#include "lib/person.h"
#include "lib/log.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <time.h>

#define FILENAME_SIZE 1024
#define MODE_WRITE "w"
#define MODE_APPEND "a"
#define BUFFER_SIZE 256
#define NEW_LINE_CHARS "\n\r"
#define MAX_ITERATIONS_RANDOM_READ 10
#define PERSON_ID_MIN 22000000L
#define PERSON_ID_MAX 35000000L

extern char * optarg;
extern int errno;

enum words_kind { FIRSTNAMES=0, SURNAMES };
char ** words[2];
char BUFFER[BUFFER_SIZE];

/*
 * Crea una lista de personas aleatoria a partir de los archivos name y surname
 * que contienen nombres y apellidos respectivamente.
 * La cantidad de personas generadas es size y se guardan en out
 */
void
generate_data(FILE *name, FILE * surname, FILE *out, unsigned size);

/*
 * Implementacion mas abajo
 */
struct tm
generate_date();

t_person_id
generate_id(t_person_id *ids, unsigned pos);

void
error_message(const char * err, const char * name)
{
           error(EXIT_FAILURE,0,
            "%s %s\n"
            "This program generates random data file of person records.\n"
            "The generated file will have the following formar:\n"
            "FORMAT\n\n"
            "Usage: \n\t%s [OPTIONS] -z NUM -n NAME_SOURCE_FILE -s SURNAME_SOURCE_FILE -o OUTPUT.cdt"
            "\n\t\t-z NUM:\t\t\tGenerate NUM records"
            "\n\t\t-n NAME_SOURCE_FILE:\tFile with one name by line"
            "\n\t\t-s SURNAME_SOURCE_FILE:\tFile with one surname by line"
            "\n\t\t-o OUTPUT.cdt:\tFile to generate"
            "\n\tOPTIONS:"
            "\n\t\t-a\t\t\tAppend mode"
            "\n\t\t-w\t\t\tOverwrite mode (will ask if output exists)",
            err==NULL?"":"\nERROR:",
            err==NULL?"":err,
            name);
}



int
main(int argc, char *argv[])
{
  int opt;
  FILE
    * name,
    * surname,
    * out;
  long int size=0;
  char *  mode;
  char
    source_name[FILENAME_SIZE],
    source_surname[FILENAME_SIZE],
    output[FILENAME_SIZE];

    /* Initialize variables */
    mode = "w";
    bzero(source_name,FILENAME_SIZE); 
    bzero(source_surname,FILENAME_SIZE); 
    bzero(output,FILENAME_SIZE); 

    while ((opt = getopt(argc, argv, "hz:n:s:o:aw")) != -1) 
    {
      switch (opt) 
      {
        // Write mode
        case 'h':
              error_message(NULL,argv[0]);
              break;
        case 'a':
              mode= MODE_APPEND;
              break;
        case 'w':
            mode = MODE_WRITE;
            break;
        // Number of records to generate
        case 'z':
            size = strtol(optarg, (char **) NULL, 10);
            if ( size == EINVAL )
            {
              error_message("Invalid size",argv[0]);
            }
            break;
        case 'n':
            if ( strlen(optarg)> FILENAME_SIZE -1 )
            {
              error_message("Specified source name filename too long",argv[0]);
            }
            strcpy(source_name,optarg);
            break;
        case 's':
            if ( strlen(optarg)> FILENAME_SIZE -1 )
            {
              error_message("Specified source surname filename too long",argv[0]);
            }
            strcpy(source_surname,optarg);
            break;
        case 'o':
            if ( strlen(optarg)> FILENAME_SIZE -1 )
            {
              error_message("Specified output filename too long",argv[0]);
            }
            strcpy(output,optarg);
            break;
        default:
            error_message("Unknown option",argv[0]);
      }
    }

    /*
      Check parameters
    */
    if ( strlen(source_name) == 0 )
    {
      error_message("Source name filename not specified (-n)",argv[0]);
    }
    if ( strlen(source_surname) == 0 )
    {
      error_message("Source surname filename not specified (-s)",argv[0]);
    }
    if ( strlen(output) == 0 )
    {
      error_message("Output filename not specified (-o)",argv[0]);
    }
    if ( size == 0 )
    {
      error_message("Number of records not specifieda or zero (-z)", argv[0]);
    }

    /*
      Open files
    */
    if ( (name=fopen(source_name,"r")) == NULL )
    {
      error_message("Error opening source name filename", argv[0]);
    }
    if ( (surname=fopen(source_surname,"r")) == NULL )
    {
      error_message("Error opening source surname filename", argv[0]);
    }
    /* Check if file exists and will be overwriten */
    if ( strcasecmp(mode,MODE_WRITE) == 0 )
    {
      if ( (out=fopen(output,"r")) !=NULL )
      {
        int op=0;
        fclose(out);
        do {
          printf("Overwrite file \"%s\"? (y/n): ", output);
          op=getchar();
          if ( op == 'Y' ) op='y';
          if ( op == 'N' ) op='n';
        }while( op!='y' && op!='n' );

        if ( op == 'n' )
        {
          exit(EXIT_SUCCESS);
        }
      }
    }

    if ( (out=fopen(output,mode)) == NULL )
    {
      error_message("Error opening output filename", argv[0]);
    }

    srandom(time(NULL));
    /*
      Begin data processing
    */
    generate_data(name,surname,out,size);
    /*
      Close opened files
    */
    fclose(name);
    fclose(surname);
    fclose(out);
    return 0;
}

unsigned long
initialize_words(FILE * f, char *** words)
{
  size_t read=0;
  int byte;
  unsigned long count = 0;
  *words=NULL;

  if ( fseek( f, 0L, SEEK_SET)<0 )
  {
      error( EXIT_FAILURE, errno, "Initializing words");
  }

  bzero(BUFFER, BUFFER_SIZE);
  while( (byte = fgetc(f)) != EOF )
  {
    if ( index ( NEW_LINE_CHARS, byte) == NULL )
    {

      BUFFER[read++] = byte;
      if ( byte == BUFFER_SIZE )
      {
        error( EXIT_FAILURE, 0, "File contains words bigger than BUFFER_SIZE");
      }
    }
    else
    {
      if ( read )
      {
        /* Copiamos la palabra */
        *words = (char **) realloc(*words,  sizeof(char *) * ( count +1 ) );
        (*words)[count++]= strdup(BUFFER);
        bzero(BUFFER, BUFFER_SIZE);
      }
      read = 0;
    }
  }
  if ( read )
  {
    /* Copiamos la palabra */
    *words = (char **) realloc(*words,  sizeof(char *) * ( count +1 ) );
    (*words)[count++]= strdup(BUFFER);
  }
  return count;
}

void
free_words(char ** words, unsigned long s)
{unsigned long i;
  for (i=0;i<s;i++)
  {
    free(words[i]);
  }
  free(words);
}

/* Writes Size records into out file */
void
generate_data(FILE *name, FILE * surname, FILE *out, unsigned size)
{
  unsigned i;
  unsigned long current_f, firstnames_count = initialize_words( name, & words[FIRSTNAMES]);
  unsigned long current_s, surnames_count = initialize_words( surname, & words[SURNAMES]);
  t_person_id * ids=(t_person_id *) malloc( sizeof(t_person_id)*size);
  t_person p;
  struct tm date;
  if ( firstnames_count == 0 || surnames_count == 0 )
  {
      error(EXIT_FAILURE,0,"There are no words to generate file");
  }
  for ( i=0 ; i < size ; i++ )
  {

    current_f = random() % firstnames_count;
    current_s = random() % surnames_count;
    date = generate_date();
    p = person_init(
          generate_id(ids,i),
          words[FIRSTNAMES][current_f],
          words[SURNAMES][current_s],
          date);
    person_write_csv(p,out);
  }
  free (ids);
  free_words (words[FIRSTNAMES], firstnames_count);
  free_words (words[SURNAMES], surnames_count);
}

t_person_id
generate_id(t_person_id *ids, unsigned pos)
{
  int ok=0;
  t_person_id i;
  do {
    unsigned aux;
    i = PERSON_ID_MIN + (random() % (PERSON_ID_MAX-PERSON_ID_MIN));
    ok = 1;
    for ( aux=0; ( aux < pos ) && ok; aux++ )
    {
      ok = ids[aux] != i;
    }
    if ( ok )
    {
      ids[pos] = i;
    }
  }while (!ok);
  return i;
}

struct tm
generate_date()
{unsigned long r, ts_from, ts_to;
  time_t ts;
  struct tm date, from, to;
  bzero(&from,sizeof(from));
  bzero(&to,sizeof(from));
  from.tm_year=90;
  from.tm_mon=0;
  from.tm_mday=1;
  to.tm_year=100;
  to.tm_mon=0;
  to.tm_mday=1;
  ts_from = mktime(&from);
  ts_to = mktime(&to);
  r = random();
  ts = (time_t) (ts_from + (r % (ts_to - ts_from+1)));
  localtime_r(&ts,&date);
  date.tm_hour = date.tm_min = date.tm_sec = 0; //Necesario para evitar diferencias por horas
  return date;
}
