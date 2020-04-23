#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** createboard();
void print(char** board);
int isdraw(char** board);
char winningmove(char** board, int i, int j);

int main()
{
  char** board = createboard();
  char winner = '\0';
  char ch[5];
  char turn = 'X';

  while(!winner && !isdraw(board))    // in c, anything but 0 is true
  {
    print(board);

    // Read move
    printf("Player %c, make your move: ", turn);
    fflush(stdout);
    scanf(" %s", ch);

    if (strlen(ch) == 1 && ch[0] == 'R' && turn == 'X')
    {
      printf("X resigned. O wins\n");
      goto label;
    }
    else if (strlen(ch) == 1 && ch[0] == 'R' && turn == '0')
    {
      printf("O resigned. X wins\n");
      goto label;
    }


    if (strlen(ch) > 2 || ch[0] < 'A' || ch[0] > 'C' || ch[1] < '1' || ch[1] > '3')
    {
      printf("error. try again\n");
      continue;
    }



    // make move if square is free
    int row2 = ch[0] - 'A';
    int col2 = ch[1] - '1';

    if (board[row2][col2] == ' ')
    {
      board[row2][col2] = turn;
      if (turn == 'X') turn = '0';
      else turn = 'X';

      winner = winningmove(board, row2, col2);

    }
    else
      printf("Square occupied; try again.\n");
  }


  // game over- print board and declare finish
  print(board);
  if(winner == 'X' || winner == '0')
    printf("Congratulations %c!\n", winner);
  else
    printf("draw!!\n");


  label:
    for(int i = 0; i < 3; ++i)
      free(board[i]);
    free(board);

  return 0;

}

// Create the board with init to 0
char** createboard()
{
  char** b = calloc(3, sizeof(char*));  // calloc does malloc but initiate blocks to zero
  for (int i = 0; i < 3; ++i)
    b[i] = calloc(3, sizeof(char));

  for(int j = 0; j < 3; ++j)
    for(int k = 0; k < 3; ++k)
      b[j][k] = ' ';

  return b;
}

// return true if the game is a draw
int isdraw(char** board)
{
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      if (board[i][j] == ' ') return 0; // game aint over
    }
  return 1;
}

// return 'X' if (i,j) was a winning move for x
// return 'Y' if (i,j) was a winning move for y
// return ascii value 0 otherwise
char winningmove(char** board, int i, int j)
{
  if (board[i][j] == board[i][(j+1)%3] && board[i][j] == board[i][(j+2)%3])
    return board[i][j]; // complete a column

  else if (board[i][j] == board[(i+1)%3][j] && board[i][j] == board[(i+2)%3][j])
    return board[i][j]; // complete a row

  else if (i == j && board[i][j] == board[(i+1)%3][(j+1)%3] && board[i][j] == board[(i+2)%3][(j+2)%3])
    return board[i][j]; // completed forward diagonal

  else if (i+j == 2 && board[i][j] == board[(i+2)%3][(j+1)%3] && board[i][j] == board[(i+1)%3][(j+2)%3])
    return board[i][j]; // completed a reverse diagonal

  else
    return 0; // not over
}

void print(char** board)
{
  printf(" |1|2|3|\n");
  for (int i = 0; i < 3; ++i)
  {
    printf("%c|", 'A' + i);
    for (int j = 0; j < 3; ++j)
      printf("%c|", board[i][j]);
    printf("\n");
  }
}
