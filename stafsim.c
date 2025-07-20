#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct Star {
  double x, y, dx, dy;
  int speed;
};

void reset_terminal() {
  printf("\033[0m");
  printf("\033[?25h");
  fflush(stdout);
}

void init_terminal() {
  printf("\033[2J");
  printf("\033[?25l");
  fflush(stdout);
  atexit(reset_terminal);
}

void clearstar(int x, int y) {
  if (x < 1 || y < 1)
    return;
  printf("\033[%d;%dH ", y, x);
  fflush(stdout);
}

void newstar(int x, int y) {
  if (x < 1 || y < 1)
    return;
  printf("\033[%d;%dH*", y, x);
  fflush(stdout);
}

int main() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  int center_x = w.ws_col / 2;
  int center_y = w.ws_row / 2;
  const int max_stars = 100;
  struct Star stars[max_stars];

  for (int i = 0; i < max_stars; i++) {
    stars[i].x = ((double)rand() / RAND_MAX) * w.ws_col;
    stars[i].y = ((double)rand() / RAND_MAX) * w.ws_row;
    double angle = atan2(stars[i].y - center_y, stars[i].x - center_x);
    double speed = 0.2 + ((double)rand() / RAND_MAX) * 0.5;

    stars[i].dx = cos(angle) * speed;
    stars[i].dy = sin(angle) * speed;
  }

  init_terminal();
  while (1) {
    for (int i = 0; i < max_stars; i++) {
      newstar(stars[i].x, stars[i].y);
      stars[i].x += stars[i].dx;
      stars[i].y += stars[i].dy;

      if (stars[i].x < 1 || stars[i].x >= w.ws_col || stars[i].y < 1 ||
          stars[i].y >= w.ws_row) {
        stars[i].x = ((double)rand() / RAND_MAX) * w.ws_col;
        stars[i].y = ((double)rand() / RAND_MAX) * w.ws_row;
        double angle = atan2(stars[i].y - center_y, stars[i].x - center_x);
        double speed = 0.2 + ((double)rand() / RAND_MAX) * 0.5;

        stars[i].dx = cos(angle) * speed;
        stars[i].dy = sin(angle) * speed;
      }
    }
    usleep(20000);
    printf("\033[2J");
    fflush(stdout);
  }
  reset_terminal();
  return 0;
}
