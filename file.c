#include <stdio.h>
#include <stdlib.h>


#include "file.h"
#include "linklist.h"

node *first;
loggame *loggamefirst;

FILE *f;
char tlog[20];
char player1[20];
char player2[20];
int type;
int result;

char username[20];
char password[20];
int status, win, lose;
node *s1;
loggame *pre;

void readLogFile()
{
  f = fopen("log.txt", "rt");
  if (f == NULL)
  {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    pre = initLogGameList(pre, type, result, player1, player2, tlog);
    while (1)
    {
      if (fscanf(f, "%d %s %s %d %s", &type,player1, player2, &result, tlog) == EOF)
        break;
      pre = initLogGameList(pre, type, result, player1, player2, tlog);
    }
    fclose(f);
  }
}

void writeLogFile()
{
  loggame *temp = loggamefirst;
  remove("log.txt");
  f = fopen("log.txt", "w");
  temp = temp->next;

  while (temp)
  {
    fprintf(f, "%d %s %s %d %s\n", temp->type, temp->player1, temp->player2, temp-> result, temp->log);
    temp = temp->next;
  }
  fclose(f);
}

void readFile()
{
  f = fopen("account.txt", "rt");
  if (f == NULL)
  {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }
  else
  {
    s1 = initLinkList(s1, username, password, status, win, lose);
    while (1)
    {
      if (fscanf(f, "%s %s %d %d %d", username, password, &status, &win, &lose) == EOF)
        break;
      s1 = initLinkList(s1, username, password, status, win, lose);
    }
    fclose(f);
  }
}

void writeFile()
{
  node *temp = first;
  remove("account.txt");
  f = fopen("account.txt", "w");

  while (temp)
  {
    fprintf(f, "%s %s %d %d %d\n", temp->username, temp->password, temp->status, temp->win, temp->lose);
    temp = temp->next;
  }
  fclose(f);
}
