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

    cpid = fork(); // again, two processes. In here, the parent is the child of the process running main()
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    //forked!
    if (cpid == 0)
    {
        close(pipefd[1]); // close unused write end (we're only reading what comes from the parent)
        printf("%d: Child process: cpid = %d, PID = %d\n", getpid(), cpid, getpid());

        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]); //close pipe
        printf("%d: Child process %d: taking a lil nap\n", getpid(), getpid());
        sleep(5);
        _exit(EXIT_SUCCESS);
    }
    else
    {
        // do the parent process thing here
        close(pipefd[0]); //close unused read end (only writing to the child, not reading from it)
        printf("%d: Parent process: cpid = %d, PID = %d\n", getpid(), cpid, getpid());

        write(pipefd[1], buf, strlen(buf));
        close(pipefd[1]);
        printf("%d: Waiting for child to exit...\n", getpid());
        wait(NULL);
        _exit(EXIT_SUCCESS);
    }
}