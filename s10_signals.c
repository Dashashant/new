#include <stdio.h>
#include <sys/types.h>
#include <sys/signal.h>

#define MAX_LEN 100

pid_t pid;
int current_bit = 0;
char s1[MAX_LEN];
char s2[MAX_LEN];

/*
 * Засчитано.
 */

void Send()
{
  char bit;
  int num = current_bit  / 8;
  int position = current_bit % 8; 

  bit = s1[num] & (1 << position);
  current_bit++;  

  /*
   * Можно использовать тернарный оператор:
   * kill(pid, bit ? SIGUSR2 : SIGUSR1);
   * вместо if-else в данном случае.
   */
  
  if(bit)
  {
    kill(pid, SIGUSR2);
  }else
  {
    kill(pid, SIGUSR1);
  }

  if (num == MAX_LEN)
    exit(0);
}

void Receive(int nsig)
{
  int num = current_bit / 8;
  int position = current_bit % 8;

  if (nsig == SIGUSR2)
  {
     s2[num] = s2[num] | (1 << position);
  }

  current_bit++;
  kill(pid, SIGINT);

  if (num == MAX_LEN)
  {
    printf("%s\n", s2);
  }
}

int main()
{
  int i;
  pid = getpid();

  for(i = 0; i < MAX_LEN; i++)
  { 
    s1[i] = 'a' + i % ('z' - 'a' + 1);
    s2[i] = 0;
  }
  
  signal(SIGUSR1,Receive);
  signal(SIGUSR2, Receive);
  signal(SIGINT, Send);

  pid_t pid1 = fork();

  if (pid1 != 0)
  {
    pid = pid1;
    Send();
  }

  while(1);
}
