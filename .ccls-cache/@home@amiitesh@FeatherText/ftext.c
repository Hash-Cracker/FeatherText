#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disableRaw(){
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRaw(){
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRaw);


  struct termios raw = orig_termios;

  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_iflag &= ~(ICRNL | IXON | BRKINT | ISTRIP);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(){
  enableRaw();
  
  while(1){
    char c = '\0';
    read(STDIN_FILENO, &c, 1);
    if (iscntrl(c)){
      printf("%d\n", c);
    }
    else{
      printf("%d ('%c')\r\n", c, c);
    }
    if (c != 'q') break;
  }
  return 0;
}
