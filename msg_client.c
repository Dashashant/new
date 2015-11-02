#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
   /* IPC дескриптор для очереди сообщений */
   int msqid;

   /* Имя файла, использующееся для генерации ключа.
      Файл с таким именем должен существовать в текущей директории */
   char pathname[] = "hello.c";

   /* IPC ключ */
   key_t key, len;
   /* Cчетчик цикла и длина информативной части сообщения */
   int i;
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

   /* Генерируем IPC ключ из имени файла*/
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

/* Сначала заполняем структуру для нашего сообщения и определяем длину информативной части */
     mybuf.mtype = 1;
     mybuf.info.a = 3;
     mybuf.info.b = 1;
     mybuf.info.res = 0;
     mybuf.info.id = getpid();
     int maxlen = sizeof(mybuf.info);

     /* Отсылаем сообщение. В случае ошибки сообщаем об этом и удаляем очередь сообщений из системы. */
     if(msgsnd(msqid, (struct msgbuf *) &mybuf, maxlen, 0) < 0)
     {
       printf("Can\'t send message to queue\n");
       msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
       exit(-1);
     }

     if ((len = msgrcv(msqid, (struct msgbuf *)&mybuf, maxlen, +((int)getpid()), 0)) < 0)
     {
       printf("Can\'t receive message from queue\n");
       exit(-1);
     }
     printf("message type = %ld, res = %d\n", mybuf.mtype, mybuf.info.res);

 return 0;
 }





