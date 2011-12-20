#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#ifndef MAX_CANON
#define	MAX_CANON	256
#endif

size_t Getdelim (char ** lineptr, size_t *n, int terminator, FILE *stream) {
  char *line, *p;
  size_t size, copy;
  
  if ((stream < 0) || lineptr == NULL || n == NULL) {
    //    __set_errno (EINVAL);
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

