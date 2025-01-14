#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void handle_client(int client_socket) 
{
    char buffer[BUFFER_SIZE];

    while (1) 
	{
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0)
            break;

        printf("Received message: %s\n", buffer);

        printf("Enter your response: ");
        fflush(stdout);
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
}

int main() 
{
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) 
	{
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) 
	{
        perror("Failed to bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 5) < 0) 
	{
        perror("Failed to listen");
        exit(EXIT_FAILURE);
    }

    printf("Server started and listening on port %d\n", PORT);

    while (1) 
	{
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
        if (client_socket < 0) 
		{
            perror("Failed to accept client connection");
            continue;
        }

        printf("Connected with client\n");

        if (fork() == 0) 
		{
            close(server_socket);
            handle_client(client_socket);
            exit(EXIT_SUCCESS);
        }
        close(client_socket);
    }

    close(server_socket);

    return 0;
}
