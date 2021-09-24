#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

int main()
{
    printf("Server starting...");

    uint64_t sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket()");
    }
    printf("Socket fd: %" PRIi64 "\n", sock);

    int lis = listen(sock, SOMAXCONN);
    if (lis == -1)
    {
        perror("listen()");
    }

    close(sock);

    return 0;
}