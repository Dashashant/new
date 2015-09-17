#include <stdio.h>
#include <string.h>
#define size_str 100
#define size_delim 10

void split(char *str, char *delimiters, char ***words, int *count)
{
  int i;
  char *s = strtok(str, delimiters);
  char **temp;
  *words = NULL;
  *count = 0;

  while(s != NULL)
  {
    temp = (char**)malloc((int)sizeof(char*) * (*count + 1));

    for (i = 0; i < *count; i++)
    {
      temp[i] = (*words)[i];
    }

    *words = temp;
    (*words)[*count] = s;
    (*count)++;

    s = strtok(NULL, delimiters);
  }
}
int main()
{
  char *str = (char *)malloc(sizeof(char) * size_str);
  char *delimiters = (char *)malloc(sizeof(char) * size_delim);
  char **words;
  int i, count = 0;

  scanf("%s", str);
  scanf("%s", delimiters);
  split(str, delimiters, &words, &count);

  for (i = 0; i < count; i++)
  {
    printf("\n%s", words[i]);
  }

  return 0;
}
