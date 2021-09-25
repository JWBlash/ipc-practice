#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(void)
{
    uint32_t sockfd, confd, read_size;
    size_t pos;
    struct sockaddr_in serv; //IPv4 Socket address struct
    char cli_message[1024];

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

    confd = accept(sockfd, NULL, NULL);
    if (confd == -1)
    {
        perror("accept()");
        return 1;
    }
    printf("Connection accepted!\n");

    memset(cli_message, 0, sizeof(cli_message));
    pos = 0;
    while (1) 
    {
        read_size = recv(confd, cli_message + pos, sizeof(cli_message) - pos, 0);
        if (read_size == 0) 
        {
            printf("Client terminated session.\n");
            fflush(stdout);
            break;
        }
        else if (read_size == -1) 
        {
            perror("recv()");
            return 1;
        }
        pos += read_size;
        char *end = strstr(cli_message, "\r\n\r\n");
        if (end) 
        {
            size_t msg_len = end - cli_message;
        }

    }

    /*
    while ((read_size = recv(confd, cli_message, sizeof(cli_message), 0)) > 0)
    {
        write(STDOUT_FILENO, cli_message, strlen(cli_message));
    }
    */

    printf("Closing socket.\n");
    if (close(sockfd) == -1)
    {
        perror("close()");
        return 1;
    }

    return 0;
}
