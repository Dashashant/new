#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Называйте константы либо так STRING_SIZE, либо хотя бы StringSize, чтобы они отличались от обычных переменных.
 */

#define STRING_SIZE 100
#define DELIM_SIZE 100 

void Split(char* string, char* delim, char*** tokens, int* Count)
{
  int i;
  char *pch = strtok(string, delim);
  char **tmp;

  printf("\nЛексемы:");
  /*
   * malloc был лишним
   */
  tmp = NULL;

  while (pch != NULL) 
  {
    //  printf("\n%s", pch);
    tmp = (char**) realloc(tmp,(int) (sizeof(char*) * (*Count + 1)));

    *tokens = tmp;
    (*tokens)[*Count] = pch;
    (*Count)++;
    pch = strtok (NULL, delim);
  }
}

int main()
{
  char *str = (char *) malloc(sizeof(char) * STRING_SIZE);
  char *delimiters = (char *) malloc(sizeof(char) * DELIM_SIZE);
  char **words;
  int i, count = 0;

  gets(str);
  gets(delimiters);
  Split(str, delimiters, &words, &count);

  for (i = 0; i < count; i++)
  {
    printf("\n%s", words[i]);
  }

  /*
   * Надо чистить самим память.
   */
  
  return 0;
}

/*
 * В целом всё нормально. Засчитано.
 */
