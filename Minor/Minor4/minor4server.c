// Name.....Yafet Kubrom
// EUID.....11334602
// Course...csce3600.001
// ...Minor4 server.....

// compile: gcc minor4server.c -o minor4server
// Usage: ./minor4server port

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>

char** createboard();
void print(char** board);
int draw(char** board);
char winninghand(char** board, int i, int j);
void sendboard(char** board, int newsock);
int check(int newsock, char ch[]);


void error(const char* msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  char** board = createboard();
  char wrong[] = "Not Your turn\n\n";
  char right[] = "Your turn\n\n";
  int sockfd, newsock1, newsock2, port, n;
  int nread, nready, maxfd, checker;
  fd_set fds;
  char message[] = "?- Display this help\nR- Resign\n Move should be <R><C> with no space\n Example: A1 or A2\n";
  int kin = strlen(message);
  char str[1024];
  char msg[3000];
  char final[kin+1];
  int clilen1, clilen2;
  //char message[] = "Can you see this message?  ";
  struct sockaddr_in serv_addr, cli_addr1, cli_addr2;

  // check if right format is entered
  if (argc != 2)
  {
    printf("format: ./minor4server port\n");
    exit(1);
  }

  // create a socket for server
  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    error("socket");

  // zero out garbage numbers from previos program
  bzero((char *) &serv_addr, sizeof(serv_addr));
  port = atoi(argv[1]);  // convert port string to int

  // configure socket before bind
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port); // convert to network byte order

  int on = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  // bind a name to socket (reserve a port)
  if (bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("bind");

  // listen for connection
  listen(sockfd, 3);


    printf("Network Server Starting..\n");
    printf("Waiting on clients\n");

    // accept a connection and return new file descriptor
    clilen1 = sizeof(cli_addr1);
    if ((newsock1 = accept(sockfd, (struct sockaddr *)&cli_addr1, &clilen1)) == -1)
      error("accept");

    printf("X<Connected\n");
    printf("X>You are player X. Waiting on player O to connect\n");
    send(newsock1, "X>You are player X. Waiting on player O to connect\n", 50,0);

    clilen2 = sizeof(cli_addr2);
    if ((newsock2 = accept(sockfd, (struct sockaddr *)&cli_addr2, &clilen2)) == -1)
      error("accept");

    printf("O<Connected\n");
    printf("O>You are player O\n");
    send(newsock2, "O>You are player O\n", 19, 0);

    bzero(str, 1024);   // clean out garbage values in buffer
    maxfd = (newsock1 > newsock2 ? newsock1 : newsock2) + 1;
    char turn = 'X';
    char winner = '\0';
    char ch[5];

    while(!winner && !draw(board))  //
    {
      // send both clients the current board
      sendboard(board, newsock1);
      sendboard(board, newsock2);
      print(board);
      if (turn == 'X')
      {
        send(newsock1, "X>Your turn\n", 13, 0);
        printf("X>Your turn\n\n");
      }

      else
      {
        send(newsock2, "O> Your turn\n", 13, 0);
        printf("O> Your turn\n\n");
      }


      // set up polling using select
      FD_ZERO(&fds);
      FD_SET(newsock1, &fds);
      FD_SET(newsock2, &fds);

      bzero(msg, 3000);

      // once both connect and are added to fd set
      nready = select(maxfd, &fds, (fd_set *)0, (fd_set *)0, (struct timeval *) 0);


      // if either descriptor has some input
      if (FD_ISSET(newsock1, &fds))
      {
        if (turn == 'X')
        {
          bzero(ch, 5);
          nread = recv(newsock1, ch, 2, 0);
          if (nread < 1)
          {
            close(newsock1);
            close(newsock2);
            exit(0);
          }
          if ((checker = check(newsock1, ch)) > 0)
          {
            if (checker == 1)
            {
              send(newsock2, "X> You win, X resigned\n", 24, 0);
              printf("X> O win, X resigned\n");
              goto label;
            }
            else if (checker == 3)
            {
              //bzero(msg, 3000);
              send(newsock1, message, kin, 0);
              continue;
            }
            else
            {
              strcpy(msg, "X>Invalid command\n");
              strcat(msg, message);
              send(newsock1, msg, strlen(message)+19, 0);
              printf("X> Invalid command\n");
              continue;
            }
          }

        }
        else
        {
          nread = recv(newsock1, str, sizeof(str), 0);
          send(newsock1, wrong, sizeof(wrong), 0);
          printf("X> Not your turn\n\n");
          continue;
        }
      }

      if (FD_ISSET(newsock2, &fds))
      {
        if (turn == 'O')
        {
          bzero(ch, 5);
          nread = recv(newsock2, ch, 2, 0);
          if (nread < 1)
          {
            close(newsock1);
            close(newsock2);
            exit(0);
          }
          if ((checker = check(newsock2, ch)) > 0)
          {
            if (checker == 1)
            {
              send(newsock1, "X>You win, O resigned\n", 24, 0);
              printf("X win. O resign\n");
              goto label;
            }
            else if (checker == 3)
            {
              send(newsock2, message, kin, 0);
              continue;
            }
            else
            {
              strcpy(msg, "O>Invalid command\n");
              strcat(msg, message);
              send(newsock2, msg, strlen(message)+19, 0);
              printf("O>Invalid Command\n\n");
              continue;
            }
          }

        }
        else
        {
          nread = recv(newsock2, str, sizeof(str), 0);
          send(newsock2, wrong, sizeof(wrong), 0);
          printf("O> Not your turn\n\n");
          continue;
        }
      }

      int row2 = ch[0] - 'A';
      int col2 = ch[1] - '1';

      if (board[row2][col2] == ' ')
      {
        board[row2][col2] = turn;
        if (turn == 'X') turn = 'O';
        else turn = 'X';

        winner = winninghand(board, row2, col2);
      }
      else if (turn == 'X')
        send(newsock1, "That spot is already taken\n", 30, 0);
      else
        send(newsock2, "That spot is already taken\n", 30, 0);
    }

    sendboard(board, newsock1);
    sendboard(board, newsock2);
    print(board);

    if (winner == 'X')
    {
      send(newsock1, "You won\n", 10, 0);
      send(newsock2, "You lose\n", 10, 0);
      printf("X wins\n");
    }
    else if (winner == 'O')
    {
      send(newsock2, "You won\n", 10, 0);
      send(newsock1, "You lose\n", 10, 0);
      printf("O wins\n");
    }
    else
    {
      send(newsock2, "You tie\n", 10, 0);
      send(newsock1, "You tie\n", 10, 0);
      printf("Its a tie\n");
    }



label:
    for(int i = 0; i < 3; ++i)
      free(board[i]);
    free(board);

    close(newsock1);
    close(newsock2);
    close(sockfd);
  return 0;

}

char** createboard()
{
  char** b = calloc(3, sizeof(char*));  // calloc does malloc but initiate blocks to 0
  for (int i = 0; i < 3; ++i)
    b[i] = calloc(3, sizeof(char));

  for (int j = 0; j < 3; ++j)
    for (int k = 0; k < 3; ++k)
      b[j][k] = ' ';

  return b;
}

// return true if game is draw
int draw(char** board)
{
  for(int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
    {
      if (board[i][j] == ' ') return 0;   // game aint over
    }
  return 1;
}

// return X or 0 if they win.
// return ascii value 0 otherwise
char winninghand(char** board, int i, int j)
{
  if (board[i][j] == board[i][(j+1)%3] && board[i][j] == board[i][(j+2)%3])
    return board[i][j];   // complete a column

  else if (board[i][j] == board[(i+1)%3][j] && board[i][j] == board[(i+2)%3][j])
    return board[i][j];   // complete a row

  else if (i == j && board[i][j] == board[(i+1)%3][(j+1)%3] && board[i][j] == board[(i+2)%3][(j+2)%3])
    return board[i][j];   // completed forward diagonal

  else if (i + j == 2 && board[i][j] == board[(i+2)%3][(j+1)%3] && board[i][j] == board[(i+1)%3][(j+2)%3])
    return board[i][j];   // completed backward diagonal

  else
    return 0; // not over
}

void print(char** board)
{
  printf(" |1|2|3\n");
  for (int i = 0; i < 3; ++i)
  {
    printf("%c|", 'A' + i);
    for (int j = 0; j < 3; ++j)
      printf("%c|", board[i][j]);
    printf("\n");
  }
}

void sendboard(char** board, int newsock)
{
  char buf[1024];
  bzero(buf, sizeof(buf));
  send(newsock, " |1|2|3\n", 8, 0);
  for (int i = 0; i < 3; ++i)
  {
    sprintf(buf, "%c|", 'A' + i);
    send(newsock, buf, sizeof(buf), 0);
    bzero(buf, sizeof(buf));
    for (int j = 0; j < 3; ++j)
    {
      sprintf(buf, "%c|", board[i][j]);
      send(newsock, buf, sizeof(buf), 0);
    }
    send(newsock, "\n", 2, 0);
    bzero(buf, sizeof(buf));
  }
}

int check(int newsock, char ch[])//char ch1, char ch2
{
  if (ch[0] == 'R' && ch[1] != 0)
    return 1;
  else if (ch[0] == '?')
    return 3;
  else if (strlen(ch) > 2 || ch[0] < 'A' || ch[0] > 'C' || ch[1] < '1' || ch[1] > '3')
    return 2;

  else
    return -2;
}
