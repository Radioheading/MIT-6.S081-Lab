#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define BUFFER_SIZE 512

int is_space(char c) {
    return c == ' ' || c == '\t';
}

int main(int argc, char *argv[]) {
    char buf[512];
    char *args[MAXARG];
    int arg_size = 0;

    if (argc <= 1) {
        fprintf(2, "usage: xargs command\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        args[arg_size++] = argv[i];
//        printf("args[%d] = %s\n", arg_size - 1, args[arg_size - 1]);
    }

    // read from stdin, one by one

    char in;

    int cur_len = 0;

    args[arg_size] = buf;

    while (1) {
        int n = read(0, &in, 1);

        if (n == 0) {
            break;
        } else {
//            printf("read: %c\n", in);
            if (is_space(in)) {
                buf[cur_len++] = '\0';
                args[++arg_size] = buf + cur_len;
//                printf("args[%d] = %s\n", arg_size - 1, args[arg_size - 1]);
            } else if (in == '\n') {
                buf[cur_len++] = '\0';
                args[++arg_size] = buf + cur_len;
//                printf("args[%d] = %s\n", arg_size - 1, args[arg_size - 1]);
                if (fork() == 0) {
                    args[arg_size] = 0;
                    printf("exec: %s\n", argv[0]);
                    exec(args[0], args);
                } else {
                    wait(0);
                    cur_len = 0;
                    arg_size = argc - 1;
                    args[arg_size] = buf;
                }
            } else {
                buf[cur_len++] = in;
//                printf("buf: %s\n", buf);
            }
        }
    }

    exit(0);
}
