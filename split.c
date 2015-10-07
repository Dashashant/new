#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define size_str 100
#define size_delim 100 

void Split(char* string, char* delim, char*** tokens, int* Count)
{
  int i;
  char *pch = strtok(string, delim);
  char **tmp;

  printf("\nЛексемы:");
  tmp = (char**) malloc(sizeof(char*) * 1);

  while (pch != NULL) 
  {
    //  printf("\n%s", pch);
    tmp = (char**) realloc(tmp,(int) (sizeof(char*) * (*Count + 1)));

    *tokens = tmp;
    (*tokens)[*Count] = pch;
    (*Count)++;
    pch = strtok (NULL, delim);
  }
  return; 
}
int main()
{
  char *str = (char *) malloc(sizeof(char) * size_str);
  char *delimiters = (char *) malloc(sizeof(char) * size_delim);
  char **words;
  int i, count = 0;

  gets(str);
  gets(delimiters);
  Split(str, delimiters, &words, &count);

  for (i = 0; i < count; i++)
  {
    printf("\n%s", words[i]);
  }

  return 0;
}
