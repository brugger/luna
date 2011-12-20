/******************************************************************************
 * S3 (Sub-Sequence Statistic).
 *
 *
 * Kodet af Kim Brugger Maj 2000. Version 1.0
 *****************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <getopt.h>
#include <malloc.h>
#include <ctype.h>

#include "s3.h"
#include "kerne.h"
#include "sw.h"


#define dprintf if (1) printf
#define uprintf if (0) printf
#define txtHelp "%s [-m lower cutoff] [-h help]\n"

#define hit   2
#define miss -1

int main(int argc, char **argv){
  int c;
  struct fasta *orf, *ss;
  struct Matrix *matrix;

  struct mallinfo MI;
  //int ** matrix;
  // parser komandolinie flagene
  
  while ((c = getopt (argc, argv, "m:h")) != -1)
    switch (c) {
      
      case 'h':
        printf(txtHelp, argv[0]);
        return(1);
        break;

      case '?':
        if (isprint(optopt)) 
          fprintf (stderr, txtHelp, argv[0]);	
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;

      default:
        abort ();
      }



  // Indlæser sekvensen

  ss  = FastaIn();
  ss->length = strlen(ss->seq);
  
  orf = FastaIn();
  orf->length = strlen(orf->seq);

 
  printf("s1  : %s (%d)\ns2  : %s (%d)\n\n", 
	 ss->navn, ss->length, orf->navn, orf->length);
  
  matrix = BuildMatrix(orf->seq, ss->seq);
  PrintMatrix(matrix, orf->seq, ss->seq);
  CalcMatrix(matrix, orf->seq, ss->seq);
  PrintMatrix(matrix, orf->seq, ss->seq);
  PrintMatrixHits(matrix, orf->seq, ss->seq, 200);
  printf("FREEING MATRIX\n");
  FreeMatrix(matrix);
 
  printf("\nKørsels tid  %ld sek\n", (clock() /CLOCKS_PER_SEC));
  MI = mallinfo();
  printf("Allokeret hukommelse : %d kilobytes\n" , MI.arena / 1024);

  return(0);
}


// Introd. Computational Molecular Biology -- Setabal
