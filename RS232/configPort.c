#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define BAUDRATE B9600
#define SERIAL_PORT "/dev/ttyS0"

// Estructura para almacenar la configuración del terminal
static struct termios ttyold, ttynew;

// Configuración del puerto serie
int termset(int fd, int baudrate, struct termios *old_tty, struct termios *new_tty) {
    if (tcgetattr(fd, old_tty) != 0) {
        perror("Error obteniendo configuracion del puerto serie");
        return -1;
    }

    memset(new_tty, 0, sizeof(*new_tty));
    new_tty->c_cflag = baudrate | CS8 | CLOCAL | CREAD;
    new_tty->c_iflag = IGNPAR;
    new_tty->c_oflag = 0;
    new_tty->c_lflag = 0;
    new_tty->c_cc[VTIME] = 0;
    new_tty->c_cc[VMIN] = 1;

    if (tcsetattr(fd, TCSANOW, new_tty) != 0) {
        perror("Error configurando el puerto serie");
        return -1;
    }

    return 0;
}
