/****************************************************************************************
 * SW.h
 * Smith-Watermann implementation.
 * Is used to write sequences that have insertions and/or deletions.
 *
 * Coded by Kim Brugger August 2000. Version 1.0
 ***************************************************************************************/
#ifndef __SW
#define __SW


#define HitValue   1
#define MissValue -1
#define GapValue  -2

struct Matrix{
  int ** matrix;
  int hlen, vlen;
};


struct Matrix * BuildMatrix(const char *hseq , const char * vseq);
/******************************************************************************
 * Builds the matrix, that is going to be used to calculate the score matrix
 *****************************************************************************/

int CalcMatrix(struct Matrix * matrix, const char * hseq, const char * vseq);
/******************************************************************************
 * Calculate the values in hthe matrix
 *****************************************************************************/

int PrintMatrix(struct Matrix * matrix, const char * hseq, const char* vseq);
/******************************************************************************
 * Print the full matrixs
 *****************************************************************************/

int PrintMatrixHits(struct Matrix * matrix, 
		    const char * hseq, const char* vseq, const int min);
/******************************************************************************
 * Prints the hits, with a lower cutoff
 *****************************************************************************/

int FreeMatrix(struct Matrix* matrix);
/******************************************************************************
 * Frees the memory that the matrix have been using.
 *****************************************************************************/

void MatrixHits(struct Matrix* matrix, const char * hseq, const char* vseq, 
		char ** ResHseq, char** ResVseq);
/******************************************************************************
 * Calculates the hits and returns them.
 *****************************************************************************/
#endif

