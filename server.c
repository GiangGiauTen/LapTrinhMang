#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
#endif
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>  // for open
#include <unistd.h> // for close
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include "serverService.h"
#include "file.h"

#define MAX 80
#define SA struct sockaddr

// char log[30];
char currentGame[MAX][MAX];
char rankGame[MAX][MAX];
char userName[MAX][MAX];
int norQueue[MAX], rankQueue[MAX];   //-3: in game, -2: offline, -1: online, >0: in queue


int msg(int index,int sockfd, char ip[])
{
  char buffer[MAX], number[MAX], charactor[MAX], msg[MAX], password[MAX], win[MAX], lose[MAX];

  int n, i, j, sendlong = 0;

  // listen client
  bzero(buffer, MAX);
  bzero(number, MAX);
  bzero(charactor, MAX);
  bzero(msg, MAX);

  bzero(password, MAX);
  bzero(win, MAX);
  bzero(lose, MAX);

  if (recv(sockfd, buffer, sizeof(buffer), 0) != 0)
  {
    printf("From client: %s To client : ", buffer);
    if (buffer[0] == '1') //for login
    {
      for (i = 0; i < strlen(buffer); i++)
      {
        if (buffer[i + 1] == '~')
          break;
        userName[index][i] = buffer[i + 1];
      }

      for (j = 0; j < strlen(buffer); j++)
      {
        if (buffer[i] == '\n')
          break;
        password[j] = buffer[i + 2];
        i++;
      }
      password[j - 2] = '\0';
      strcat(msg, _signin(userName[index], password));
      msg[strlen(msg)] = '\0';
      printf("%s\n", msg);
      norQueue[index] = -1;
      rankQueue[index] = -1;
    }

    else if (buffer[0] == '2') //for logout
    {
      for (i = 0; i < strlen(buffer); i++)
        userName[index][i] = buffer[i + 1];

      userName[index][strlen(userName[index]) - 1] = '\0';
      strcat(msg, _signout(userName[index]));
    }

    else if (buffer[0] == '3') //for lockAccount
    {
      for (i = 0; i < strlen(buffer); i++)
        userName[index][i] = buffer[i + 1];

      userName[index][strlen(userName[index]) - 1] = '\0';
      strcat(msg, _lockAccount(userName[index]));
    }

    else if (buffer[0] == '4') // for signup
    {
      for (i = 0; i < strlen(buffer); i++)
      {
        if (buffer[i + 1] == '~')
          break;
        userName[index][i] = buffer[i + 1];
      }

      for (j = 0; j < strlen(buffer); j++)
      {
        if (buffer[i] == '\n')
          break;
        password[j] = buffer[i + 2];
        i++;
      }
      password[j - 2] = '\0';
      strcat(msg, _signup(userName[index], password));
      msg[strlen(msg)] = '\0';
      printf("%s\n", msg);
    }
    
    else if (buffer[0] == 'n') // for create normal queue
    {
      char temp[MAX];
      char ready[MAX];
      bzero(ready, MAX);
      if (buffer[1]=='~'){
          for (i = 0; i < strlen(buffer); i++)
          {
            if (buffer[i + 2] == '~')
              break;
            temp[i] = buffer[i + 2];
          }
            
        for (i = 0; i < MAX; i++ ){
          if ( i != index && strcmp(temp, userName[i]) == 0){
            if (norQueue[i] == index){  // start game
              norQueue[index] = i;
              strcat(msg,"s");
              printf("start game\n");
            } else {                     // waiting for accept
              norQueue[index] = i;
              strcat(msg,"w");
              printf("waiting for accept\n");
            }
          }
        }
        strcat(msg,"b");
      }
      else if (buffer[1]=='@'){
        norQueue[index] = -1;
      }
      else{
        char temp[MAX];
        
        int k = 0;
        for (int i = 0; i < MAX; i++ ){
          if ( i!= index && norQueue[i] >= -1){
            k++;
            sprintf(temp,"%d",k);
            strcat(ready,temp);
            strcat(ready,"\t");
            strcat(ready, userName[i]);
            bzero(temp,sizeof(temp));
            if (norQueue[i] == index){
              strcat(ready,"\t\033[1;31mWaiting for normal game\033[0m");
            }
            strcat(ready,"\n");

          }
        }
        sendlong =1;
        send(sockfd, ready, sizeof(ready), 0);
      }
    }

    else if (buffer[0] == '5') // for create normal game
    {
      char log[MAX];
      bzero(log, MAX);

     for (i = 0; i < strlen(buffer); i++)
        buffer[i] = buffer[i + 1];

      if (buffer[0] == '~')
        {
          int i;
          int s = sscanf (buffer, "~%[^~]~%[^~]~%d~%s", win, lose, &i, log);
          
          if (s != 4)   
            printf("error\n");
          else
            (msg,_saveLog(log,1, win,lose, i));
          printf("%s\n", msg);
      }
      else {
        

        if (norQueue[norQueue[index]]  == -1){           

          strcat(currentGame[index], ip);
          strcat(currentGame[index], "~");
          strcat(currentGame[index], buffer);
          printf("ip %s\n", currentGame[index]);
          strcat(msg, "h");
          strcat(msg, ip);
        } else {

          strcat(msg, currentGame[norQueue[index]]);
          bzero(currentGame[index], sizeof(currentGame[index]));  
          bzero(currentGame[norQueue[index]], sizeof(currentGame[norQueue[index]]));  
          norQueue[norQueue[index]] = -1;
          norQueue[index] = -1;
        }
      }
    }

    else if (buffer[0] == 'r') // for create rank queue
    {
      char temp[MAX];
      char ready[MAX];
      bzero(ready, MAX);
      if (buffer[1]=='~'){
          for (i = 0; i < strlen(buffer); i++)
          {
            if (buffer[i + 2] == '~')
              break;
            temp[i] = buffer[i + 2];
          }
            
        for (i = 0; i < MAX; i++ ){
          if ( i != index && strcmp(temp, userName[i]) == 0){
            if (rankQueue[i] == index){  // start game
              rankQueue[index] = i;
              strcat(msg,"s");
              printf("start game\n");
            } else {                     // waiting for accept
              rankQueue[index] = i;
              strcat(msg,"w");
              printf("waiting for accept\n");
            }
          }
        }
        strcat(msg,"b");
      }
      else if (buffer[1]=='@'){
        rankQueue[index] = -1;
      }
      else{
        char temp[MAX];
        
        int k = 0;
        for (int i = 0; i < MAX; i++ ){
          if ( i!= index && rankQueue[i] >= -1){
            k++;
            sprintf(temp,"%d",k);
            strcat(ready,temp);
            strcat(ready,"\t");
            strcat(ready, userName[i]);
            bzero(temp,sizeof(temp));
            if (rankQueue[i] == index){
              strcat(ready,"\t\033[1;31mWaiting for rank game\033[0m");
            }
            strcat(ready,"\n");
          }
        }
        sendlong =1;
        send(sockfd, ready, sizeof(ready), 0);
      }
    }

    else if (buffer[0] == '6') // for create rank game
    {
      char log[MAX];
      bzero(log, MAX);

      for (i = 0; i < strlen(buffer); i++)
        buffer[i] = buffer[i + 1];

      if (buffer[0] == '~')
      {
        int i;
        int s = sscanf (buffer, "~%[^~]~%[^~]~%d~%s", win, lose, &i, log);
        if (s != 4) {
          printf("error\n");
        };
        if (i != 0 && i != 3)  updateWinLose(win, lose);

        strcat(msg,_saveLog(log,0, win,lose, i));
             printf("%s\n", msg);
      }
      else
      { 
          if (rankQueue[rankQueue[index]]  == -1){          
            strcat(rankGame[index], ip);
            strcat(rankGame[index], "~");
            strcat(rankGame[index], buffer);
            printf("ip %s\n", rankGame[index]);
            strcat(msg, "h");
            strcat(msg, ip);
          } else {

            strcat(msg, rankGame[rankQueue[index]]);
            bzero(rankGame[index], sizeof(rankGame[index]));  
            bzero(rankGame[rankQueue[index]], sizeof(rankGame[rankQueue[index]]));  
            rankQueue[rankQueue[index]] = -1;
            rankQueue[index] = -1;
          }

      }
    }

    else if (buffer[0] == 'c'){         // for challenge
      if (buffer[1]=='~'){
        char temp[MAX];
        for (i = 0; i < strlen(buffer); i++)
        {
          if (buffer[i + 2] == '~')
            break;
          temp[i] = buffer[i + 2];
        }
            
        for (i = 0; i < MAX; i++ ){
          if ( strcmp(temp, userName[i]) == 0){
            if (norQueue[i] == -1 && rankQueue[i] == -1){  
              strcat(msg,"0"); // free
            } else {
              if (norQueue[i] == index) {
                norQueue[index] = i;
                strcat(msg,"1"); // start normal game
              }
              if (rankQueue[i] == index) {
                rankQueue[index] = i;
                strcat(msg,"2"); // start rank game 
              } 
              if (rankQueue[i] != index || norQueue[i] != index) strcat(msg,"3"); // busy
            }
          }
        }
      } else 
      {
        char temp[MAX];
        int k = 0;
        for (int i = 0; i < MAX; i++ ){
          if ( i!= index && rankQueue[i] >= -1){
            k++;
            sprintf(temp,"%d",k);
            strcat(msg,temp);
            strcat(msg,"\t");
            strcat(msg, userName[i]);
            bzero(temp,sizeof(temp));
            if (rankQueue[i] == index){
              strcat(msg,"\t\033[1;33mWaiting you for rank game\033[0m");
            }
            if (norQueue[i] == index){
              strcat(msg,"\t\033[1;33mWaiting you for normal game\033[0m");
            }
            if (rankQueue[i] != -1 && rankQueue[i] != index){
              strcat(msg,"\t\033[1;31mBusy\033[0m");
            }
            if (norQueue[i] != -1 && norQueue[i] != index){
              strcat(msg,"\t\033[1;31mBusy\033[0m");
            }
            if (rankQueue[i] == -1 && norQueue[i] == -1){
              strcat(msg,"\t\033[1;34mFree\033[0m");
            }
            strcat(msg,"\n");
          }
        }
      }
    }
    else if (buffer[0]=='x'){                         // reject challenge request
      for (i = 0; i < strlen(buffer); i++)
        buffer[i] = buffer[i + 1];
      
      for (i = 0; i < MAX; i++){
        if (strcmp(buffer, userName[i]) == 0){
          if (norQueue[i] == index) norQueue[i] = -1;
          if (rankQueue[i] == index) rankQueue[i] = -1;
        }
        send(i,"x",sizeof("x"),0);
        recv(i,buffer,sizeof(buffer),0);

        if (buffer[0] == 'a') strcat(msg,"Reject successfully");
      }
    }

    else if (buffer[0] == '7')        // for get rank
    {
      char rank[500];
      bzero(rank, 500);
      for (i = 0; i < strlen(buffer); i++)
        buffer[i] = buffer[i + 1];

      strcat(rank, _getRank(buffer));
      printf("\n%s ", rank);
      sendlong = 1;
      send(sockfd, rank, sizeof(rank), 0);
    }
    else if (buffer[0] == '8'){       // get log
      char log[500];
      bzero(log, 500);
      for (i = 0; i < strlen(buffer); i++)
        buffer[i] = buffer[i + 1];

      strcat(log, _getLog(buffer));
      printf("\n%s ", log);
      sendlong = 1;
      send(sockfd, log, sizeof(log), 0);
    }
    if (!sendlong)
      send(sockfd, msg, sizeof(msg), 0);
    sendlong = 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
    memset(msg, 0, sizeof(msg));
    return 1;
  }
  else
  {
    return 0;
  }
}

// Driver function
int main(int argc, char *argv[])
{
  int16_t PORT;
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, address;

  int opt = 1;
  int addrlen, new_socket, client_socket[30],
      max_clients = 30, activity, i, valread, sd;
  int max_sd;

  char buffer[50];

  for (int i = 0; i < MAX; i++){     
      // bzero(currentGame[i], MAX);
      // bzero(rankGame[i], MAX);  
      bzero(userName[i], MAX);
      norQueue[i] = -2;
      rankQueue[i] = -2;
  }



  //set of socket descriptors
  fd_set readfds;

  if (argc != 2)
  {
    PORT = 8080;
  }
  else
  {
    PORT = atoi(argv[1]);
  }

  //initialise all client_socket[] to 0 so not checked
  for (i = 0; i < max_clients; i++)
  {
    client_socket[i] = 0;
  }

  printf("listen port 127.0.0.1:%s \n", argv[1]);
  printf("Press Ctrl+C to close\n");

  // socket create and verification
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
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  addrlen = sizeof(servaddr);

  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
  {
    printf("socket bind failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully binded..\n");

  // Now server is ready to listen and verification
  if ((listen(sockfd, 3)) != 0)
  {
    printf("Listen failed...\n");
    exit(0);
  }
  else
    printf("Server listening..\n");


  readFile();
  printf("Read complete\n")  ;
  readLogFile();

  while (1)
  {
    //clear the socket set
    FD_ZERO(&readfds);

    //add master socket to set
    FD_SET(sockfd, &readfds);
    max_sd = sockfd;

    //add child sockets to set
    for (i = 0; i < max_clients; i++)
    {
      //socket descriptor
      sd = client_socket[i];

      //if valid socket descriptor then add to read list
      if (sd > 0)
        FD_SET(sd, &readfds);

      //highest file descriptor number, need it for the select function
      if (sd > max_sd)
        max_sd = sd;
    }

    //wait for an activity on one of the sockets , timeout is NULL ,
    //so wait indefinitely
    activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

    if ((activity < 0) && (errno != EINTR))
    {
      printf("select error");
    }

    //If something happened on the master socket ,
    //then its an incoming connection
    if (FD_ISSET(sockfd, &readfds))
    {
      if ((new_socket = accept(sockfd,
                               (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
      {
        perror("accept");
        exit(EXIT_FAILURE);
      }

      //inform user of socket number - used in send and receive commands
      printf("New connection , socket fd is %d , ip is : %s , port : %d \n ", new_socket, inet_ntoa(address.sin_addr), ntohs(address.sin_port));

      //add new socket to array of sockets
      for (i = 0; i < max_clients; i++)
      {
        //if position is empty
        if (client_socket[i] == 0)
        {
          client_socket[i] = new_socket;
          printf("Adding to list of sockets as %d\n", i);
          break;
        }
      }
    }

    //else its some IO operation on some other socket
    for (i = 0; i < max_clients; i++)
    {
      sd = client_socket[i];
      
      if (FD_ISSET(sd, &readfds))
      {
        if (msg(i, sd, inet_ntoa(address.sin_addr)) == 0)
        {
          _signout(userName[i]);
          bzero(userName[i], MAX);
          getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
          printf("\nHost disconnected , ip %s , port %d \n",
                 inet_ntoa(address.sin_addr), ntohs(address.sin_port));
          close(sd);
          client_socket[i] = 0;
        }
      }
    }
  }
  close(sockfd);
}
