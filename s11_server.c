#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * У файлов не было формата. Такое обычно не компилируется.
 * 
 * У меня сервер падает с ошибкой Address family not supported by protocol.
 * Давайте вы на семинаре продемонстрируете работу.
 */

#define MAX 5
int N;
int main()
{
  int sockfd;
  int clilen, n, i;
  char line[1000];
  struct sockaddr_in servaddr, cliaddr;
  struct sockaddr_in cliaddr_mas[MAX];

  N = 0;  
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(53000);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror(NULL);
    exit(1);
  } else
  {
    printf("Sock fd:%d\n", sockfd);
  }

  if (bind(sockfd, (struct sockaddr*) &servaddr,
    sizeof(servaddr)) < 0)
  {
    perror(NULL);
    close(sockfd);
    exit(1);
  }

  while (1)
  {
    clilen = sizeof(cliaddr);
    if ((n = recvfrom(sockfd, line, 999, 0,
      (struct sockaddr*)&cliaddr, &clilen)) < 0)
    {
      perror(NULL);
      close(sockfd);
      exit(1);
    }

    printf("%s %d\n", line, n);

    int flag = 0;

    for( i = 0; i < N; i++)
    {
      if ( cliaddr_mas[i].sin_port ==  cliaddr.sin_port && 
           cliaddr_mas[i].sin_addr.s_addr ==  cliaddr.sin_addr.s_addr)
      {
        flag = 1;
      }
    }

    if(flag == 0)
    {  
      cliaddr_mas[N].sin_port = cliaddr.sin_port;
      cliaddr_mas[N].sin_addr.s_addr = cliaddr.sin_addr.s_addr;
      N++;
    }
    flag = 0;  

    for( i = 0; i < N; i++)  
    {
      /*
       * Пользователь, отправивший сообщение, получит его тоже. Ок, пусть так.
       */
      if (sendto(sockfd, line, strlen(line), 0,
        (struct sockaddr*)&cliaddr_mas[i], clilen) < 0)
      {
        perror(NULL);
        close(sockfd);
        exit(1);
      }
    }
  }
  return 0;
}
