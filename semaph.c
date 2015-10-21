#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define N 3
#define MAX 100000

long int a[N] = {0, 1, 2};


void* my_func1(void* dummy)
{
 int i = 0; 
 int semid; 

 const char pathname[] = "hello.c"; 
 key_t key; 

 struct sembuf mybuf; 



 for(i = 0; i < MAX; i++)
 { 
   mybuf.sem_op = 1; // what do
   mybuf.sem_flg = 0;
   mybuf.sem_num = 0; //with wich

   /*
    * FIXIT:
    * Чтобы осуществлять операции A(S, n) и D(S, n) надо вызвать системный вызов semop, в который передать переменную типа (struct sembuf).
    * Пока семафоров фактически нет.
    */
   
   ++a[0];

   mybuf.sem_op = -1;
   mybuf.sem_flg = 0;
   mybuf.sem_num = 0;
 }

 return;
}

int main()
{
    pthread_t thread_id , thread_id1, my_thread_id;
    int result, result1;
    int semid; /* IPC-дескриптор для массива IPC семафоров */

    const char pathname[] = "hello.c"; /* Имя файла, использующееся для генерации ключа.
    Файл с таким именем должен существовать в текущей директории*/

    key_t key; /* IPC ключ */

    struct sembuf mybuf; /* Структура для задания операции над семафором */

    if ( (key = ftok(pathname , 0)) < 0 ) {
        printf("Can`t generate key\n");
        exit(-1);
    }

    /* Пытаемся получить доступ по ключу к массиву семафоров, если он существует,
       или создать его из одного семафора, если его ещё не существует, с правами
       доступа rw для всех пользователей */

    if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0) {
        printf("Can`t get semid\n");
        exit(-1);
    }


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

    return 0;
}
