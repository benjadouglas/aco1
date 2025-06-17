#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "EasyPIO.h"

// Function prototypes
void disp_binary(int);
void menu();
void autoFantastico();
void choque();
// void saltos();
// void carga_bateria();
struct termios modifyTerminalConfig(void);
void restoreTerminalConfig(struct termios);
bool keyHit(int index);
void pinSetup(void);
void ledShow(unsigned char output);
int delay(int index);
void clearTerminal();
void turnOff();

const unsigned char led[] = {14, 15, 18, 23, 24, 25, 8, 7};
int delayTime = 100000;

int main(void) {
  // pinSetup();
  menu();
}

void disp_binary(int i) {
  for (int t = 128; t > 0; t = t / 2) {
    if (i & t)
      printf("* ");
    else
      printf("_ ");
  }
  printf("                   ");
  fflush(stdout);
  printf("\r");
}

void menu() {
  int opcion;
  do {
    clearTerminal();
    printf("Seleccione una opcion:\n");
    printf("1: Auto Fantastico    \n");
    printf("2: El Choque          \n");
    printf("3: Saltos             \n");
    printf("4: Carga bateria      \n");
    printf("0: Salir              \n");
    scanf("%d", &opcion);

    switch (opcion) {
    case 1:
      autoFantastico();
      break;
    case 2:
      choque();
      break;
    // case 3:
    //   saltos();
    //   break;
    // case 4:
    //   carga_bateria();
    //   break;
    case 0:
      break;
    default:
      printf("Numero entre 0 y 4\n");
    }
  } while (opcion != 0);
}

void autoFantastico() {
  printf("Presione esc para finalizar la secuencia\n");
  printf("Presione flecha ARRIBA para aumentar la velocidad\n");
  printf("Presione flecha ABAJO para disminuir la velocidad\n");
  printf("Auto Fantastico:\n");

  unsigned char output;

  while (true) {
    output = 0x80;
    for (int i = 0; i < 8; i++) {
      // ledShow(output);
      disp_binary(output);
      output = output >> 1;
      if (delay(0) == 0) {
        // turnOff();
        return;
      }
    }
    output = 0x2;
    for (int i = 0; i < 6; i++) {
      // ledShow(output);
      disp_binary(output);
      output = output << 1;
      if (delay(0) == 0) {
        turnOff();
        return;
      }
    }
  }
}

void choque() {
  printf("Presione esc para finalizar la secuencia\n");
  printf("Presione flecha ARRIBA para aumentar la velocidad\n");
  printf("Presione flecha ABAJO para disminuir la velocidad\n");
  printf("Choque:\n");

  unsigned char output, aux1, aux2;

  while (true) {
    aux1 = 0x80;
    aux2 = 0x1;
    for (int i = 0; i < 7; i++) {
      output = aux1 | aux2;
      // ledShow(output);
      disp_binary(output);
      aux1 = aux1 >> 1;
      aux2 = aux2 << 1;

      if (delay(1) == 0) {
        turnOff();
        return;
      }
    }
  }
}

struct termios modifyTerminalConfig(void) {
  struct termios oldattr, newattr;
  tcgetattr(STDIN_FILENO, &oldattr);
  newattr = oldattr;
  newattr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
  return oldattr;
}

void restoreTerminalConfig(struct termios oldattr) {
  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
}

bool keyHit(int index) {
  struct termios oldattr = modifyTerminalConfig();
  int ch, oldf;
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  ch = getchar();
  if (ch == 117) { // ASCII para u
    if (delayTime > 1000) {
      delayTime = delayTime - 1000;
    }
  }

  if (ch == 100) { // ASCII para d
    delayTime = delayTime + 1000;
  }
  restoreTerminalConfig(oldattr);

  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if (ch == 27) {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}

void pinSetup(void) {
  pioInit();

  for (int i = 0; i < 8; i++) {
    pinMode(led[i], OUTPUT);
  }
}

void ledShow(unsigned char output) {
  for (int j = 0; j < 8; j++) {
    digitalWrite(led[j], (output >> j) & 1);
  }
}

int delay(int index) {
  int i;
  for (i = delayTime; i > 0; --i) {
    if (keyHit(index)) {
      return 0;
    }
  }
  return 1;
}

void clearTerminal() {
  printf("Presione ENTER\n");
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
  }
}

void turnOff() {
  unsigned char off = 0x0;
  ledShow(off);
}
