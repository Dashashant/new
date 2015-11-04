#include <pthread.h>
 #include <stdio.h>
 #include <stdlib.h>
 #include <sys/types.h>
 #include <sys/ipc.h>
 #include <sys/sem.h>

 #define N 3
 #define MAX 10000

 long int a[N] = {0, 1, 2};
 
 /*
  * FIXIT: 
  * уже почти то, что надо.
  * Смотрите, у вас сейчас есть глобальная переменная a[0], которую вы инкрементируете из нескольких потоков.
  * Чтобы не было состояний гонки вы решили воспользоваться семафорами, и сделали это абсолютно верно ... за небольшим исключением.
  * Переменная mybuf - также глобальная, и при модификации её полей также будет возникать состояние гонки, т.е. надо вводить либо ещё один семафор для неё, либо ...
  */
 struct sembuf mybuf;
 int semid;

 void* my_func1(void* dummy)
 {
  int i = 0;

  for(i = 0; i < MAX; i++)
  {
    mybuf.sem_op = -1;
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;

    if(semop(semid, &mybuf, 1) < 0)
    {
       printf("Can't wait for condition\n");
    }

    a[0]++;

    mybuf.sem_op = 1;
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;

    if(semop(semid, &mybuf, 1) < 0)
    {
       printf("Can't wait for condition\n");
    }

  }
  return;
 }

 int main()
 {
     pthread_t thread_id , thread_id1, my_thread_id;
     int result, result1;
     const char pathname[] = "hello.c"; 
     key_t key; 
     a[0] = 0;

     if ( (key = ftok(pathname , 0)) < 0 ) {
         printf("Can`t generate key\n");
         exit(-1);
     }

     if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0) {
         printf("Can`t get semid\n");
         exit(-1);
     }

     mybuf.sem_op = 1;
     semop(semid, &mybuf, 1);

     result = pthread_create(&thread_id ,
                             (pthread_attr_t *)NULL ,
                             my_func1 ,
                             NULL);
     result1 = pthread_create(&thread_id1 ,
                             (pthread_attr_t *)NULL ,
                             my_func1 ,
                             NULL);
     if (result) {
         printf("Can`t create thread, returned value = %d\n" , result);
         exit(-1);
     }
     if (result1) {
         printf("Can`t create thread, returned value = %d\n" , result);
         exit(-1);
     }

     pthread_join(thread_id , (void **) NULL);
     pthread_join(thread_id1 , (void **) NULL);
     printf("%ld", a[0]);
     semctl(semid, IPC_RMID, (int) NULL);

     return 0;
 }





