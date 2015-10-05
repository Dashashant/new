#include <stdio.h>
#include <stdlib.h>

#define N 3
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
long int desp_res[N];
long int result = 0;

void* summary(void* task)
{
  long int i, sum = 0;

  for(i = ((struct Task*)task)->a; i < ((struct Task*)task)->b; i++)
  {
    sum += array[i];
  }

  res[((struct Task*)task)->index] = sum;
  printf("index - %d, sum - %ld\n", ((struct Task*)task)->index, res[((struct Task*)task)->index]);
  return;
}
void* dispertion(void* task)
{
  long int i, sum = 0;

  for(i = ((struct Task*)task)->a; i < ((struct Task*)task)->b; i++)
  {
    sum += (array[i] - (float)result / MAX) * (array[i] - (float)result / MAX);
  }

  desp_res[((struct Task*)task)->index] = sum;
  return;
}

 int main()
 {
   int i;
   array = (long int *)malloc(MAX * sizeof(long int));

   for(i = 0; i < MAX; i++)
     array[i] = 1;

   struct Task tasks[N];
   struct Thread threads[N];

   for(i = 0; i < N; i++)
   {
     tasks[i].index = i;
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

   long int D = 0;

   for(i = 0; i < N; i++)
   {
     printf(" %d %d interval, %ld\n", tasks[i].a, tasks[i].b, res[i]);

     result += res[i];
   }

   for(i = 0; i < N; i++)
   {
     threads[i].result = pthread_create(&(threads[i].id) ,
                              (pthread_attr_t *)NULL ,
                              dispertion,
                              &tasks[i]);


     if (threads[i].result) {
        printf("Can`t create thread, returned value = %d\n" ,
        threads[i].result);
        exit(-1);
     }
   }
   for(i = 0; i < N; i++)
   {
     D += desp_res[i];
   }

   printf("average %f, dispertion %f\n", (float)result / MAX, (float)D / MAX);
   return 0;
 }
