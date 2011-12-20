/****************************************************************************************
 * Kerne funktioner til lidt af hvert.
 ***************************************************************************************/
#ifndef __KERNE
#define __KERNE

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#include "sw.h"

#ifdef DMALLOC
#include "dmalloc.h"
#endif

#define nocp

struct fasta {
  char * navn, * seq;
  int length;
};

extern char * ProgramName;
extern const char * Version;

extern int klgde;
extern int back;
extern int gap;
extern int Mem;
extern int verbose;
extern int min;
extern int max;
extern int DistMin;
extern int DistMax;
extern int PageWidth;
extern int PrintForm;
extern int flank;
extern int direction;
extern int currentDir;
extern int SeqLgth;
extern int Frag;
extern int Insert;

char* StdIn(char *s);
/******************************************************************************
 * Læser alt fra stdin.
 *****************************************************************************/

char * RevStr(char * s);
/******************************************************************************
 * Reveres a string the DNA-way.
 *****************************************************************************/


size_t Getline (char **lineptr, size_t *n, FILE *stream);
/******************************************************************************
 * Reads a sequence from stdin into a fasta structure.
 *****************************************************************************/

struct fasta * FastaIn();
/******************************************************************************
 * Reads a sequence from stdin into a fasta structure.
 *****************************************************************************/

int ATGConly(char* s);
/******************************************************************************
 * Checks that a string only contains ATGC and no ducks.
 *****************************************************************************/

char *strndup (__const char *__string, size_t __n);
/******************************************************************************
 * 
 *****************************************************************************/

char * strndupp (const char * s, size_t n, char * t);
/******************************************************************************
 * t is allocated before this function is called ...
 *****************************************************************************/

extern void printSeq(const int start1, const int stop1, 
		     const int start2, const int stop2,
		     const int Ins);
/******************************************************************************
 * Global wrapping function for printing.
 *****************************************************************************/


void printSeqAlign(const int start1, const int stop1, const int start2, 
		  const int stop2, const char * s1, const char * s2);
/******************************************************************************
 * Makes the alligned output, the output width = Bredde.
 *****************************************************************************/

void printSeqOnly(const int Start1, const int Stop1, const int Start2, 
		  const int Stop2, const char * s1, const char * s2);
/******************************************************************************
 * Prints the found sequences as "naked"
 *****************************************************************************/

void printPosOnly(const int Start1, const int Stop1, const int Start2, 
		  const int Stop2, const char * s1, const char * s2);
/******************************************************************************
 * Prints the found sequences as "naked"
 *****************************************************************************/

void printSWSeqAlign(const int Start1, const int Stop1, const int Start2, 
		     const int Stop2, const char * s1, const char * s2);
/******************************************************************************
 * Makes the alligned output, the output width = Bredde.
 * Also controls that upper cutof is not printed. SWgappend print
 *****************************************************************************/

void printSWSeqOnly(const int Start1, const int Stop1, const int Start2, 
		    const int Stop2, const char * s1, const char * s2);
/******************************************************************************
 * Prints the found sequences as "naked"
 *****************************************************************************/

void printSWPosOnly(const int Start1, const int Stop1, const int Start2, 
		  const int Stop2, const char * s1, const char * s2);
/******************************************************************************
 * Prints the found positions only.
 *****************************************************************************/


void CommandLine(int argc, char**argv);
/******************************************************************************
 * I parse therefore I rule.
 *****************************************************************************/

void disclaim();
/******************************************************************************
 * The name says it all.
 *****************************************************************************/

void copyright();
/******************************************************************************
 * I own everything...
 *****************************************************************************/

void version();
/******************************************************************************
 * I own everything...
 *****************************************************************************/

void usage();
/******************************************************************************
 * The name says it all.
 *****************************************************************************/

void PrintVerbose();
/******************************************************************************
 * Prints verbose information.
 *****************************************************************************/

#endif
