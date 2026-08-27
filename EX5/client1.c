#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int sockfd;
struct sockaddr_in server;
char data[100];
char reply[100];

void createClient()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
        return;
    }

    printf("Client socket created\n");
}

void connectServer()
{
    server.sin_family = AF_INET;

    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    server.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Connection failed\n");
        return;
    }

    printf("Connected to server\n");
}

void sendData()
{
    printf("Enter a string: ");
    fgets(data, sizeof(data), stdin);

    data[strcspn(data, "\n")] = '\0';

    send(sockfd, data, strlen(data), 0);
}

void receiveData()
{
    memset(reply, 0, sizeof(reply));

    recv(sockfd, reply, sizeof(reply), 0);

    printf("Echo from server: %s\n", reply);
}

void closeClient()
{
    close(sockfd);

    printf("Connection closed\n");
}

int main()
{
    createClient();
    connectServer();
    sendData();
    receiveData();
    closeClient();

    return 0;
}
