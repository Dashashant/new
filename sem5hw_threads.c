#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1
#define MAX 1000000 
long int * array;

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
    sum += array[i];
  }

  res[((struct Task*)task)->index] = sum;
  return;
}

int main()
{
  int i;
  array = (long int *)malloc(MAX * sizeof(long int));

  for(i = 0; i < MAX; i++)
    array[i] = 1;
    // array[i] = rand() % 2;

  struct Task tasks[N];
  struct Thread threads[N];

  for(i = 0; i < N; i++)
  {
    tasks[i].index = 0;
    tasks[i].a = i * MAX / N ;
    tasks[i].b = (i + 1) * MAX / N;
  }
  if (MAX % N != 0);
    tasks[N-1].b = MAX;

  for(i = 0; i < N; i++)
  {
    threads[i].result = pthread_create(&(threads[i].id) ,
                             (pthread_attr_t *)NULL ,
                             summary ,
                             &tasks[i]);

    if (threads[i].result) {
       printf("Can`t create thread, returned value = %d\n" ,
       threads[i].result);
       exit(-1);
    }
  }

  for(i = 0; i < N; i++)
  {
    pthread_join(threads[i].id , (void **) NULL);
  }

  long int result = 0;
  long int D = 0;

  for(i = 0; i < N; i++)
  {
    result += res[i];
  }

  /*
   * При вычислении среднего и дисперсии нужно делить на число элементов в массиве, а не на число нитей.
   * Дисперсию необходимо также посчитать с помощью N нитей.
   */
  for(i = 0; i < N; i++)
  {
    D += (array[i] - (float)result / MAX) * (array[i] - (float)result / MAX);
  }

  /*
   * Смотрите, пусть у вас значение элементов массива одинаковы и равны 1. Тогда среднее должно быть также равно 1, а дисперсия - 0.
   * Если вместо одной нити вы используте 2, 3, 4 ... нити, то результат (величина среднего и дисперсии) не должны измениться. Изменится только время, которое затратила программа на 
   * вычисления. Поэтому, если для разных N результат различный, то в вашем коде ошибка.
   * Вы перепутали индекс в одном месте. Выведите массив res, и посмотрите результат работы каждой из нитей.
   */
  
  printf("average %f, dispertion %f\n", (float)result / MAX, (float)D / MAX);
  return 0;
}
