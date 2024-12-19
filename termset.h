#ifndef TERMSET_H
#define TERMSET_H

#include <termios.h>
#include <fcntl.h>

extern struct termios ttyold, ttynew;

int termset(int fd, int baudrate, struct termios *ttyold, struct termios *ttynew);

#endif

