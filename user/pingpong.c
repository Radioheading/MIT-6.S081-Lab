#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 1) {
    fprintf(2, "usage: pingpong...\n");
    exit(1);
  }

  int p[2];

  pipe(p);

  if (fork() == 0) {
    close(p[1]);
    char buf[1];
    read(p[0], buf, 1);
    printf("%d: received ping\n", getpid());
    close(p[0]);
    write(p[1], buf, 1);
    close(p[1]);
    exit(0);
  } else {
    close(p[0]);
    write(p[1], "s", 1);
    close(p[1]);
    wait((int *) 0);
    char buf[1];
    read(p[0], buf, 1);
    printf("%d: received pong\n", getpid());
    close(p[0]);
  }

  exit(0);
}
