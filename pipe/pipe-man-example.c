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

    cpid = fork(); 
    if (cpid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (cpid == 0)
    {            
        close(pipefd[1]); /* Close unused write end */ 
        printf("%d: Child process starting.\n", getpid());

        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        printf("%d: Nested routine starting.\n", getpid());
        nested(argv[1]);
        printf("%d: Returned from nested routine.\n", getpid());
        printf("%d: Taking a nap.\n", getpid());
        sleep(5);
        _exit(EXIT_SUCCESS);
    }
    else
    {
        close(pipefd[0]); /* Close unused read end */ 
        printf("%d: Parent process starting.\n", getpid());
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]); /* Reader will see EOF */
        printf("%d: Waiting for child (%d) to exit...\n", getpid(), cpid);
        wait(NULL); /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}
