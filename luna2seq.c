/******************************************************************************
 * luna2seq
 * Kodet af Kim Brugger sommeren 2000, som en del af et speciale på 
 * Molekylært biologisk institut på Københavns Universitet.
 *
 *
 * Version 1.00    4 Juli 2000
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <malloc.h>

#include "Hliste.h"
#include "Dliste.h"
#include "kerne.h"
#include "Hash.h"

#define dprintf if (0) printf
#define uprintf if (0) printf

#define hashtest 1

struct fasta *f1, *f2;

const char* Version = "2.00 Alpha";

#define dhash 0

int main(int argc, char **argv){
  long   j, p;
  int    pos, last, flushpos;
#ifdef __LUNA_DEBUG
  clock_t start, end; 
#endif
  struct Dhash* DHash;
  struct mallinfo MI;
  struct list * liste;
  DNAhash  hashliste;

#ifdef __LUNA_DEBUG
  start = clock();
#endif

  CommandLine(argc, argv);

  // reading the sequence from stdin. 
  p =  0; 
  f1 = FastaIn();
  f2 = FastaIn();

  hashliste = MakeDNAhash(klgde);

  if (SeqLgth > 100000)
    flushpos = 10000;
  else
    flushpos = SeqLgth / 100;
  if (flushpos<100)
    flushpos = 100;

  printf("%s : Sequence length : %-7ld   Tuble : %2d Gap: %2d",
	 ProgramName, p, klgde, gap - klgde);

  if (Insert)
    printf("  Insert : %d", Insert);

  printf("\nComparing sequences : \n\t%s (%d)\n\t%s (%d)\n", 
	 f1->navn, 
	 f1->length,
	 f2->navn,
	 f2->length);

  // find the longest string ...
  //p = (f1->length>f2->length) ? f1->length : f2->length;
  p = f1->length;

  DHash = MakeDHash(p);
  last = 0;

  DistMin = 0;
  if (verbose)
    fprintf(stderr, "Building k-probe database\n");
  for (j = 0; j < p - klgde; j+=klgde - back) {    
    liste = 0;
    pos = FetchDNA(&f1->seq[j]);
    if (pos == -1)
      continue;
    hashliste[pos] = insertNode(hashliste[pos]);
    hashliste[pos]->start = j;
    if (verbose)
      if ((!(j % 1)) &&
	  ((j * 100 / p) != last)) {
	last = (j * 100 /p);
	fprintf(stderr, "\rBuilding : %d %%",  last);
	fflush(stderr);
      }
  }
  if (verbose)
    fprintf(stderr, "\rBuilding : 100 %% .... Done\n");

  //Sammen ligningen del 2.
  //p = (f1->length>f2->length) ? f1->length : f2->length; //f2->length;
  p = f2->length;
  if (p > 100000)
    flushpos = 10000;
  else
    flushpos = p / 100;

  if (verbose) 
    fprintf(stderr, "Finding inverted repeats\n");

  for (j = 0; j < p - klgde; j+=klgde - back) {    
    liste = 0;
    pos = FetchDNA(&f2->seq[j]);
    if (pos == -1)
      continue;
    liste = hashliste[pos];
    HashCalcDistGar(DHash, liste, j);

    // Let me show you my hash signs while I work
    if (!(j % flushpos)) {
      last = (j * 100 / p);
      if (verbose)
	fprintf(stderr, "\rProgress : %d %%",  last);
      DHashFlush(DHash, j);
    }
  }
  if (verbose)
    fprintf(stderr, "\rProgress : 100 %%\n");
  DHashLastFlush(DHash);

  printf("\n\n");
  
  // So You wanna know the time and memory usage well... verbose we are.
  if (verbose) {
    PrintVerbose();
#ifdef __LUNA_DEBUG
    end = clock();
    fprintf(stderr, "Runtime  %.2f sec\n", (double) (end - start) / CLOCKS_PER_SEC);    
#endif

    MI = mallinfo();
    fprintf(stdout, "Allocated memory : %d kilobytes\n" , 
	    (MI.uordblks + MI.usmblks) / 1024);
  }
  
  return(0);
}

// Just so we can share the pretty-printer, we have to wrap it in each main...
void printSeq(const int start1, 
	      const int stop1, 
	      const int start2, 
	      const int stop2,
	      const int Ins) {
/******************************************************************************
 * ikke særlig køn men den virker ..... Skal kodes om
 *****************************************************************************/
  switch (PrintForm) {
  case 1 :  
    printSeqAlign(start1, stop1, start2, stop2, f1->seq, f2->seq);
    break;
  case 2 :  
    printSeqOnly(start1, stop1, start2, stop2, f1->seq, f2->seq);
    break;
  case 3 :  
    printPosOnly(start1, stop1, start2, stop2, f1->seq, f2->seq);
    break;
  default : 
    printSeqAlign(start1, stop1, start2, stop2, f1->seq, f2->seq);
    break;
  }
}
