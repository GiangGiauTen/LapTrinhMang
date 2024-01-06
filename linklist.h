#ifndef __LINKLIST_H__
#define __LINKLIST_H__

typedef struct node
{
  char username[20];
  char password[20];
  int status;        // 0: offline, 1: online
  int win;
  int lose;
  int loginTimeLeft;
  int isLogin;
  struct node *next;
} node;

typedef struct rank
{
  char username[20];
  int win;
  int lose;
  float wlradio;
  struct rank *next;
} rank;

typedef struct loggame
{
  int type; // 0 rank, 1 normal
  char player1[20];  // winner
  char player2[20];  // losser
  int result; // 0: draw, 1: player1 go first, 2: player2 go first 
  char log[20];
  struct loggame *next;
} loggame;

extern loggame *loggamefirst;

extern rank *rankfirst;

extern node *first;
extern node *account;
extern int islogin;

loggame *initLogGameList();
rank *initRankList();
node *initLinkList();
node *getAccount(char username[]);

void addToLogGameList(char log[],int type,int result,char player1[],char player2[]);
void addToLinkList(char username[], char password[], int status, int win, int lose);
void printfAllUser();

int checkPassword(char username[], char password[]);
int checkAccountExist(char username[]);
void updateWinLose();
void printfRank();

#endif
