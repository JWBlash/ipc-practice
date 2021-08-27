#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "nested.h"

int main(int argc, char *argv[])
{
    int pipefd[2];
    pid_t cpid;
    char buf;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork(); // now there are two processes. The child will see "cpid == 0", the parent will see "cpid == else" (the child process pid).
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0)
    {                                                  /* Child reads from pipe */
        close(pipefd[1]); /* Close unused write end */ //the child still has both ends of the pipe, but it's only using one. Hence the close.
        printf("%d: Child process: cpid = %d, PID = %d\n", getpid(), cpid, getpid());

        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        nested(argv[1]);
        printf("%d: Child process %d: taking a lil nap\n", getpid(), getpid());
        sleep(5);
        _exit(EXIT_SUCCESS);
    }
    else
    {                                                 /* Parent writes argv[1] to pipe */
        close(pipefd[0]); /* Close unused read end */ //the parent still has both ends of the pipe, but it's only using one. Hence the close.
        printf("%d: Parent process: cpid = %d, PID = %d\n", getpid(), cpid, getpid());
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]); /* Reader will see EOF */
        printf("%d: Waiting for child to exit...\n", getpid());
        wait(NULL); /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}
