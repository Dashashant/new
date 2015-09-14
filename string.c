#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void GenerateString(int n, char *string)
{
  int i = 0;
  string[0] = 'a';
  /*
   * FIXIT: перед ; пробел не нужен
   */
  for(i = 1 ; i < n; i++)
  {
    string[(1 << i) - 1] = 'a' + i;
    /*
     * FIXIT: вокруг бинарных арифметических операторов надо ставить пробелы: x + y, а не x+y.
     */
    memcpy(string+(1 << i), string, sizeof(char)*((1<<i)-1));
  }
}

int main()
{ 
  /*
   * FIXIT:
   * Нужно либо с клавиатуры ввести номер необходимой строки, либо в самом начале файла определить константу N так
   * #define N 4
   * и далее уже в соответствии с этой константой выделять необходимое количество памяти под строку.
   * Ваша программа вполне может упасть с segmentation fault так-то.
   * Отступ делайте одинаковый во все программе. До этого был в 2 пробела, здесь почему-то в 1.
   */
 char* s = (char *)malloc((1<<2)*sizeof(char));
 
 GenerateString(4, s);
 printf("%s ", s);
 
 return 0;
}


