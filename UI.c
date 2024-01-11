#include <stdio.h>

void red()
{
  printf("\033[1;31m");
}

void yellow()
{
  printf("\033[1;33m");
}

void blue()
{
  printf("\033[1;34m");
}

void reset()
{
  printf("\033[0m");
}

void nav(char name[])
{
  printf("+-----------------------------------------------------------+\n");
  blue();
  printf("  CaroChess 1.0-beta");
  reset();
  yellow();
  printf("                                 %s\n", name);
  reset();
  printf("+-----------------------------+-----------------------------+\n");
}

void printXO(char XO)
{
  if (XO == '1')
  {
    red();
    printf("O");
    reset();
  }
  else if (XO == '2')
  {
    blue();
    printf("X");
    reset();
  }
  else
  {
    printf(" ");
  }
}

void playerBroad(char broad[])
{
  red();
  printf("            G-A-M-E           ");
  reset();
  printf("|");
  red();
  printf("             COORDINATES             \n");
  reset();
  printf("             |   |            |             |    |           \n");
  printf("          ");
  printXO(broad[0]);
  printf("  | ");
  printXO(broad[1]);
  printf(" |  ");
  printXO(broad[2]);
  printf("         |          \033[1;34ma1\033[0m | \033[1;34ma2\033[0m | \033[1;34ma3\033[0m        \n");
  printf("        -----|---|-----       |        -----|----|-----      \n");
  printf("          ");
  printXO(broad[3]);
  printf("  | ");
  printXO(broad[4]);
  printf(" |  ");
  printXO(broad[5]);
  printf("         |          \033[1;34mb1\033[0m | \033[1;34mb2\033[0m | \033[1;34mb3\033[0m        \n");
  printf("        -----|---|-----       |        -----|----|-----      \n");
  printf("          ");
  printXO(broad[6]);
  printf("  | ");
  printXO(broad[7]);
  printf(" |  ");
  printXO(broad[8]);
  printf("         |          \033[1;34mc1\033[0m | \033[1;34mc2\033[0m | \033[1;34mc3\033[0m        \n");
  printf("             |   |            |             |    |           \n");
}

void printXO2(int XO)
{
  if (XO == 1)
  {
    red();
    printf("O");
    reset();
  }
  else if (XO == -1)
  {
    blue();
    printf("X");
    reset();
  }
  else
  {
    printf(" ");
  }
}

void playerBroad2(const int broad[])
{
  red();
  printf("            G-A-M-E           ");
  reset();
  printf("|");
  red();
  printf("             COORDINATES               \n");
  reset();
  printf("             |   |            |             |    |           \n");
  printf("          ");
  printXO2(broad[0]);
  printf("  | ");
  printXO2(broad[1]);
  printf(" |  ");
  printXO2(broad[2]);
  printf("         |          \033[1;34ma1\033[0m | \033[1;34ma2\033[0m | \033[1;34ma3\033[0m        \n");
  printf("        -----|---|-----       |        -----|----|-----      \n");
  printf("          ");
  printXO2(broad[3]);
  printf("  | ");
  printXO2(broad[4]);
  printf(" |  ");
  printXO2(broad[5]);
  printf("         |          \033[1;34mb1\033[0m | \033[1;34mb2\033[0m | \033[1;34mb3\033[0m        \n");
  printf("        -----|---|-----       |        -----|----|-----      \n");
  printf("          ");
  printXO2(broad[6]);
  printf("  | ");
  printXO2(broad[7]);
  printf(" |  ");
  printXO2(broad[8]);
  printf("         |          \033[1;34mc1\033[0m | \033[1;34mc2\033[0m | \033[1;34mc3\033[0m        \n");
  printf("             |   |            |             |    |           \n");
}

void score(char name1[], char name2[])
{
  printf("+-----------------------------+-----------------------------+\n");
  red();
  printf(" Match: ");
  reset();
  blue();
  printf(" %s", name1);
  reset();
  yellow();
  printf(" vs");
  reset();
  blue();
  printf(" %s", name2);
  reset();
  printf(".                               \n");
  printf("+-----------------------------------------------------------+\n");
}

void ingame(char broadGame[])
{
  nav("");
  playerBroad(broadGame);
  // score();
}

void gameMenu(char name[])
{
  nav(name);
  printf("\n");
  printf("    [ ");
  red();
  printf("MAIN MENU");
  reset();
  printf(" ]\n\n");
  printf("    (\033[1;31m1\033[0m) Normal game\n");
  printf("    (\033[1;31m2\033[0m) Rank game\n");
  printf("    (\033[1;31m3\033[0m) VS Cute Bot\n");
  printf("    (\033[1;31m4\033[0m) Chanlenge\n\n");
  printf("    (\033[1;31m5\033[0m) Ranking\n");
  printf("    (\033[1;31m6\033[0m) Match log\n");

  printf("    (\033[1;31mq\033[0m) Quit\n");
}



void loginMenu()
{
  nav("");
  printf("\n");
  printf("    [ ");
  red();
  printf("USER MANAGEMENT");
  reset();
  printf(" ]\n\n");

  printf("    (\033[1;31m1\033[0m) Log in\n");
  printf("    (\033[1;31m2\033[0m) Sign in\n\n");
  printf("    (1-2, other to \033[1;34mquit\033[0m):\n");
}

void rankNav()
{
  printf("+-----------------------------------------------------------+\n");
  red();
  printf("RANK\t");
  reset();
  red();
  printf("PLAYER\t");
  reset();
  red();
  printf("WIN\t");
  reset();
  red();
  printf("LOSE\t");
  reset();
  red();
  printf("WIN/LOSE\n");
  reset();
  printf("+-----------------------------------------------------------+\n");
}

void logNav()
{
  printf("+-----------------------------------------------------------+\n");
  red();
  printf("TYPE\t");
  reset();
  red();
  printf("OPPONENT\t");
  reset();
  red();
  printf("RESULT\t");
  reset();
  red();
  printf("FIRST MOVE\t");
  reset();
  red();
  printf("LOG\n");
  reset();
  printf("+-----------------------------------------------------------+\n");
}

void waitNav()
{
  printf("+-----------------------------------------------------------+\n");
  red();
  printf("RANK\t");
  reset();
  red();
  printf("PLAYER\t");
  reset();
  red();
  printf("WIN\t");
  reset();
  red();
  printf("LOSE\t");
  reset();
  red();
  printf("WIN/LOSE\t");
  reset();
  red();
  printf("STATUS\n");
  reset();
  printf("+-----------------------------------------------------------+\n");
}