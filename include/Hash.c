/******************************************************************************
 * En hash liste, til opslag af DNA fragmenter
 *
 * Kodet af Kim Brugger Juli 2000 Email: bugger@diku.dk
 *****************************************************************************/
#include "Hash.h"

//typedef struct dhlist **DNAhash;

DNAhash MakeDNAhash(const int length) {
/******************************************************************************
 *
 *****************************************************************************/
  DNAhash tmpHash;
  int i;
  //  printf("making space for %d buckets\n",(int)pow(4,length));
  tmpHash =  malloc(sizeof(DNAhash) * (int)pow(4,length));

  if (tmpHash == 0) {
    perror("Hash allokering");
    exit(3);
  }
  for (i = 0; i < (int) pow(4,length); i++)
    tmpHash[i] = 0;

  return(tmpHash);
} /* MakeDNAhash()*/

enum {A = 0, C=1, G=2, T=3};

inline int ATGChash2(const char * s) {
/******************************************************************************
 * Calculates a  unique interger from a DNA seq. 
 * The algorithm was developed by Peter Redder (tm).
 * It calculates a values in a Quadroary-number system, and converts this number to
 * a decimal number.
 *****************************************************************************/
  int res;
  int i;
  res = 0;
  for(i = klgde - 1; i >=0  ;i--) {
    res <<= 2;
    switch (s[i]) {
    case 'A':
    case 'a': res |=A; break;

    case 'C':
    case 'c': res |=C;  break;

    case 'G':
    case 'g':  res |=G; break;

    case 'T':
    case 't':  res |=T; break;

    default : return(-1); break;
    }
  }
  return(res);
}

inline int FetchDNA(const char * s) {
/******************************************************************************
 * Fetch the wanted instance from the array.
 *****************************************************************************/
  return(ATGChash2(s));
}

void printDNAhash(const int length, const DNAhash hash) {
/******************************************************************************
 *
 *****************************************************************************/
  int i;
  for (i = 0; i < (int) pow(4,length); i++)
    printf("%d == %p\n",i,hash[i]);
}

