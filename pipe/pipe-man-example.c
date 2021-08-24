#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int nested(char buf, int bufsize)
{
    int pipefd[2];
    pid_t cpid;

    if (bufsize == 0)
    {
        perror("bufsize null");
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

        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]); //close pipe
        _exit(EXIT_SUCCESS);
    }
    else
    {
        // do the parent process thing here
    }
}

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
        printf("Child process: cpid = %d, PID = %d\n", cpid, getpid());

        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        sleep(5);
        _exit(EXIT_SUCCESS);
    }
    else
    {                                                 /* Parent writes argv[1] to pipe */
        close(pipefd[0]); /* Close unused read end */ //the parent still has both ends of the pipe, but it's only using one. Hence the close.
        printf("Parent process: cpid = %d, PID = %d\n", cpid, getpid());
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]); /* Reader will see EOF */
        printf("Waiting for child to exit...\n");
        wait(NULL); /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}
