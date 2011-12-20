/******************************************************************************
 * Kerne funktioner til lidt af hvert.
 *****************************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>


#include "kerne.h"
#include "texts.h"

char* ProgramName;

int   klgde      =  8;
int   back       =  7;
int   verbose    =  0;
int   gap        =  4;
int   min        = 50;
int   max        =  0;
int   DistMin    =  0;
int   DistMax    =  0;
int   flank      =  0;
int   direction  =  1;
int   currentDir =  0; //=forward, 1=reverse
int   PageWidth  = 80;
int   PrintForm  =  1;
int   SeqLgth    =  0;
int   Frag       =  0;
int   Insert     =  0;


size_t strnlen (const char *string, size_t maxlen) {
/******************************************************************************
 *
 *****************************************************************************/
  const char *end = memchr (string, '\0', maxlen);
  return end ? end - string : maxlen;
}


char * strndup (const char * s, size_t n) {
/******************************************************************************
 *
 *****************************************************************************/
  size_t len = strnlen (s, n);
  char *new = malloc (len + 1);

  if (new == NULL)
    return NULL;

  new[len] = '\0';
  return memcpy (new, s, len);
}

char * strndupp (const char * s, size_t n, char * t) {
/******************************************************************************
 * t is allocated before this function is called ...
 *****************************************************************************/
  size_t len = strnlen (s, n);

  t[len] = '\0';
  return memcpy (t, s, len);
}
// balrog shamen summon

char* StdIn(char *Res) {
/******************************************************************************
 *
 *****************************************************************************/
  long p, ps;
  size_t i;
  char c;
  char *s, *ResS;


  i = 0;
  s = 0;

  ResS = malloc(1 * sizeof(char));
  ps = 0;
  while ((c = getchar()) != '>' || c == EOF) {
    //while not EOF or new name
    ungetc(c, stdin);
    p = Getline(&s, &i, stdin);

    if (p == EOF)
      break;

    if (p < 0) {
      perror("getline");
      break;
    }

    ps += p;

    ResS = (char * ) realloc(ResS, (ps + p)  * sizeof(char));
    strcpy(&ResS[ps - p], s);

    ResS[--ps] = 0;

    free(s);
    p = i = 0;
    s = 0;
  }
  if (c == '>') {
    fprintf(stderr, 
	    "File is in fasta format. It should be in \"pure\" sequence.\n");
    exit(-10);
  }
  ungetc(c, stdin);  
  free(s);

  Res = ResS;
  return(Res);
}

char * RevStr(char * s) {
/******************************************************************************
 * Reveres a string the DNA-way.
 *****************************************************************************/
  char * r;
  long i, j;

  i = strlen(s);
  r =  malloc((i + 2) * sizeof(char));

  if (r == 0) {
    perror("String allocating\n");
    exit(2);
  }

  r[i] = 0;
  for (j = 0; j <= i - 1; j++) {
    if (1)
      switch (s[i - j -1]) { 
        case 'A' : case 'a' : r[j] = 'T'; break;
        case 'T' : case 't' : r[j] = 'A'; break;
        case 'C' : case 'c' : r[j] = 'G'; break;
        case 'G' : case 'g' : r[j] = 'C'; break;
      } 
    else
      r[j] = s[i-j-1];
  }
  return(r);
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

  p = Getline(&fs->navn, &i, stdin);
  fs->navn[strlen(fs->navn) - 1] = 0; 
  if (p == -1) {
    perror("FastaIn ");
    exit(4);
  }
  else if (p == EOF) {
    free(fs->navn);
    free(fs);
    return NULL;
  }
  else if (fs->navn[0] != '>') {
    fprintf(stderr, "Input error : The stdin should be fasta formated %s\n", 
	    fs->navn);
    exit(1);
  }

  i = 0;
  s = 0;

  fs->seq = NULL;//malloc(1 * sizeof(char));
  ps = 0;
  while ((c = getchar()) != '>' || c == EOF) {
    //while not EOF or new name
    ungetc(c, stdin);
    p = Getline(&s, &i, stdin);

    if (p == EOF)
      break;

    if (p < 0) {
      perror("getline");
      break;
    }

    ps += p;

    fs->seq = (char * ) realloc(fs->seq, (ps+p)  * sizeof(char));
    strcpy(&fs->seq[ps - p], s);

    fs->seq[--ps] = 0;

    //free(s);
    p = 0;
    //s = 0;
  }
  ungetc(c, stdin);  
  free(s);
  fs->length = strlen(fs->seq);

  return(fs);
}

//char last = 0;

//int ATGConly(char* s) {
/******************************************************************************
 * Checks that a string only contains ATGC and no ducks.
 *****************************************************************************/
/* int i;
 // This is a unsafe for loop, but we'll risk it for the speed..

 i = klgde -1;
 if (last) {
   if (s[i] == 'A' ||
       s[i] == 'C' ||
       s[i] == 'G' ||
       s[i] == 'T' ||
       s[i] == 'a' ||
       s[i] == 'c' ||
       s[i] == 'g' ||
       s[i] == 't') 
     return 1;
   else {
     last = 0;
     return 0;
   }
 }

 for (i =0; i != klgde;) 
   if (s[i] == 'A' ||
       s[i] == 'C' ||
       s[i] == 'G' ||
       s[i] == 'T' ||
       s[i] == 'a' ||
       s[i] == 'c' ||
       s[i] == 'g' ||
       s[i] == 't')
      i++;
   else 
     return(0);
 last = 1;
 return(1);
 }*/

void printSeqAlign(const int Start1, const int Stop1, const int Start2, 
		   const int Stop2, const char * s1, const char * s2) {
/******************************************************************************
 * Makes the alligned output, the output width = Bredde.
 * Also controls that upper cutof is not printed
 *****************************************************************************/
  int i, c, dist, lgde, Bredde, start1, stop1, start2, stop2;
  char *t;
  start1 = Start1;
  start2 = Start2;
  stop1  = Stop1;
  stop2  = Stop2;
  dist = stop1 - start1 + 1;
  c = 0;


  printf("\n> %d->%d %d->%d length : %d  distance : %d", start1 + 1, stop1 + 1,
	  start2 + 1, stop2 + 1 , stop1 - start1 + 1, start2 - start1 +1);

  c = 0;
  for (i = 0; i< dist ; i++)
    if ((s1[i + start1] != '-')&& (s1[i + start1] == s2[i + start2]))
      c++;

  printf(" percentage : %d %%\n", c * 100 / dist);
      
  i = c = 0;
  Bredde = PageWidth - 10;
  // should be made by wrapping...... ___LATER___
  if (((start1 - flank) > 0) &&
      ((start2 - flank) > 0)) {
    start1 -= flank;
    start2 -= flank;
    dist   += flank;
  }
  if (((stop1 + flank) < SeqLgth) &&
      ((stop2 + flank) < SeqLgth)) {
    stop1 += flank;
    stop2 += flank;
    dist   += flank;
  }
  t = (char*) malloc((Bredde + 1) * sizeof(char*));
  while (dist) {
    if (dist < Bredde)
      lgde = dist;
    else 
      lgde = Bredde;
    
    printf("%-4d->%-4d\t%s\n", start1 + c*Bredde , 
	   start1 + c*Bredde +lgde, (char *)strndupp(&s1[start1+c*Bredde], lgde, t));

    printf("          \t");
    for (i = 0+c*Bredde; i< c*Bredde+lgde ; i++) {
      if ((s1[i + start1] == s2[i + start2]) ||
	  (tolower(s1[i + start1]) == tolower(s2[i + start2])))
	printf("|");
      else
	printf(" ");
    }
    printf("\n");
        
    printf("%-4d->%-4d\t%s\n", start2 + c*Bredde , 
	   start2 + c*Bredde +lgde, (char *) strndupp(&s2[start2+c*Bredde], lgde, t));
    c++;
    printf("\n");
    dist -= Bredde;
    if (dist <= 0)
      dist = 0;
  }
  free(t);
}


void printSeqOnly(const int Start1, const int Stop1, const int Start2, 
		  const int Stop2, const char * s1, const char * s2) {
/******************************************************************************
 * Prints the found sequences as "naked"
 *****************************************************************************/
  int dist,  start1, stop1, start2, stop2;
  start1 = Start1;
  start2 = Start2;
  stop1  = Stop1;
  stop2  = Stop2;
  dist = stop1 - start1 + 1;

  // should be made by wrapping...... ___LATER___
  if (((start1 - flank) > 0) &&
      ((start2 - flank) > 0)) {
    start1 -= flank;
    start2 -= flank;
    dist   += flank;
  }
  if (((stop1 + flank) < SeqLgth) &&
      ((stop2 + flank) < SeqLgth)) {
    stop1 += flank;
    stop2 += flank;
    dist   += flank;
  }
  
  printf("%s\n", (char *)strndup(&s1[start1], dist));
  printf("%s\n", (char *)strndup(&s1[start1], dist));
}


void printPosOnly(const int Start1, const int Stop1, const int Start2, 
		  const int Stop2, const char * s1, const char * s2) {
/******************************************************************************
 * Prints the found positions only.
 *****************************************************************************/
  int dist, start1, stop1, start2, stop2;
  int i, c;
  start1 = Start1;
  start2 = Start2;
  stop1  = Stop1;
  stop2  = Stop2;
  dist = stop1 - start1 + 1;

  printf("> %d->%d %d->%d length : %d  distance : %d", start1 + 1, stop1 + 1,
	  start2 + 1, stop2 + 1 , stop1 - start1 + 1, start2 - start1 +1);

  c = 0;
  for (i = 0; i< dist ; i++)
    if (s1[i + start1] == s2[i + start2])
      c++;

  printf(" percentage : %d %%\n", c * 100 / dist);


}


void printSWSeqAlign(const int Start1, const int Stop1, const int Start2, 
		     const int Stop2, const char * s1, const char * s2) {
/******************************************************************************
 * Makes the alligned output, the output width = Bredde.
 * Also controls that upper cutof is not printed
 *****************************************************************************/
  int i, c, dist, lgde, Bredde, start1, stop1, start2, stop2;
  //int ** matrix;
  struct Matrix *matrix;
  char *S1, *S2, *reS1, *reS2, *t;

  start1 = Start1;
  start2 = Start2;
  stop1  = Stop1;
  stop2  = Stop2;
  dist = stop1 - start1 + 1;
  c = 0;

  if (((start1 - flank) > 0) &&
      ((start2 - flank) > 0)) {
    start1 -= flank;
    start2 -= flank;
    dist   += flank;
  }
  if (((stop1 + flank) < SeqLgth) &&
      ((stop2 + flank) < SeqLgth)) {
    stop1 += flank;
    stop2 += flank;
    dist   += flank;
  }

  printf("\n> %d->%d %d->%d (sw) length : %d  distance : %d", start1 + 1, stop1 + 1,
	 start2 + 1, stop2 + 1 , stop1 - start1 + 1, start2 - start1 +1);
  
  S1 = strndup(&s1[start1], stop1 - start1 + 1);
  S2 = strndup(&s2[start2], stop2 - start2 + 1);
  matrix = BuildMatrix(S1, S2);
  CalcMatrix(matrix, S1, S2);
  MatrixHits(matrix, S1, S2, &reS1, &reS2);

  c = 0;
  for (i = 0; i< strlen(reS1); i++)
    if ((reS1[i] != '-')&& (reS1[i] == reS2[i]))
      c++;

  printf(" percentage : %d %%\n", c * 100 / dist);
      
  i = c = 0;
  Bredde = PageWidth - 10;
  // should be made by wrapping...... ___NEVER___
  t = (char*) malloc((Bredde + 1) * sizeof(char));
  dist = strlen(reS1);
  while (dist) {
    if (dist < Bredde)
      lgde = dist;
    else 
      lgde = Bredde;
    
    printf("%-6d->%-6d\t%s\n", start1 + c*Bredde , 
	   start1 + c*Bredde +lgde, 
	   (char *)strndupp(&reS1[c*Bredde], lgde, t));

    printf("          \t");
    for (i = 0+c*Bredde; i< c*Bredde+lgde ; i++) {
      if (reS1[i] == reS2[i])
	printf("|");
      else
	printf(" ");
    }
    printf("\n");
        
    printf("%-6d->%-6d\t%s\n", start2 + c*Bredde , 
	   start2 + c*Bredde +lgde, 
	   (char *) strndupp(&reS2[c*Bredde], lgde, t));
    c++;
    printf("\n");
    dist -= Bredde;
    if (dist <= 0)
      dist = 0;
  }

  // En eller anden sær fejl her ..... !!!
  //printf("res1 %p res2 %p\n", reS1, reS2);
    
  free(reS1);
  free(reS2);

  FreeMatrix(matrix);

  free(S1);
  free(S2);
  
  free(t);
}


void printSWSeqOnly(const int Start1, const int Stop1, const int Start2, 
		  const int Stop2, const char * s1, const char * s2) {
/******************************************************************************
 * Prints the found sequences as "naked"
 *****************************************************************************/
  int dist,  start1, stop1, start2, stop2;
  start1 = Start1;
  start2 = Start2;
  stop1  = Stop1;
  stop2  = Stop2;
  dist = stop1 - start1 + 1;

  // should be made by wrapping...... ___LATER___
  if (((start1 - flank) > 0) &&
      ((start2 - flank) > 0)) {
    start1 -= flank;
    start2 -= flank;
    dist   += flank;
  }
  if (((stop1 + flank) < SeqLgth) &&
      ((stop2 + flank) < SeqLgth)) {
    stop1 += flank;
    stop2 += flank;
    dist   += flank;
  }
  
  printf("%s\n", (char *)strndup(&s1[start1], dist));
  printf("%s\n", (char *)strndup(&s1[start1], dist));
}


void printSWPosOnly(const int Start1, const int Stop1, const int Start2, 
		  const int Stop2, const char * s1, const char * s2) {
/******************************************************************************
 * Prints the found positions only.
 *****************************************************************************/
  int dist, start1, stop1, start2, stop2;
  int i, c;
  start1 = Start1;
  start2 = Start2;
  stop1  = Stop1;
  stop2  = Stop2;
  dist = stop1 - start1 + 1;

  printf("> %d->%d %d->%d length : %d  distance : %d", start1 + 1, stop1 + 1,
	  start2 + 1, stop2 + 1 , stop1 - start1 + 1, start2 - start1 +1);

  c = 0;
  for (i = 0; i< dist ; i++)
    if (s1[i + start1] == s2[i + start2])
      c++;

  printf(" percentage : %d %%\n", c * 100 / dist);


}


char * ShortFlagValue(char ** ss, int i) {
/******************************************************************************
 * 
 *****************************************************************************/
  if (ss[i][2] != 0)
    return (&ss[i][2]);
  else if (ss[i++] != NULL)
    return (&ss[i][0]);
  else
    return NULL;
}

char * LongFlagValue(char ** s, int i, char* flagvalue) {
/******************************************************************************
 * 
 *****************************************************************************/
  int l = strlen(flagvalue);
  if (s[i][l] != 0)
    return (&s[i][l]);
  else if (s[i++] != NULL)
    return (&s[i][0]);
  else
    return NULL;
}


void CommandLine(int argc, char **argv) {
/******************************************************************************
 * I parse therefore I rule.
 *****************************************************************************/
  int i;

  ProgramName = argv[0];
  // We dont wanna display the ugly ./ in the program name.....
  if (ProgramName[0] == '.' && ProgramName[1] == '/')
    ProgramName += 2;
  
  for (i = 0; i < argc;i++) {
    if (argv[i][0] == '-' && argv[i][1] != '-')
      switch (argv[i][1]) {
      case 'k':
	klgde = strtol(ShortFlagValue(argv, i),NULL,  0); 
	break;
	  
      case 'g':
	gap = strtol(ShortFlagValue(argv, i),NULL,  0);
	break;

      case 'f' :
	flank = strtol(ShortFlagValue(argv, i),NULL,  0);
	break;

      case 'm':
	min = strtol(ShortFlagValue(argv, i),NULL,  0);
	break;

      case 'M':
	max = strtol(ShortFlagValue(argv, i),NULL,  0);
	break;

      case 'd':
	DistMin = strtol(ShortFlagValue(argv, i),NULL,  0); 
	break;

      case 'D':
	DistMax = strtol(ShortFlagValue(argv, i),NULL,  0);
	break;

      case 'F':
	Frag = strtol(ShortFlagValue(argv, i),NULL,  0); 
	break;

      case 'v':
	verbose++; break;

      case 'V':
	version(); break;

      case 'h':
	usage();
	break;
	
      case 'I':
	Insert = strtol(ShortFlagValue(argv, i),NULL,  0);
	break; 

      default: 
	printf("Illegal flag : %s. Try %s -h for help.\n", argv[i], ProgramName);
	exit(-1);
      }
    else if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (!strncmp(argv[i], "--ktub", strlen("--ktub"))) 
	klgde = strtol(LongFlagValue(argv, i, "--ktub"),NULL,0);
      else if (!strncmp(argv[i], "--direction", strlen("--direction")))
	direction = strtol(LongFlagValue(argv, i, "--direction"),NULL,0);
      else if (!strncmp(argv[i], "--backstep", strlen("--backstep")))
	back = strtol(LongFlagValue(argv, i, "--backstep"),NULL,0);
      else if (!strncmp(argv[i], "--gap", strlen("--gap")))
	gap = strtol(LongFlagValue(argv, i, "--gap"),NULL,0);
      else if (!strncmp(argv[i], "--min", strlen("--min")))
	min = strtol(LongFlagValue(argv, i, "--min"),NULL,0);
      else if (!strncmp(argv[i], "--max", strlen("--max")))
	max = strtol(LongFlagValue(argv, i, "--max"),NULL,0);
      else if (!strncmp(argv[i], "--DistMin", strlen("--DistMin")))
	DistMin = strtol(LongFlagValue(argv, i, "--DistMin"),NULL,0);
      else if (!strncmp(argv[i], "--DistMax", strlen("--DistMax")))
	DistMax = strtol(LongFlagValue(argv, i, "--DistMax"),NULL,0);
      else if (!strncmp(argv[i], "--flank", strlen("--flank")))
	flank = strtol(LongFlagValue(argv, i, "--flank"),NULL,0);
      else if (!strncmp(argv[i], "--pagewidth", strlen("--pagewidth")))
	PageWidth = strtol(LongFlagValue(argv, i, "--pagewidth"),NULL,0);
      else if (!strncmp(argv[i], "--printout", strlen("--printout")))
	PrintForm = strtol(LongFlagValue(argv, i, "--printout"),NULL,0);
      else if (!strncmp(argv[i], "--verbose", strlen("--verbose")))
	verbose++;
      else if (!strncmp(argv[i], "--disclaimer", strlen("--disclaimer")))
	disclaim();	
      else if (!strncmp(argv[i], "--Disclaimer", strlen("--Disclaimer")))
	Disclaim();	
      else if (!strncmp(argv[i], "--help", strlen("--help")))
	usage();
      else if (!strncmp(argv[i], "--Version", strlen("--Version")))
	version();
      else if (!strncmp(argv[i], "--Insert", strlen("--Insert")))
	Insert = strtol(LongFlagValue(argv, i, "--Insert"),NULL,0);
      else {
	printf("Illegal flag : %s. Try %s -h for help.\n", argv[i], ProgramName);
	exit(-1);
      }
    }
  }
  back = klgde - 1;

  // calculate the real read value
  //gap += klgde;

  if (max && (max < min)) {
    fprintf(stderr, "Maximum length (%d) is shorter minimum length (%d)\n", 
	    max, min);
    exit(-1);
  }

}


#ifndef MAX_CANON
#define	MAX_CANON	256
#endif

size_t Getdelim (char ** lineptr, size_t *n, int terminator, FILE *stream) {
  char *line, *p;
  size_t size, copy;
  
  if ((stream < 0) || lineptr == NULL || n == NULL) {
    fprintf(stderr, "EINVAL\n");
    return -1;
  }

  if (ferror (stream))
    return -1;
  
  // Make sure we have a line buffer to start with.
  if ((*lineptr == NULL) || (*n < 2))  { // !seen and no buf yet need 2 chars.
    line = malloc (MAX_CANON);
    if (line == NULL)
      return -1;
    *lineptr = line;
    *n = MAX_CANON;
  }

  line = *lineptr;
  size = *n;

  copy = size;
  p = line;

  while (1) {
    size_t len;

    while (--copy > 0) {
      register int c = getc (stream);
      if (c == EOF)
	goto lose;
      else if ((*p++ = c) == terminator)
	goto win;
    }

    /* Need to enlarge the line buffer.  */
    len = p - line;
    size *= 2;
    line = realloc (line, size);
    if (line == NULL)
      goto lose;
    *lineptr = line;
    *n = size;
    p = line + len;
    copy = size - len;
  }

 lose:
  if (p == *lineptr)
    return -1;
  /* Return a partial line since we got an error in the middle.  */
 win:
  *p = '\0';
  return p - *lineptr;
}

/* Like getdelim, but always looks for a newline.  */
size_t Getline (char **lineptr, size_t *n, FILE *stream) {
  return Getdelim (lineptr, n, '\n', stream);
}

