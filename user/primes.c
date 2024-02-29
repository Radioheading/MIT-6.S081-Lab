#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RANGE_MIN 2
#define RANGE_MAX 35

int work(int input_fd) {
  int p[2];
  pipe(p);

  int prime = 0;
  if (read(input_fd, &prime, sizeof(prime)) == 0) {
    close(input_fd);
    close(p[0]);
    close(p[1]);
    exit(0);
  }

  printf("prime %d\n", prime);

  if (fork()) {
    close(p[0]);
    int cur = 0;
    while (read(input_fd, &cur, sizeof(cur))) {
      if (cur % prime != 0) {
        write(p[1], &cur, sizeof(cur));
      }
    }
    close(input_fd);
    close(p[1]);
  } else {
    close(p[1]);
    work(p[0]);
  }

  wait(0);
  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc != 1) {
    fprintf(2, "usage: primes...\n");
    exit(1);
  }

  int p[2];
  pipe(p);

  if (fork()) { // the main process simply writes the number 2 to 35 to the first pipe
    close(p[0]);
    for (int i = RANGE_MIN; i <= RANGE_MAX; i++) {
      write(p[1], &i, sizeof(i));
    }
    close(p[1]);
  } else {
    close(p[1]);
    work(p[0]);
  }
  wait(0);
  exit(0);
}
