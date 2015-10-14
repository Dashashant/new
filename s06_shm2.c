#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
int main()
{
  
   int shmid; /* IPC дескриптор для области разделяемой памяти */
   int new = 1 /* Флаг необходимости инициализации элементов массива */;
   const char pathname[] = "in.c"; /* Имя файла, используемое для генерации ключа.

    Файл с таким именем должен существовать в текущей директории*/

   key_t key; /* IPC ключ */

   if((key = ftok(pathname , 0)) < 0){ /* Генерируем ключ */
     printf("Can\'t generate key\n");
     exit(-1);
   }

   /* Пытаемся создать разделяемую память для сгенерированного ключа,
    т.е. если для этого ключа она уже существует, системный вызов вернет
    отрицательное значение. Размер памяти определяем как размер массива из трех целых
    переменных, права доступа 0666 - чтение и запись разрешены для всех */
    
    // объяснение про число 11 в s06_shm1.c
    if((shmid = shmget(key, 11 * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0){
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
         if((shmid = shmget(key, 11 * sizeof(char), 0)) < 0){
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
  printf("%s", shm);
}
