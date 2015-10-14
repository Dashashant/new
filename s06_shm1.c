#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#define M 100000

/*
 * Пойдёт, засчитано. Не используйте магические константы.
 * #define MAX_STRING_SIZE 11
 * 
 * Ещё надо с семафорами сделать программу.
 */

int main()
{
   int new = 1;
   int shmid; /* IPC дескриптор для области разделяемой памяти */
   const char pathname[] = "in.c"; /* Имя файла, используемое для генерации ключа.

    Файл с таким именем должен существовать в текущей директории*/
   int fin = open("in.c", O_RDONLY);
   char *s = (char *)malloc(sizeof(char)*M);
   int bytesRead = read( fin, s, 100);
   printf("bytesread - %d\n", bytesRead);
   key_t key; /* IPC ключ */

   if((key = ftok(pathname , 0)) < 0){ /* Генерируем ключ */
     printf("Can\'t generate key\n");
     exit(-1);
   }

   /* Пытаемся создать разделяемую память для сгенерированного ключа,
    т.е. если для этого ключа она уже существует, системный вызов вернет
    отрицательное значение. Размер памяти определяем как размер массива из трех целых
    переменных, права доступа 0666 - чтение и запись разрешены для всех */
    
    /*Выделение памяти для 11 символов, так как для большего количества память не выделяется. 
    Срабатывает проверка выделения памяти, и выводится Can't find shared memory. Не знаю, с чем это связано. 
    Файл in.c находится в той же директории и для памяти меньше 11*sizeof(char) всё работает.   */
    
    if((shmid = shmget(key, 11*sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0){
        /*В случае ошибки пытаемся определить возникла ли она из-за того, что
        сегмент разделяемой памяти уже существует или по другой причине */
        if(errno != EEXIST) {
            /* По другой причине...*/
            printf("Can't create shared memory\n");
            exit(-1);
        } else {
        /* Если из-за того, что разделяемая память уже существует, то пытаемся получить
            её IPC дескриптор и, в случае удачи, сбрасываем флаг необходимости
            инициализации элементов массива
        */
         if((shmid = shmget(key, 11*sizeof(char), 0)) < 0){
            printf("Can't find shared memory\n");
            exit(-1);
         }
         new = 0;
        }
    }
    char *shm;
    if(( shm = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)){
      printf("Can't attach shared memory\n");
      exit(-1);
   }
   
   int i;
   for(i = 0; i < 11; i++) // Память не больше чем для 11 символов. Их и передаем.
     shm[i] = s[i];
   printf("%s", shm);

}
