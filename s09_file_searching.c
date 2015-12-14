#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN_DIR 100
#define MAX_FILE_NAME 100

/*
 * Хорошо. Засчитано. Потом кратко устно прокомментируете, как ф-я Find работает.
 */

int Find(char *current_dir, int depth, char *file);

int Find(char *current_dir, int depth, char *file)
{
   DIR *dir;
   struct dirent *dp = (struct dirent *)malloc(sizeof(struct dirent));

   if((dir = opendir(current_dir)) == NULL)
   {
     printf("error\n");
     exit(1);
   }

   while((dp = readdir(dir)) != NULL)
   {
     if( 0 == strcmp(file, dp->d_name))
     {
       printf("File found in directory; %s\n", current_dir);
       exit(0);
     }

     if((dp->d_type == DT_DIR) && (depth > 0))
     {
       char *tmp = (char *)malloc(MAX_LEN_DIR * sizeof(char));
       strcpy(tmp, current_dir);
       strcat(tmp, "/");
       strcat(tmp, dp->d_name);
       Find(tmp, depth - 1, file);
     }   
    }
  return 1;
}
int main(int argc, char* argv[])
{
  char *directory = (char *)malloc(MAX_LEN_DIR * sizeof(char));
  int depth;

  char *file = (char *) malloc(MAX_FILE_NAME * sizeof(char));
  directory = argv[1];
  depth = atoi(argv[2]);
  file = argv[3];

  int z = Find(directory, depth, file);
  if(z == 1)
    printf("No file\n");
  return 0;
}
