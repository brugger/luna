/******************************************************************************
 * Project name : luna (Locating Uniform poly-Nucleotide Algorithm).
 * Programmed by Kim Brugger summer 2000-summer to summer 2002, 
 * as part of a Master project at :
 * Molecular Biological Institute at Copenhagen University.
 *
 *
 * Version 2.00     Juli 2001
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

#ifdef DMALLOC
#include "dmalloc.h"
#endif

#define dprintf if (0) printf
#define uprintf if (0) printf

// Globale version variable
const char* Version = "2.00 Alpha";
struct fasta *seq, *revseq;

int main(int argc, char **argv){
  long   j, p;
  int    pos, last, flushpos;
#ifdef __LUNA_DEBUG
  clock_t start, end; 
#endif
  //struct Dnode*  Dnode;
  struct Dhash* DHash;
  struct mallinfo MI;
  struct list * liste;
  DNAhash  hashliste;

#ifdef __LUNA_DEBUG
  start = clock();
#endif

  CommandLine(argc, argv);
  // reading the sequence from stdin. 
  // should "know" different formats ... Later
  p =  0; 
  seq = FastaIn();

  if (direction != 2) {
    revseq = malloc(sizeof(struct fasta));
    revseq->seq = RevStr(seq->seq);
  }

  p = strlen(seq->seq);
  SeqLgth = p;  
  hashliste = MakeDNAhash(klgde);

  if (SeqLgth > 100000)
    flushpos = 10000;
  else
    flushpos = SeqLgth / 100;
  if (flushpos<100)
    flushpos = 100;

  printf("%s : Sequence length : %-7ld   Tuble : %2d Gap: %2d",
	 ProgramName, p, klgde, gap);

  if (Insert)
    printf("  Insert : %d", Insert);

  printf("\nSequence name : %s\n", seq->navn);

  //Dnode = MakeDNode();
  DHash = MakeDHash(p);

  if (!DistMin)
    DistMin = 1;

  last = 0;
  // if we only look for inverted repeats
  if (direction == 3) {
    DistMin = 0;
    if (verbose)
      fprintf(stderr, "Building k-probe database\n");
    for (j = 0; j < p - klgde; j+=klgde - back) {    
      liste = 0;
      pos = FetchDNA(&seq->seq[j]);
      if (pos == -1)
	continue;
      hashliste[pos] = insertNode(hashliste[pos]);
      hashliste[pos]->start = j;
      //liste = hashliste[pos];
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
  }
  else {
    if (verbose) {
      fprintf(stderr, "Finding direct repeats\n");
    //lets compare some things
      fprintf(stderr,"Progress : 0 %%");
    }
    for (j = 0; j != p - klgde; j+=klgde - back) {    
      liste = 0;
      pos = FetchDNA(&seq->seq[j]);
      if (pos == -1)
	continue;
      hashliste[pos] = insertNode(hashliste[pos]);
      hashliste[pos]->start = j;
      liste = hashliste[pos];
      dprintf("CalcDist\n");
      HashCalcDistGar(DHash, liste, j);
      
      if (verbose)
	if (((j * 100 / p) != last)) {
	  last = (j * 100 /p);
	  fprintf(stderr, "\rProgress : %d %%",  last);
	  fflush(stderr);
	}

      if ((j) && 
	  (j % flushpos)==0) {
	DHashFlush(DHash, j);
      }

    } 
    if (verbose)
      fprintf(stderr,"\rProgress : 100 %%\n");
    DHashLastFlush(DHash);
  }

  // If looking for the inverted repeats as well...
  last = 0;
  if (direction != 2) {
    currentDir = 1;
    fprintf(stdout, "\nInverted repeats\n");
    if (verbose)
      fprintf(stderr, "Finding inverted repeats\n");
    fflush(stderr);
    //DHashPrintAll(DHash, 1);

    for (j = 0; j < p - klgde; j+=klgde - back) {    
      liste = 0;
      pos = FetchDNA(&revseq->seq[j]);
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
	//DHashPrintAll(DHash, 1);
      }
    }
    if (verbose)
      fprintf(stderr, "\rProgress : 100 %%\n");
    DHashLastFlush(DHash);
  }

  // print the last hits, if they exists ...

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
  
  free(seq->seq);
  free(seq->navn);
  free(seq);
  if (direction != 2) {
    free(revseq->seq);
    free(revseq);
  }

  return(0);
}


// Just so we can share the pretty-printer, we have to wrap it in each main...
void printSeq(const int start1, const int stop1, 
	      const int start2, const int stop2,
	      const int Ins) {
/******************************************************************************
 * Global wrapping function for printing.
 *****************************************************************************/

  if (currentDir == 0) {
    if (Ins) {
      switch (PrintForm) {
      case 1 :  
	printSWSeqAlign(start1, stop1, start2, stop2, seq->seq, seq->seq);
	break;
      case 2 :  
	printSWSeqOnly(start1, stop1, start2, stop2, seq->seq, seq->seq);
	break;
      case 3 :  
	printSWPosOnly(start1, stop1, start2, stop2, seq->seq, seq->seq);
	break;
      default : 
	printSWSeqAlign(start1, stop1, start2, stop2, seq->seq, seq->seq);
	break;
      }
    }
    else {
      switch (PrintForm) {
      case 1 :  
	printSeqAlign(start1, stop1, start2, stop2, seq->seq, seq->seq);
	break;
      case 2 :  
	printSeqOnly(start1, stop1, start2, stop2, seq->seq, seq->seq);
	break;
      case 3 :  
	printPosOnly(start1, stop1, start2, stop2, seq->seq, seq->seq);
	break;
      default : 
	printSeqAlign(start1, stop1, start2, stop2, seq->seq, seq->seq);
	break;
      }
    }
  }
  else {
    if (Ins) {
      switch (PrintForm) {
      case 1 :  
	printSWSeqAlign(start1, stop1, start2, stop2, seq->seq, revseq->seq);
	break;
      case 2 :  
	printSWSeqOnly(start1, stop1, start2, stop2, seq->seq, revseq->seq);
	break;
      case 3 :  
	printSWPosOnly(start1, stop1, start2, stop2, seq->seq, revseq->seq);
	break;
      default : 
	printSWSeqAlign(start1, stop1, start2, stop2, seq->seq, revseq->seq);
	break;
      }
    }
    else {
      switch (PrintForm) {
      case 1 :  
	printSeqAlign(start1, stop1, start2, stop2, seq->seq, revseq->seq);
	break;
      case 2 :  
	printSeqOnly(start1, stop1, start2, stop2, seq->seq, revseq->seq);
	break;
      case 3 :  
	printPosOnly(start1, stop1, start2, stop2, seq->seq, revseq->seq);
	break;
      default : 
	printSeqAlign(start1, stop1, start2, stop2, seq->seq, revseq->seq);
	break;
      }
    }
  }
}  
// Håndværker : Henrik Gemynte, tlf: 4010-6295.
