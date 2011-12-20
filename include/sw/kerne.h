#ifndef __KERNE_H
#define __KERNE_H



struct fasta {
  char * navn, * seq , *revSeq;
  int length;
};


/******************************************************************************
 * Kerne funktioner til lidt af hvert.
 *****************************************************************************/

char* StdIn(char *s);

struct fasta * FastaIn();
#endif
