#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int nested(char *buf)
{
    int pipefd[2];
    pid_t cpid;

    if (strlen(buf) == 0)
    {
        perror("empty string");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    //forked!
    if (cpid == 0)
    {
        close(pipefd[1]); // close unused write end (we're only reading what comes from the parent)
        printf("%d: NESTED Child process starting.\n", getpid());

        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]); //close pipe
        printf("%d: Taking a nap.\n", getpid());
        sleep(5);
        _exit(EXIT_SUCCESS);
    }
    else
    {
        // do the parent process thing here
        close(pipefd[0]); //close unused read end (only writing to the child, not reading from it)
        printf("%d: NESTED parent process starting.\n", getpid());

        write(pipefd[1], buf, strlen(buf));
        close(pipefd[1]);
        printf("%d: Waiting for nested child (%d) to exit...\n", getpid(), cpid);
        wait(NULL);
        return 0;
    }
}
