#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    /*
     * FIXIT:
     * Вы используете в итоге слишком много памяти, т.к. не чистите её за собой совсем.
     * Это надо делать.
     * Выделить память + скопировать прежние данные делает команда realloc.
     */
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
  /*
   * FIXIT:
   * Ваш код похож на переделанный начальный вариант программы Никиты. Если я ошибаюсь, то поправьте меня. Самое страшное, что он теперь ещё и неверно работает.
   * Вы помните, что уличение в списывании ведёт к удвоению количества домашних упражнений.
   * Поэтому в этот раз считаем, что вы сами написали неработащий код: не получается, например, разбить строку "a a", когда разделитель - пробел.
   * 
   * При компиляции выводились предупреждения из-за того, что не подключили stdlib.h для malloc`а.
   * 
   * В целом, код хорошо написан.
   */

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
