#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#define N 4

int main()
{
  int i, status = 0;
  printf("main parent process %d\n", getpid());

  for(i = 0; i < N; ++i)
  {
    pid_t pid = fork();

    if(pid == 0)
    {
      printf("%d %d\n", getpid(), getppid());
    }
    if(pid != 0)
    {
      pid_t st = wait(&status);
      break;
    }
  }
return 0;
}
