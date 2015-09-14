#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void GenerateString(int n, char *string)
{
  int i = 0;
  string[0] = 'a';
  
  for(i = 1 ; i < n; i++)
  {
    string[(1 << i) - 1] = 'a' + i;
    memcpy(string+(1 << i), string, sizeof(char)*((1<<i)-1));

  }
}

int main()
{ 
 char* s = (char *)malloc((1<<2)*sizeof(char));
 
 GenerateString(4, s);
 printf("%s ", s);
 
 return 0;
}


