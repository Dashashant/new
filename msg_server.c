#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

#define N 2

int main()
{
  /* IPC дескриптор для очереди сообщений */
  int msqid;
 
  /* Имя файла, использующееся для генерации ключа.
  Файл с таким именем должен существовать в текущей директории */
  char pathname[] = "hello.c";
  
  /* IPC ключ */
  key_t key;
 
  /* Реальная длина и максимальная длина информативной части сообщения */
  int len, maxlen;
  
  /* Ниже следует пользовательская структура для сообщения */
  struct mymsgbuf
  {
    long mtype;
    struct{
      int a;
      int b;
      int res;
      pid_t id;
    }info;
  } mybuf;

  int semid; /* IPC-дескриптор для массива IPC семафоров */
  const char pathname1[] = "time.c"; /* Имя файла, использующееся для генерации ключа*/
  key_t key1; /* IPC ключ */
  struct sembuf mybufsem; /* Структура для задания операции над семафором */

  if ( (key1 = ftok(pathname1 , 0)) < 0 ){
    printf("Can`t generate key\n");
    exit(-1);
  }

    /* Пытаемся получить доступ по ключу к массиву семафоров, если он существует,
       или создать его из одного семафора, если его ещё не существует, с правами
       доступа rw для всех пользователей */
  if ((semid = semget(key1 , 1 , 0666 | IPC_CREAT)) < 0) {
        printf("Can`t get semid\n");
        exit(-1);
  }

  /* Генерируем IPC ключ из имени файл*/
  if((key = ftok(pathname, 0)) < 0)
  {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  /* Пытаемся получить доступ по ключу к очереди сообщений, если она существует,
  или создать ее, если она еще не существует, с правами доступа
  read & write для всех пользователей */
  if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
  {
    printf("Can\'t get msqid\n");
    exit(-1);
  }
  
   mybufsem.sem_op = N;
   semop(semid, &mybufsem, 1);
   maxlen = sizeof(mybuf.info);

   while (1)
   {
     if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, +1, 0)) < 0)
     {
       printf("Can\'t receive message from queue\n");
       exit(-1);
     }

     printf("message type = %ld, info = %d\n, %d, id %d\n", mybuf.mtype, mybuf.info.a, mybuf.info.b, mybuf.info.id);
    pid_t pid = fork();

    if(pid == 0)
    {

      mybufsem.sem_op = -1;
      mybufsem.sem_flg = 0;
      mybufsem.sem_num = 0;

      if(semop(semid, &mybufsem, 1) < 0)
      {
        printf("Can't wait for condition\n");
      }

      mybuf.mtype = (int) mybuf.info.id;
      mybuf.info.res = mybuf.info.a * mybuf.info.b;
      sleep(10);
      len = sizeof(mybuf.info);

      if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0)
      {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
      }

      mybufsem.sem_op = 1;
      mybufsem.sem_flg = 0;
      mybufsem.sem_num = 0;

      if(semop(semid, &mybufsem, 1) < 0)
      {
        printf("Can't wait for condition\n");
      }
    }
  }
 return 0;
}
