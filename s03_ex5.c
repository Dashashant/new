#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define NUM_CMD 100
#define LEN_CMD 100
#define NUM_ARG 100

typedef struct cmds
{
  char *cmd;
  char **argCmd;
  int arguments;
  int sleepTime;
};

FILE *fin;

void Split(char *string, char *delim, char ***tokens, int *Count);
void Get(struct cmds **comands, int num);

int main()
{
   int numCmd, i, status = 0;

   fin = fopen("file_for_exec.txt", "rt");

   struct cmds *comands =(struct cmds *)malloc(NUM_CMD * sizeof(struct cmds));
   char *tmp = (char *)malloc(LEN_CMD * sizeof(char));

   fgets(tmp, LEN_CMD, fin);
   numCmd = atoi(tmp);
   Get(&comands, numCmd);

   for (i = 0; i < numCmd; i++)
   {
     pid_t pid = fork();
     /*
      * По идее задачи должны запускаться в дочерних процессах. Вы сейчас запускаете новую задачу при (pid != 0), т.е. в родительском.
      * После запуска всех задач общий родительский процесс дожидается завершения работы всех дочерних.
      */
     if (pid != 0)
     {
       sleep(comands[i].sleepTime);
       execvp(comands[i].cmd, comands[i].argCmd);
       printf("%d", i);
       exit(i);
     }
     /*
      * Сейчас судя по вашему коду ребёнок должен кого-то дожидаться. Т.к. ему дожидаться некого, то никто никого ждать не будет.
      * Разберитесь и поправьте (буквально пару строк изменить надо).
      */
     for (i = 0; i < numCmd; i++)
     {
       pid = wait(&status);
     }
   }


   fclose(fin);

   for (i = 0; i < NUM_CMD; i++)
   {
     free(comands[i].argCmd);
   }

   free(comands);
   free(tmp);
   return 0;
 }
 
void Split(char *string, char *delim, char ***tokens, int *count)
{
   int i;
   char *pch = strtok(string, delim);
   char **tmp;

   tmp = NULL;

   while (pch != NULL)
   {
     tmp = (char**) realloc(tmp,(int) (sizeof(char*) * (*count + 1)));

     *tokens = tmp;
     (*tokens)[*count] = pch;
     (*count)++;
     pch = strtok (NULL, delim);
   }
}

void Get(struct cmds **comands, int num)
{
   char *delimiters = " \n";
   char **Args = (char **)malloc(NUM_ARG * sizeof(char *));

   int i, j;

   for (i = 0; i < num; i++)
   {
     char* tmp = (char *)malloc(LEN_CMD * sizeof(char));

     fgets(tmp, LEN_CMD, fin);
     Split(tmp, delimiters, &Args, &((*comands)[i].arguments));

     (*comands)[i].sleepTime = atoi(Args[0]);
     (*comands)[i].cmd = Args[1];

     (*comands)[i].argCmd = (char **)malloc(NUM_ARG * sizeof(char));
     (*comands)[i].arguments = (*comands)[i].arguments - 1;
     for (j = 0; j < (*comands)[i].arguments; j++)
     {
       ((*comands)[i].argCmd)[j] = Args[j + 1];
     }
   }
   free(Args);
}
