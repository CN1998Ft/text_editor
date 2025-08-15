/*** includes ***/
#include "errno.h"
#include "stdlib.h"
#include "termios.h"
#include "unistd.h"
#include <cctype>
#include <iostream>

/*** data ***/
struct termios orig_termios;

/*** terminal ***/
void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
    die("tcsetattr");
  }
}
// tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

void enableRawMode() {

  // Get the terminal attribute and put in struct raw(a reference)
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
    die("tcgetattr");
  }
  // call func at exit
  atexit(disableRawMode);

  struct termios raw = orig_termios;
  // flip ECHO to not ECHO
  // ICANON off, read byte-by-byte. On line-by-line.
  // ISIG disable ctrl-c ctrl-z
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  // IXON/IOFF (input) file, ctrl-s and ctrl-q
  // ICRNL carriage return new line off
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_cflag |= (CS8);

  // Turn off post processing of output
  raw.c_oflag &= ~(OPOST);

  raw.c_cc[VMIN] = 0;  // minimal time to read(), 0 = immediately
  raw.c_cc[VTIME] = 1; // maximum time before return read(), 1/10 of a sec

  // Set the terminal attribute using raw(reference)
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    die("tcsetattr");
  }
}

/*** init ***/
int main() {
  enableRawMode();

  while (1) {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) {
      die("read");
    }
    read(STDIN_FILENO, &c, 1);
    if (iscntrl(c)) {
      // std::cout << static_cast<int>(c) << std::endl;
      std::cout << static_cast<int>(c) << "\r\n";
    } else {
      // std::cout << static_cast<int>(c) << " ('" << c << "')" << std::endl;
      std::cout << static_cast<int>(c) << " ('" << c << "')" << "\r\n";
    }
    if (c == 'q') {
      break;
    }
  };

  return 0;
}
