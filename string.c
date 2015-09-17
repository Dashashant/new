#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define N 4

void GenerateString(int n, char *string)
{
  int i = 0;
  string[0] = 'a';
  
  for(i = 1 ; i < n; i++)
  {
    string[(1 << i) - 1] = 'a' + i;
    memcpy(string + (1 << i), string, sizeof(char) * ((1 << i) - 1));
  }
}

int main()
{ 
  char* s = (char *)malloc((1 << N) * sizeof(char));
  GenerateString(N, s);
  printf("%s ", s);
 
  return 0;
}


