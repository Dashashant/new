#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LEN 1000
int main(int argc, char** argv)
{
  int sockfd;
  int n, len;
  char sendline[MAX_LEN], recvline[MAX_LEN];
  struct sockaddr_in servaddr, cliaddr;

  if (argc != 2)
  {
    printf("Usage: ./a.out 127.0.0.1 \n");
    exit(1);
  }
  if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror(NULL);
    exit(1);
  }

  bzero(&cliaddr, sizeof(cliaddr));
  cliaddr.sin_family = AF_INET;
  cliaddr.sin_port = htons(0);
  cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0)
  {
    perror(NULL);
    close(sockfd);
    exit(1);
  }
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(53000);
  if (inet_aton(argv[1], &servaddr.sin_addr) == 0)
  {
    printf("Invalid IP address\n");
    close(sockfd);
    exit(1);
  }
  printf("String -> ");

  printf("Your name:\n");
  fgets(sendline, MAX_LEN, stdin);
  sendline[strlen(sendline) - 1 ] = '\0';

  /*
    Вы отправляете имя, но никак его не используете. 
    Ок, пусть пока так будет.
  */
  
  if (sendto(sockfd, sendline, strlen(sendline) + 1, 0, 
      (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
  {
    perror(NULL);
    close(sockfd);
    exit(1);
  }

  pid_t pid = fork();

  if(pid != 0)
  {
    /*
     * У вас пользователь сможет только 1 раз отправить сообщение
     */
    fgets(sendline, MAX_LEN, stdin);

    if (sendto(sockfd, sendline, strlen(sendline) + 1, 0, 
       (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
      perror(NULL);
      close(sockfd);
      exit(1);
    }
  }else
  {
    while(1)
    {
      if((n = recvfrom(sockfd, recvline, MAX_LEN, 0, (struct sockaddr*) NULL, NULL)) < 0)
      {
        perror(NULL);
        close(sockfd);
        exit(0);
      }
      printf("%s\n", recvline);
    }
  }

  close(sockfd);
  return 0;
}
