#include <stdio.h>
#ifdef _WIN32
#include <winsock2.h>
#endif
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <fcntl.h>  // for open
#include <unistd.h> // for close
#include <netdb.h>
#include <string.h>
#include "UI.h"
#include "clientP2P.h"
#include "clientPeer1.h"
#include "clientPeer2.h"

#define clear() printf("\033[H\033[J")

#define MAX 80
#define SA struct sockaddr

int loginTime = 3;
char name[20] = "";

void login(int sockfd);
void signup(int sockfd);
void logout(int sockfd);
void normalQueue(int sockfd);
void normalgame(int sockfd, char name[]);
void rankQueue(int sockfd, char name[]);
void rankgame(int sockfd, char name[]);
void challenge(int sockfd);
void showRank(int sockfd, char name[]);
void showLog(int sockfd, char name[]);
void gameScreen(int sockfd);
int win(const int board[9]);
int minimax(int board[9], int player);
void computerMove(int board[9]) ;
void playerMove(int board[9]);
void botgame(char name[]);

void login(int sockfd)
{
  char buff[MAX], username[MAX];
  int n;
  bzero(buff, sizeof(buff));
  if (loginTime != 4)
  {
    printf("Enter username : ");
    strcat(buff, "1");
    n = 1;
    while ((buff[n++] = getchar()) != '\n')
      ;

    strcpy(username, buff);
    buff[n - 1] = '~';
    printf("Enter password : ");
    while ((buff[n++] = getchar()) != '\n');

    send(sockfd, buff, sizeof(buff), 0);
    bzero(buff, sizeof(buff));

    recv(sockfd, buff, sizeof(buff), 0);

    if (loginTime < 4)
      if (strcmp(buff, "Login successfully") != 0)
        loginTime--;
      else
      {
        strcpy(name, username);
        for (int i = 0; i <= strlen(name); i++)
          name[i] = name[i + 1];
        name[strlen(name) - 1] = '\0';
        loginTime = 4;
      }

    if (loginTime == 0)
    {
      username[0] = '3';
      send(sockfd, username, sizeof(username), 0);
      bzero(buff, sizeof(buff));
      recv(sockfd, buff, sizeof(buff), 0);
    }
    printf("From Server : %s\n", buff);
  }
  else
  {
    printf("You already login !\n");
  }
}

void signup(int sockfd)
{
  char buff[MAX], username[MAX];
  int n;
  bzero(buff, sizeof(buff));

  printf("Enter username : ");
  strcat(buff, "4"); //set for logout
  n = 1;
  while ((buff[n++] = getchar()) != '\n')
    ;

  strcpy(username, buff);
  buff[n - 1] = '~';
  printf("Enter password : ");
  while ((buff[n++] = getchar()) != '\n')
    ;

  send(sockfd, buff, sizeof(buff), 0);
  bzero(buff, sizeof(buff));

  recv(sockfd, buff, sizeof(buff), 0);

  if (strcmp(buff, "Signup successfully") == 0)
  {
    strcpy(name, username);
    for (int i = 0; i <= strlen(name); i++)
      name[i] = name[i + 1];
    name[strlen(name) - 1] = '\0';
    loginTime = 4;
  }

  printf("From Server : %s\n", buff);
}

void logout(int sockfd)
{
  char buff[MAX];
  int n;
  bzero(buff, sizeof(buff));
  printf("Enter username : ");
  strcat(buff, "2");
  n = 1;
  while ((buff[n++] = getchar()) != '\n')
    ;

  send(sockfd, buff, sizeof(buff), 0);
  bzero(buff, sizeof(buff));
  recv(sockfd, buff, sizeof(buff), 0);
  if (strcmp(buff, "Goodbye") == 0)
    loginTime = 3;
  printf("From Server : %s\n", buff);
}

void normalQueue(int sockfd)
{
  char buff[MAX], temp[MAX];
  for(;;)
  {
    bzero(buff, sizeof(buff));
    strcat(buff, "n");
    send(sockfd, buff, sizeof(buff), 0);
    bzero(buff, sizeof(buff));
    recv(sockfd, buff, sizeof(buff), 0);
    printf("Already player : \n%s\n\n", buff);
    printf("You need to refresh before choose your opponent\n(1) Start game\n(2) Refresh\n(3) Back to menu\n");

    char d;
    scanf("%c", &d);

    if (d == '1')
    {
      printf("Enter player's ID to start game\n");
      bzero(buff, sizeof(buff));
      bzero(temp, sizeof(temp));
      
      strcat(buff, "n~");
    
      int n = 0;

      getchar();

      while ((temp[n++] = getchar()) != '\n');
      
      temp[n - 1] = '\0';
      strcat(buff, temp);
      strcat(buff, "~");

      send(sockfd, buff, sizeof(buff), 0);
      bzero(buff, sizeof(buff));
      recv(sockfd, buff, sizeof(buff), 0);

      if ( buff[0] == 'w'){
        printf("Waiting for player to accept\n");
        normalgame(sockfd, name);
        break;
      } else if (buff[0] == 's'){
        normalgame(sockfd, name);
        break;
      } else {
        printf("Player is busy\n");
      }
    }
    if (d == '2')
    {
      clear();
      continue;
    }
    if (d == '3')
    {
      send(sockfd, "n@", sizeof("n@"), 0);
      break;
    }
  }
}

void normalgame(int sockfd, char name[])
{
  int a, iPort;
  char buff[MAX], *ip_port, *ip, *cPort;
  bzero(&buff, sizeof(buff));

  ip_port = (char *)calloc(80, sizeof(char));
  ip = (char *)calloc(20, sizeof(char));
  cPort = (char *)calloc(10, sizeof(char));

  strcpy(ip_port, genPort());
  strcat(buff, "5");
  strcat(buff, ip_port);

  send(sockfd, buff, sizeof(buff), 0);

  bzero(buff, sizeof(buff));

  recv(sockfd, buff, sizeof(buff), 0);
  if (buff[0] == 'h')
  {
    for (int i = 0; i < strlen(buff); i++)
      buff[i] = buff[i + 1];
    strcpy(ip, buff);
    iPort = atoi(ip_port);
    startP2P(ip, iPort, 1, name, sockfd);
  }
  else
  {
    strcat(ip, return_ip(buff));
    strcat(cPort, return_port(buff));
    iPort = atoi(cPort);
    connectP2P(ip, iPort, 1, name, sockfd);
  }
}

void rankQueue(int sockfd, char name[])
{
  char buff[MAX], temp[MAX];
  for(;;)
  {
    bzero(buff, sizeof(buff));
    strcat(buff, "r");
    send(sockfd, buff, sizeof(buff), 0);
    bzero(buff, sizeof(buff));
    recv(sockfd, buff, sizeof(buff), 0);
    printf("Already player : \n%s\n\n", buff);
    printf("You need to refresh before choose your opponent\n(1) Start game\n(2) Refresh\n(3) Back to menu\n");

    char d;
    scanf("%c", &d);

    if (d == '1')
    {
      printf("Enter player's ID to start game\n");
      bzero(buff, sizeof(buff));
      bzero(temp, sizeof(temp));
      
      strcat(buff, "r~");
    
      int n = 0;

      getchar();

      while ((temp[n++] = getchar()) != '\n');
      
      temp[n - 1] = '\0';
      strcat(buff, temp);
      strcat(buff, "~");

      send(sockfd, buff, sizeof(buff), 0);
      bzero(buff, sizeof(buff));
      recv(sockfd, buff, sizeof(buff), 0);

      if ( buff[0] == 'w'){
        printf("Waiting for player to accept\n");
        rankgame(sockfd, name);
        break;
      } else if (buff[0] == 's'){
        rankgame(sockfd, name);
        break;
      } else {
        printf("Player is busy\n");
      }
    }
    if (d == '2')
    {
      clear();
      continue;
    }
    if (d == '3')
    {
      send(sockfd, "r@", sizeof("r@"), 0);
      break;
    }
  }
}

void rankgame(int sockfd, char name[])
{
  int a, iPort;
  char buff[MAX], *ip_port, *ip, *cPort;
  bzero(&buff, sizeof(buff));

  ip_port = (char *)calloc(80, sizeof(char));
  ip = (char *)calloc(20, sizeof(char));
  cPort = (char *)calloc(10, sizeof(char));

  strcpy(ip_port, genPort());
  strcat(buff, "6");
  strcat(buff, ip_port);

  send(sockfd, buff, sizeof(buff), 0);

  bzero(buff, sizeof(buff));

  recv(sockfd, buff, sizeof(buff), 0);
  if (buff[0] == 'h')
  {
    for (int i = 0; i < strlen(buff); i++)
      buff[i] = buff[i + 1];
    strcpy(ip, buff);
    iPort = atoi(ip_port);
    startP2P(ip, iPort, 2, name, sockfd);
  }
  else
  {
    strcat(ip, return_ip(buff));
    strcat(cPort, return_port(buff));
    iPort = atoi(cPort);
    connectP2P(ip, iPort, 2, name, sockfd);
  }
}

void challenge(int sockfd){
  char buff[MAX], temp[MAX];
  for(;;)
  {
    bzero(buff, sizeof(buff));
    strcat(buff, "c");
    send(sockfd, buff, sizeof(buff), 0);
    bzero(buff, sizeof(buff));
    recv(sockfd, buff, sizeof(buff), 0);
    printf("Already player : \n%s\n\n", buff);
    printf("You need to refresh before choose your opponent\n(1) Start game\n(2) Refresh\n(3) Back to menu\n");

    char d;
    scanf("%c", &d);

    if (d == '1')
    {
      printf("Enter player's ID to challenge\n");
      bzero(buff, sizeof(buff));
      bzero(temp, sizeof(temp));
      
      int n = 0;

      getchar();

      while ((temp[n++] = getchar()) != '\n');
      
      temp[n - 1] = '\0';

      strcat(buff, "c~"); 

      strcat(buff, temp);
      strcat(buff, "~");


      send(sockfd, buff, sizeof(buff), 0);
      bzero(buff, sizeof(buff));
      recv(sockfd, buff, sizeof(buff), 0);

        if ( buff[0] == '0'){
          char e;
          printf("Do you wany to challenge this player with rank or normal game?\n(1) Rank game\n(2) Normal game\n(other) Back\n");
          scanf("%c", &e);
          if (e == '1'){
            bzero(buff, sizeof(buff));
            strcat(buff, "r~");
            strcat(buff, temp);
            strcat(buff, "~");

            send(sockfd, buff, sizeof(buff), 0);
            bzero(buff, sizeof(buff));
            recv(sockfd, buff, sizeof(buff), 0);

            if (buff[0] == 'w'){
              printf("Waiting for player to accept\n");
              rankgame(sockfd, name);
            }
            else if (buff[0] == 's'){
              rankgame(sockfd, name);
            }
            else continue;

          }
          else if (e == '2'){
            bzero(buff, sizeof(buff));
            strcat(buff, "n~");
            strcat(buff, temp);
            strcat(buff, "~");

            send(sockfd, buff, sizeof(buff), 0);
            bzero(buff, sizeof(buff));
            recv(sockfd, buff, sizeof(buff), 0);
            
             if (buff[0] == 'w'){
              printf("Waiting for player to accept\n");
              normalgame(sockfd, name);
            }
            else if (buff[0] == 's'){
              normalgame(sockfd, name);
            }
            else continue;
          }
          else continue;
          printf("Waiting for player to accept\n");

          break;
        } else if (buff[0] == '2'){
          printf("You've accepted a rank game\n");
          rankgame(sockfd, name);
          break;
        } else if (buff[0] == '1'){
          printf("You've accepted a normal game\n");
          normalgame(sockfd, name); 
          break;
        } else { 
          printf("Player is busy\n");
          continue; 
        }
    }
    if (d == '2')
    {
      clear();
      continue;
    }
    if (d == '3')
    {
      send(sockfd, "r@", sizeof("r@"), 0);
      break;
    }
  }
}

void showRank(int sockfd, char name[])
{
  int a;
  char buff[MAX], rank[500];
  bzero(buff, sizeof(buff));
  strcat(buff, "7");
  strcat(buff, name);

  send(sockfd, buff, sizeof(buff), 0);

  bzero(buff, sizeof(buff));

  recv(sockfd, rank, sizeof(rank), 0);
  rankNav();
  printf("%s", rank);
  getchar();
}

void showLog(int sockfd, char name[])
{
  int a;
  char buff[MAX], log[500];
  bzero(buff, sizeof(buff));
  strcat(buff, "8");
  strcat(buff, name);

  send(sockfd, buff, sizeof(buff), 0);

  bzero(buff, sizeof(buff));

  recv(sockfd, log, sizeof(log), 0);
  logNav();
  printf("%s", log);
  getchar();
}

void gameScreen(int sockfd)
{
  char d;
  while (1)
  {
    clear();
    gameMenu(name);
    scanf("%c", &d);

    if (d == '1')
    {
      scanf("%*c");
      normalQueue(sockfd);
    }
    if (d == '2')
    {
      scanf("%*c");
      rankQueue(sockfd, name);
    }
    if (d == '3')
    {
      scanf("%*c");
      botgame(name);
    }
    if (d == '5')
    {
      scanf("%*c");
      showRank(sockfd, name);
    }
    if (d == '4')
    {
      scanf("%*c");
      challenge(sockfd);
    }
    if (d == '6')
    {
      scanf("%*c");
      clear();
      showLog(sockfd, name);
    }
    if (d == 'q')
    {
      scanf("%*c");
      logout(sockfd);
      break;
    }
  }
}

int win(const int board[9]) {
    //determines if a player has won, returns 0 otherwise.
    unsigned wins[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    int i;
    for(i = 0; i < 8; ++i) {
        if(board[wins[i][0]] != 0 &&
           board[wins[i][0]] == board[wins[i][1]] &&
           board[wins[i][0]] == board[wins[i][2]])
            return board[wins[i][2]];
    }
    return 0;
}

int minimax(int board[9], int player) {
    //How is the position like for player (their turn) on board?
    int winner = win(board);
    if(winner != 0) return winner*player;
    
    int move = -1;
    int score = -2;//Losing moves are preferred to no move
    int i;
    for(i = 0; i < 9; ++i) {//For all moves,
        if(board[i] == 0) {//If legal,
            board[i] = player;//Try the move
            int thisScore = -minimax(board, player*-1);
            if(thisScore > score) {
                score = thisScore;
                move = i;
            }//Pick the one that's worst for the opponent
            board[i] = 0;//Reset board after try
        }
    }
    if(move == -1) return 0;
    return score;
}

void computerMove(int board[9]) {
    int move = -1;
    int score = -2;
    int i;
    for(i = 0; i < 9; ++i) {
        if(board[i] == 0) {
            board[i] = 1;
            int tempScore = -minimax(board, -1);
            board[i] = 0;
            if(tempScore > score) {
                score = tempScore;
                move = i;
            }
        }
    }
    //returns a score based on minimax tree at a given node.
    board[move] = 1;
}
int checkmove( char location[])
{
  if (strcmp(location, "a1\n") == 0)
    return 0;
  if (strcmp(location, "a2\n") == 0)
    return 1;
  if (strcmp(location, "a3\n") == 0)
    return 2;
  if (strcmp(location, "b1\n") == 0)
    return 3;
  if (strcmp(location, "b2\n") == 0)
    return 4;
  if (strcmp(location, "b3\n") == 0)
    return 5;
  if (strcmp(location, "c1\n") == 0)
    return 6;
  if (strcmp(location, "c2\n") == 0)
    return 7;
  if (strcmp(location, "c3\n") == 0)
    return 8;

  return -1;
}
void playerMove(int board[9]) {
    int move = 0;
    do {
        printf("\nInput move: ");
        
        char buff[MAX];
        bzero(buff, MAX);

        int n = 0;
        while ((buff[n++] = getchar()) != '\n')
          ;
        move = checkmove(buff);
    } while (move >= 9 || move < 0 && board[move] == 0);
    board[move] = -1;
}

void botgame(char name[])
{
   int board[9] = {0,0,0,0,0,0,0,0,0};
    //computer squares are 1, player squares are -1.
    printf("Computer: O, You: X\nPlay (1)st or (2)nd? ");
    int player=0;
    scanf("%d",&player);
    printf("\n");
    unsigned turn;
    for(turn = 0; turn < 9 && win(board) == 0; ++turn) {
        if((turn+player) % 2 == 0)
            computerMove(board);
        else {
            clear();
            score(name, "Cute Bot");
            playerBroad2(board);
            playerMove(board);
        }
    }
    switch(win(board)) {
        case 0:
            printf("A draw. How droll.\n");
            getchar();
            break;
        case 1:
            clear();
            score(name, "Cute Bot");
            playerBroad2(board);
            printf("You lose.\n");
            getchar();
            break;
        case -1:
            printf("You win. Inconceivable!\n");
            getchar();
            break;
    }
}




int main(int argc, char *argv[])
{
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;
  char ipAddress[15];
  unsigned short PORT;

  if (argc < 2 || argc > 3)
  {
    printf("hay nhap chuoi theo cu phap \n ./client 3asd15 \n ");
    return 0;
  }

  strcpy(ipAddress, argv[1]);
  PORT = atoi(argv[2]);

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
  servaddr.sin_addr.s_addr = inet_addr(ipAddress);
  servaddr.sin_port = htons(PORT);

  // connect the client socket to server socket
  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
  {
    printf("connection with the server failed...\n");
    exit(0);
  }
  else
    printf("connected to the server..\n");

  char c, d;
  int check = 0;

  while (1)
  {
    if (loginTime >= -1)
    {
      if (loginTime == 4)
        gameScreen(sockfd);

      clear();
      loginMenu();
      scanf("%c", &c);

      if (c == '1')
      {
        scanf("%*c");
        login(sockfd);
      }
      if (c == '2')
      {
        scanf("%*c");
        signup(sockfd);
      }
      if (c != '2' && c != '1')
        break;
    }
  }
  // close the socket
  close(sockfd);
}
