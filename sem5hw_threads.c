#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4
#define MAX 100000000

/*
 * array - массив
 * Лучше такие большие массивы создавайте в куче malloc`ом, т.к. только чудом программа не падает из-за нехватки памяти по крайней мере у меня.
 */
long int mas[MAX];

struct Thread
{
  pthread_t id;
  int result;
};

struct Task
{
  int index;
  int a, b;
};

long int res[N]; 

void* summary(void* task)
{
 int i, sum = 0; 

 for(i = ((struct Task*)task)->a; i < ((struct Task*)task)->b; i++)
 { 
   sum += mas[i];
 }

 res[((struct Task*)task)->index]  = sum;
 return;
}
int main()
{
    int i;

    for(i = 0; i < MAX; i++)
      //mas[i] = rand() % 2;
      mas[i] = 1;
   
    struct Task tasks[N];
    struct Thread threads[N];
    
    /*
     * А что, если MAX не делится нацело на N?
     */
    for(i = 0; i < N; i++)
    {
      tasks[i].index = 0;
      tasks[i].a = i * MAX / N ;
      tasks[i].b = (i + 1) * MAX / N;
    }

    for(i = 0; i < N; i++)
    {
      threads[i].result = pthread_create(&(threads[i].id) ,
                            (pthread_attr_t *)NULL ,
                            summary ,
                            &tasks[i]);
      
      if (threads[i].result) {
        printf("Can`t create thread, returned value = %d\n" , threads[i].result);
        exit(-1);
      }
    }

    for(i = 0; i < N; i++)
    {
      pthread_join(threads[i].id , (void **) NULL);
    }
    
    /*
     * Тут тоже нужен цикл от 0 до N, суммирующий результат.
     * Да, у вас выдаётся неверный результат пока (в N раз меньше нужного).
     * 
     * Вам совсем немного осталось поправить. В задании требовалось найти среднее и дисперсию.
     */
    int result;
    printf("%ld", res[0] + res[1] + res[2] + res[3]);

    return 0;
}
