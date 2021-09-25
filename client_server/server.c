#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

int main()
{

    uint64_t sockfd, confd;
    struct sockaddr_in serv, cli; //IPv4 Socket address struct
    socklen_t cli_size;
    char cli_message[2000];
    int read_size;

    printf("Server starting...\n");

    // Defines a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket()");
        return 1;
    }

    // make sure serv struct is 0'd, although
    // since I'm reassigning right after this I probably don't need to
    memset(&serv, 0, sizeof(serv));

    //INADDR_ANY binds the socket to any available interface
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(8888);

    //INADDR_ANY binds the socket to any available interface

    // Binds that socket to an address given a sockaddr struct
    if (bind(sockfd, (struct sockaddr *)&serv, sizeof(serv)) == -1)
    {
        perror("bind()");
        return 1;
    }

    // Converts the socket from an unconnected one to a passive socket,
    // meaning that the kernel should accept incoming requests at this socket
    if (listen(sockfd, SOMAXCONN) == -1)
    {
        perror("listen()");
        return 1;
    }
    printf("Waiting for incoming connection.\n");
    // make sure cli struct is 0'd
    //memset(&cli, 0, sizeof(cli));

    // Accepts an incoming connection on the defined socket, turns it into a request
    // If we don't care about the client's identity, you can set args 2/3 to NULL
    //cli_size = sizeof(cli);
    //confd = accept(sockfd, (struct sockaddr *)&cli, &cli_size);

    confd = accept(sockfd, NULL, NULL);
    if (confd == -1)
    {
        perror("accept()");
        return 1;
    }
    printf("Connection accepted!\n");

    while ((read_size = recv(confd, cli_message, sizeof(cli_message), 0)) > 0)
    {
        write(STDOUT_FILENO, cli_message, strlen(cli_message));
    }

    if (read_size == 0)
    {
        printf("Client terminated session.\n");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv()");
        return 1;
    }

    printf("Closing socket.");
    if (close(sockfd) == -1)
    {
        perror("close()");
        return 1;
    }

    return 0;
}
