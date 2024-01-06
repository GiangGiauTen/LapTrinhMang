#ifndef __HOME_H__
#define __HOME_H__

char *_signin(char username[20], char password[20]);
char *_signout(char username[20]);
char *_signup(char username[20], char password[20]);
char *_lockAccount(char username[20]);
char *_getRank(char name[]);
char *_getLog(char name[]);
char *_saveLog( char log[], int type, char player1[], char player2[], int result);

#endif
