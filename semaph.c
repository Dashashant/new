#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define N 3
#define MAX 100

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
   semop(semid, &mybuf, 1);

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

    /*
     * FIXIT: Этот код не должен работать вот почему:
     * когда вы впервые создаёте семафор, в нем по умолчанию записывается значение 0.
     * Далее в каждой нити вы патаетесь выполнить операцию D, т.е. вычесть 1 из него, и обе нити "повиснут" на этой операции.
     * Т.е. вы забыли перед запуском нитей записать в семафор значение 1.
     * 
     * Чтобы не было путаницы, после завершения работы программы лучше семафор удалить (вызов semctl).
     */

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
