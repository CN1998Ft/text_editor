/*** inlcudes ***/
#include "ctype.h"
#include "errno.h"
#include "stdio.h"
#include "termios.h"
#include "unistd.h"
#include <cstdlib>
#include <iostream>

/*** data ***/
// struct containing all terminal information
struct termios orig_termios;

/*** terminal ***/
void die(const char *s) {
  perror(s);
  exit(1);
}
void disableRawMode() {
  // Dsiable Raw mode, use the original terminal info stored.
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
    die("tcsetattr");
  }
}

void enableRawModel() {
  // get the parameter associated with the terminal referred to by fildes and
  // store them in the termios structure referenced by termios_p
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
    die("tcgetattr");
  }
  // at exit of the program, call function disableRawMode()
  atexit(disableRawMode);

  struct termios raw = orig_termios;
  // bit wise AND and assign with bitwise-NOT
  // Equavilent: raw.c_lflag = (raw.c_lflag) & (~(ECHO))
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
  raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
  raw.c_oflag &= ~(OPOST);
  // bit-wise OR
  raw.c_cflag |= ~(CS8);
  // minimal number of bytes of input needed before read() can return
  raw.c_cc[VMIN] = 0;
  // minial duration of time to wait before read() can return
  raw.c_cc[VTIME] = 1;

  // Set the parameters associated with the terminal referred to by the open
  // file descriptor, fildes, from the termios structure refrenced by termios_p
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    die("tcsetattr");
  }
}

/*** init ***/
int main() {
  enableRawModel();

  // char c;
  // keep running program until there is no more bytes to read and c is not 'q'
  // while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
  while (1) {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
      die("read");
    }
    if (iscntrl(c)) {
      // std::cout << (int)c << std::endl;
      // to enable "carriage return" and "new line" after diable OPOST
      std::cout << (int)c << "\r\n";
    } else {
      // std::cout << (int)c << "('" << c << "')" << std::endl;
      // to enable "carriage return" and "new line" after diable OPOST
      std::cout << (int)c << "('" << c << "')" << "\r\n";
    }
    if (c == 'q') {
      break;
    }
  }
  // }
  return 0;
}
