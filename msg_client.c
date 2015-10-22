/* Программа 1 для иллюстрации работы с очередями сообщений */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Тип сообщения для прекращения работы программы 2 */
#define LAST_MESSAGE 255


int main()
{
  /* IPC дескриптор для очереди сообщений */
  int msqid;

  /* Имя файла, использующееся для генерации ключа.
     Файл с таким именем должен существовать в текущей директории */
  char pathname[] = "hello.c";

  /* IPC ключ */
  key_t key;

  /* Cчетчик цикла и длина информативной части сообщения */
  int i, len;

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

  /* Генерируем IPC ключ из имени файла 09-1a.c в текущей директории
  и номера экземпляра очереди сообщений 0. */
  if ((key = ftok(pathname, 0)) < 0)
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

  /* Посылаем в цикле 5 сообщений с типом 1 в очередь сообщений, идентифицируемую msqid.*/
  //for (i = 1; i <= 5; i++)
  {
    /* Сначала заполняем структуру для нашего сообщения и определяем длину информативной части */
    mybuf.mtype = 1;
    mybuf.info.a = 3;
    mybuf.info.b = 1;
    mybuf.info.res = 0;
    mybuf.info.id = getpid();
    len = sizeof(mybuf.info);

    /* Отсылаем сообщение. В случае ошибки сообщаем об этом и удаляем очередь сообщений из системы. */
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0)
    {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
      exit(-1);
    }
  }

  /* Отсылаем сообщение, которое заставит получающий процесс прекратить работу, с типом LAST_MESSAGE и длиной 0 */
 /* mybuf.mtype = LAST_MESSAGE;
  len = 0;

  if (msgsnd(msqid, (struct msgbuf*)&mybuf, len, 0) < 0)
  {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
    exit(-1);
  }
*/

   int maxlen = 81;

    if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, +1, 0)) < 0)
    {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }


     printf("message type = %ld, res = %d\n", mybuf.mtype, mybuf.info.res);    


  return 0;
} 
