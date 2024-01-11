#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#ifdef _WIN32
#include <winsock2.h>
#endif
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>  // for open
#include <unistd.h> // for close
#include <sys/types.h>

#include "UI.h"
#include "clientP2P.h"

#define MAX 80
#define SA struct sockaddr
#define clear() printf("\033[H\033[J")



void joinPerson(int sockfd, int typeOfGame, char name[], int connectserver)
{
  strcpy(pointBroad, "000000000");
  char buff[MAX], msg[MAX], competitorName[MAX], log[MAX], temp[MAX];
  int n;

  bzero(log, MAX);
  bzero(buff, MAX);
  bzero(competitorName, MAX);

  
    strcpy(buff, name);
    recv(sockfd, competitorName, sizeof(competitorName), 0);
    send(sockfd, buff, sizeof(buff), 0);
  

  for (;;)
  {
    clear();
    ingame(pointBroad);
    if (typeOfGame == 2)
      score(name, competitorName);
    do {
      printf("It's your turn, enter the position where you want to place your mark. \nType 'q' to forfeit the game. \n ");

    
      bzero(buff, MAX);

      n = 0;
      while ((buff[n++] = getchar()) != '\n')
        ;
      if (strcmp(buff, "q\n") == 0)
      {
      close(sockfd);
      break;
      }

      if (isPositionExits(buff))
        printf("Invalid move! That spot is already taken. Try again.\n");
      else if (!isValidMove(buff))
        printf("Invalid move. The selected cell is not available. Make another choice.\n");
    } while(isPositionExits(buff) || !isValidMove(buff));

    
    strcpy(pointBroad, updateBroad(pointBroad, buff, '2'));
    clear();
    ingame(pointBroad);
    if (typeOfGame == 2)
      score(name, competitorName);

    strcpy(temp,buff);
    temp[2] = '\0';
    strcat(log, temp);
    bzero(temp, MAX);
    
    send(sockfd, buff, sizeof(buff), 0);

    if (checkDraw()) {
      printf("It's a draw! The game is a tie.");
      getchar();
      close(sockfd);
      break;
    }

    if (checkWinner(pointBroad, '2'))
    {
      printf("You won !!!");
      if (typeOfGame == 1)
      {
        bzero(msg, MAX);
        strcat(msg, "5~");
        strcat(msg, name);
        strcat(msg, "~");
        strcat(msg, competitorName);
        strcat(msg, "~");
        strcat(msg, "1");
        strcat(msg, "~");
        strcat(msg, log);

        send(connectserver, msg, sizeof(msg), 0);
        recv(connectserver, msg, sizeof(msg), 0);

        printf("%s", msg);
        bzero(msg, MAX);
        printf("%s", msg);
      }
      if (typeOfGame == 2)
      {
        bzero(msg, MAX);
        strcat(msg, "6~");
        strcat(msg, name);
        strcat(msg, "~");
        strcat(msg, competitorName);
        strcat(msg, "~");
        strcat(msg, "1");
        strcat(msg, "~");
        strcat(msg, log);

        send(connectserver, msg, sizeof(msg), 0);
        recv(connectserver, msg, sizeof(msg), 0);
        bzero(msg, MAX);
        printf("%s", msg);
      }
      getchar();
      close(sockfd);
      break;
    }
    
    bzero(buff, sizeof(buff));
    printf("Waiting for opponent ...\n");
    if (recv(sockfd, buff, sizeof(buff), 0) == 0)
    {
      if (typeOfGame == 1)
      {
        bzero(msg, MAX);
        strcat(msg, "5~");
        strcat(msg, name);
        strcat(msg, "~");
        strcat(msg, competitorName);
        strcat(msg, "~");
        strcat(msg, "1");
        strcat(msg, "~");
        strcat(msg, log);

        send(connectserver, msg, sizeof(msg), 0);
        recv(connectserver, msg, sizeof(msg), 0);

        printf("%s", msg);
        bzero(msg, MAX);
        printf("%s", msg);
      }
      if (typeOfGame == 2)
      {
        bzero(msg, MAX);
        strcat(msg, "6~");
        strcat(msg, name);
        strcat(msg, "~");
        strcat(msg, competitorName);
         strcat(msg, "~");
        strcat(msg, "1");
        strcat(msg, "~");
        strcat(msg, log);


        send(connectserver, msg, sizeof(msg), 0);
        recv(connectserver, msg, sizeof(msg), 0);

        bzero(msg, MAX);
        printf("%s", msg);
      }
      printf("\n Your opponent has forfeited the game. You won !!!");
      getchar();
      close(sockfd);
      break;
    }
    
    strcpy(temp,buff);
    temp[2] = '\0';
    strcat(log, temp);
    bzero(temp, MAX);
    strcpy(pointBroad, updateBroad(pointBroad, buff, '1'));

    if (checkWinner(pointBroad, '1'))
    {
      printf("You lose !!!");
      getchar();
      close(sockfd);
      break;
    }
    if (checkDraw()) {
      if (typeOfGame == 1)
      {
        bzero(msg, MAX);
        strcat(msg, "5~");
        strcat(msg, name);
        strcat(msg, "~");
        strcat(msg, competitorName);
        strcat(msg, "~");
        strcat(msg, "0");
        strcat(msg, "~");
        strcat(msg, log);

        send(connectserver, msg, sizeof(msg), 0);
        recv(connectserver, msg, sizeof(msg), 0);

        printf("%s", msg);
        bzero(msg, MAX);
        printf("%s", msg);
      }
      if (typeOfGame == 2)
      {
        bzero(msg, MAX);
        strcat(msg, "6~");
        strcat(msg, name);
        strcat(msg, "~");
        strcat(msg, competitorName);
        strcat(msg, "~");
        strcat(msg, "0");
        strcat(msg, "~");
        strcat(msg, log);

        send(connectserver, msg, sizeof(msg), 0);
        recv(connectserver, msg, sizeof(msg), 0);

        bzero(msg, MAX);
        printf("%s", msg);
      }
      printf("It's a draw! The game is a tie.");
      getchar();
      close(sockfd);
      break;
    }
  }
}

void connectP2P(char ip[], int PORT, int typeOfGame, char name[], int connectserver)
{
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;

  // socket create and varification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    printf("socket creation failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(ip);
  servaddr.sin_port = htons(PORT);

  // while (1)
  // {
  /* code */
  printf("ip port %s:%d___\n", ip, PORT);
  // }

  // connect the client socket to server socket
  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
  {
    printf("connection with the server failed...\n");
    exit(0);
  }
  else
    printf("connected to the server..\n");

  // Function for chatting between client and server
  // typeOfGame ==1 -> nomarl game
  // typeOfGame ==2 -> rank game
  joinPerson(sockfd, typeOfGame, name, connectserver);

  close(sockfd);
}
