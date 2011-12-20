#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

#include "kerne.h"


/******************************************************************************
 * Kerne funktioner til lidt af hvert.
 *****************************************************************************/

char* StdIn(char *s) {
  long p, i;
  p = 0;
  i = 1002;
  s = malloc(i * sizeof(char));
  while (scanf("%1000s", &s[p]) != -1) {
    p += strlen(&s[p]);
    i += 1000;
    
    s = realloc((void*) s, i * sizeof(int));
    if (s == 0) {
      perror("Ikke mere tilgængelig hukommelse");
      exit(1);
    }
  }
  return(s);
}


struct fasta * FastaIn() {
/******************************************************************************
 * Reads a sequence from stdin into a fasta structure.
 *****************************************************************************/
  long p, ps;
  size_t i;
  char c;
  char *s;
  struct fasta *fs;

  fs = malloc(sizeof(struct fasta));

  // First the header/name (> Name ......)
  i = 0;

  p = getline(&fs->navn, &i, stdin);
  fs->navn[strlen(fs->navn) - 1] = 0; 
  if (p == -1) {
    perror("getline");
    exit(4);
  }
  else if (p == EOF) {
    free(fs->navn);
    free(fs);
    return NULL;
  }
  else if (fs->navn[0] != '>') {
    fprintf(stderr, "Input error : The stdin should be fasta formated\n");
    exit(1);
  }

  i = 0;
  s = 0;

  fs->seq = malloc(1 * sizeof(char));
  ps = 0;
  while ((c = getchar()) != '>' || c == EOF) {
    //while not EOF or new name
    ungetc(c, stdin);
    p = getline(&s, &i, stdin);

    if (p == EOF)
      break;

    if (p < 0) {
      perror("getline");
      break;
    }

    ps += p;

    fs->seq = (char * ) realloc(fs->seq, (ps + p)  * sizeof(char));
    strcpy(&fs->seq[ps - p], s);

    fs->seq[--ps] = 0;

    free(s);
    p = i = 0;
    s = 0;
  }
  ungetc(c, stdin);  
  free(s);
  fs->length = strlen(fs->seq);

  return(fs);
}


