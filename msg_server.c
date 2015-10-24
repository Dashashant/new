#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

/*
  FIXIT: Удалите из кода всё лишнее.
 */

/* Тип сообщения для прекращения работы */
#define LAST_MESSAGE 255
#define N 2
int fact(int x)
{
  int i, sum = 1;

  for(i = 1; i <= x; i++)
  { 
    sum = sum*i;
  }
  return sum;
}


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

    const char pathname1[] = "time.c"; /* Имя файла, использующееся для генерации ключа.
    Файл с таким именем должен существовать в текущей директории*/

    key_t key1; /* IPC ключ */

    struct sembuf mybufsem; /* Структура для задания операции над семафором */

    if ( (key1 = ftok(pathname1 , 0)) < 0 ) {
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

  /* Генерируем IPC ключ из имени файла 09-1a.c в текущей директории
  и номера экземпляра очереди сообщений 0 */

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

   while (1)
  {
    /* В бесконечном цикле принимаем сообщения любого типа в порядке FIFO с
     * максимальной длиной информативной части 81 символ до тех пор, пока не
     * поступит сообщение с типом LAST_MESSAGE */

    /*
     * FIXIT: надо 81 заменить на sizeof нужно структуры.
     */
    maxlen = 81;

    /*
     * FIXIT: сервер должен принимать только сообщения типа 1, а не все подряд.
     * Иначе есть шанс прочитать своё же сообщение, отправленное клиенту.
     */
    if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, 0, 0)) < 0)
    {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }

    /* Если принятое сообщение имеет тип LAST_MESSAGE, прекращаем работу и
     * удаляем очередь сообщений из системы.
       В противном случае печатаем текст принятого сообщения. */



    printf("message type = %ld, info = %d\n, %d, id %d\n", mybuf.mtype, mybuf.info.a, mybuf.info.b, mybuf.info.id);

    /*
     * FIXIT: значением N надо инициализировать семафор до начала получения сообщений (вне бесконечного цикла).
     * Инициализация произойдёт только тогда, когда вызовите системный вызов semop(...).
     * 
     * Надо сначала записать в поля переменной mybufsem нужные значения, а потом вызвать semop.
     * 
     * У вас та же проблема с инициализацией семафора в программе semaph.c
     */
    semop(semid, &mybufsem, 1);
    mybufsem.sem_op = N;
    
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


      mybuf.mtype = 1;
      mybuf.info.res = mybuf.info.a * mybuf.info.b;
      sleep(5);
      len = sizeof(mybuf.info);

      if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0)
      {
        printf("Can\'t send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
        exit(-1);
      }
      /*
       * FIXIT:
       * У вас дочерний процесс не возвращает значение семафора в исходное состояние после завершения.
       */
        exit(0);

      mybufsem.sem_op = 1;
      mybufsem.sem_flg = 0;
      mybufsem.sem_num = 0;

      if(semop(semid, &mybufsem, 1) < 0)
      {
        printf("Can't wait for condition\n");
      }


    } 
  }

 /* mybuf.mtype = 2;
  mybuf.info.sinfo = fact(mybuf.info.sinfo);
  mybuf.info.finfo = 1;
  len = sizeof(mybuf.info);


  if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0)
  {
     printf("Can\'t send message to queue\n");
     msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
     exit(-1);
  }*/

  return 0;
} 
