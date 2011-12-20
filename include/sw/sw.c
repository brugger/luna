/******************************************************************************
 * SW.c 
 * Smith-Watermann implementation.
 * Is used to write sequences that have insertions and/or deletions.
 *
 * Coded by Kim Brugger August 2000. Version 1.0
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>

#include "sw.h"

#define p(i,j) (i == j ? HitValue : MissValue)

int Max(int a, int b, int c) {
/******************************************************************************
 * Finds the Max of 3 values.
 *****************************************************************************/

  if (a > b)
    if (a > c)
      return a;
    else
      return c;
  else
    if (b > c)
      return b;
    else
      return c;
}

struct Matrix * BuildMatrix(const char *hseq , const char * vseq) {
/******************************************************************************
 * Builds the matrix, that is going to be used to calculate the score matrix
 *****************************************************************************/
  int i,j;
  struct Matrix * matrix = malloc(sizeof(struct Matrix));

  matrix->hlen = strlen(hseq);
  matrix->vlen = strlen(vseq);

  // Skaber en matrice der er Sub * Super stor. 
  matrix->matrix = (int **) malloc(matrix->hlen * sizeof(int**));

  if (matrix->matrix == 0) {
    perror("BuildMatrix");
    exit(1);
  }

  for (i = 0; i < matrix->hlen; i++){
    matrix->matrix[i] = (int*) malloc(matrix->vlen * sizeof(int*));
    if (matrix->matrix[i] == 0) {
      perror("BuildMatrix");
      exit(1);
    }
  }

  for (i = 0; i < matrix->hlen; i++) 
    for (j = 0; j < matrix->vlen; j++) 
      matrix->matrix[i][j] = 0;

  return matrix;
}

int CalcMatrix(struct Matrix * matrix, const char * hseq, const char * vseq) {
/******************************************************************************
 * Calculate the values in the matrix
 *****************************************************************************/
  int i, j;
  int a,b,c;
  matrix->matrix[0][0] = 0;

  for (j = 0; j < matrix->vlen; j++)
    for (i = 0; i < matrix->hlen; i++) {
      // (0,0) is already canculated
      if (!i && !j) continue;

      // Ved over kanten...
      if (i == 0)
	matrix->matrix[i][j] = p(hseq[i],vseq[j]) + (j - 1)*-2;

      // Ved venstre kanten...
      if ((j == 0))
	matrix->matrix[i][j] = p(hseq[i],vseq[j]) + (i - 1)*-2;

      if (i && j) {
	a = matrix->matrix[i- 1][j - 1] + p(hseq[i],vseq[j]);
	b = matrix->matrix[i-1][j] + GapValue;
	c = matrix->matrix[i][j-1] + GapValue;
	matrix->matrix[i][j] = Max(a, b, c);
      }
    }

  return 1;
}

int PrintMatrix(struct Matrix * matrix, const char * hseq, const char* vseq) {
/******************************************************************************
 * Print the full matrix
 *****************************************************************************/

  int i, j;

  printf("Matrix:\n    ");
  for (i = 0; i < matrix->hlen;i++)
    printf("%3c", hseq[i]);
  printf("\n    ");
  for (i = 0; i < matrix->hlen;i++)
    printf("---");
  printf("\n");

  for (j = 0; j < matrix->vlen; j++) {
    printf("%3c|", vseq[j]);
    for (i = 0; i < matrix->hlen; i++)
      printf("%3d", matrix->matrix[i][j]);
    printf("| %3d\n", j);
  }

  printf("    ");
  for (i = 0; i < matrix->hlen;i++)
    printf("---");
  printf("\n");
  printf("    ");
  for (i = 0; i < matrix->hlen;i++)
    printf("%3d", i);
  printf("\n");

  return 1;

}

int PrintMatrixHits(struct Matrix * matrix, 
		    const char * hseq, const char* vseq, const int min) {
/******************************************************************************
 * Prints the hits, with a lower cutoff
 *****************************************************************************/
  int i, j, k;
  char *s1, *s2, *s3;
  j = matrix->vlen - 1;
  // først findes længden på den fundne løsning....
  k = 0;
  for(i = matrix->hlen - 1; i >= 0;i--) {
    if (i && j && 
	(matrix->matrix[i][j] == matrix->matrix[i-1][j] + GapValue)) {
      k++;
    }
    else if (i && j && 
	     (matrix->matrix[i-1][j-1] == matrix->matrix[i][j]) + 
	     p(hseq[i],vseq[j])) {
      k++;
      j--; 
    }
    else if (i && j) {
      j--; i++;
      k++;
    }
    else { 
      k++;
      break;
    }
  }
  k++;
  // Allocate 3 strings....
  s1 = (char*) malloc ((k) * sizeof(char)); 
  s2 = (char*) malloc ((k) * sizeof(char));
  s3 = (char*) malloc ((k) * sizeof(char));  
  k--;
  s3[k] = s2[k] = s1[k] = 0x00;
  k--;
  j = matrix->vlen - 1;
  // Så indskrives den i de to char*'ere
  for(i = matrix->hlen - 1; i >= 0;i--) {
    if ((i && j )&& 
	(matrix->matrix[i][j] == matrix->matrix[i-1][j] + GapValue)) {
      s1[k] = hseq[i];
      s2[k] = '-';
      s3[k] = ' ';
      k--;
    }
    else if ((i && j) && 
	     (matrix->matrix[i][j] == matrix->matrix[i][j-1] + GapValue)) {
      s1[k] = '-';
      s2[k] = vseq[j];
      s3[k] = ' ';
      j--; i++;
      k--;
    }
    else if (i && j && 
	     (matrix->matrix[i][j] == matrix->matrix[i-1][j-1]) + 
	     p(hseq[i],vseq[j])) {
      s1[k] = hseq[i];
      s2[k] = vseq[j];
      if (hseq[i] == vseq[j])
	s3[k] = '|';
      else 
	s3[k] = ' ';
      k--;
      j--; 
    }
    else { 
      s1[k] = hseq[i];
      s2[k] = vseq[j];
      if (hseq[i] == vseq[j])
	s3[k] = '|';
      else 
	s3[k] = ' ';
      break;
    }
  }
 
  printf("%s\n%s\n%s\n",s1,s3,s2);
  free(s1);
  free(s2);
  free(s3);
  
  return 1;
}

int FreeMatrix(struct Matrix* matrix) {
/******************************************************************************
 * Frees the memory that the matrix have been using.
 *****************************************************************************/
  int i;
  for (i = 0; i < matrix->hlen; i++)
    free(matrix->matrix[i]);
  free(matrix->matrix);
  free(matrix);
  
  return 1;
}

void MatrixHits(struct Matrix* matrix, const char * hseq, const char* vseq, 
		char ** ResHseq, char** ResVseq) {
  //void MatrixHits(int ** matrix, const char * Super, const char* Sub, 
  //		char ** ResSuper, char** ResSub) {
/******************************************************************************
 * Calculates the hits and returns them.
 *****************************************************************************/
  int i, j, k;
  char *s1, *s2;
  j = matrix->hlen - 1;

  // først findes længden på den fundne løsning....
  k = 0;
  for(i = matrix->vlen - 1; i >= 0;i--) {
    if (i && j && 
	(matrix->matrix[i][j] == matrix->matrix[i-1][j] + GapValue)) {
      k++;
    }
    else if ((i && j) && 
	     (matrix->matrix[i-1][j-1] == matrix->matrix[i][j]) + 
	     p(vseq[i],hseq[j])) {
      j--; 
      k++;
    }
    else if (i && j) {
      j--; i++;
      k++;
    }
    else { 
      k++;
      break;
    }
  }
  k++;
  // Allocate two strings....
  s1 = (char*) malloc ((k) * sizeof(char)); 
  s2 = (char*) malloc ((k) * sizeof(char));
  k--;
  s2[k] = s1[k] = 0x00;
  k--;
  j = matrix->hlen - 1;
  // Så indskrives den i de to char*'ere
  for(i = matrix->vlen - 1; i >= 0;i--) {
    if ((i && j) && 
	(matrix->matrix[i][j] == matrix->matrix[i-1][j] + GapValue)) {
      s1[k] = vseq[i];
      s2[k] = '-';
      k--;
    }
    else if ((i && j) &&
	     (matrix->matrix[i][j] == matrix->matrix[i][j-1] + GapValue)) {
      s1[k] = '-';
      s2[k] = hseq[j];
      j--; i++;
      k--;
    }
    else if ((i && j) && 
	     (matrix->matrix[i][j] == matrix->matrix[i-1][j-1]) + p(vseq[i],hseq[j])) {
      s1[k] = vseq[i];
      s2[k] = hseq[j];
      k--;
      j--; 
    }
    else { 
      s1[k] = vseq[i];
      s2[k] = hseq[j];
      break;
    }
  }
 
  *ResVseq   = (char*) s1;
  *ResHseq = (char*) s2;
  return;
}
