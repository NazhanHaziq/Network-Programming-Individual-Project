#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() 
{
    int client_socket;
    struct addrinfo hints, *result, *ptr;
    char buffer[BUFFER_SIZE];

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo("ubuntu", NULL, &hints, &result) != 0) 
	{
        perror("Failed to get address info");
        exit(EXIT_FAILURE);
    }

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) 
	{
        client_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (client_socket == -1)
            continue;

        if (connect(client_socket, ptr->ai_addr, ptr->ai_addrlen) == 0)
            break;

        close(client_socket);
    }

    freeaddrinfo(result);

    if (ptr == NULL) 
	{
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server.\n");

    while (1) 
	{
        printf("Enter your message: ");
        fflush(stdout);

        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
            break;

        send(client_socket, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0)
            break;

        printf("Received response: %s\n", buffer);
    }

    close(client_socket);

    return 0;
}
/*

if (getaddrinfo(argv[1], argv[2], &peer_address))
{
	fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
	return 1;
}

*/
