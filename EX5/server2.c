#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int sockfd, clientfd;
struct sockaddr_in server, client;

int a[20];
int n;
int sum;

void createServer()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        printf("Socket creation failed\n");
        return;
    }

    printf("Server socket created\n");
}

void bindServer()
{
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Bind failed\n");
        return;
    }

    printf("Server bind successful\n");
}

void waitClient()
{
    listen(sockfd, 5);

    printf("Waiting for client...\n");

    socklen_t len = sizeof(client);

    clientfd = accept(sockfd, (struct sockaddr *)&client, &len);

    if (clientfd < 0)
    {
        printf("Client connection failed\n");
        return;
    }

    printf("Client connected\n");
}

void receiveData()
{
    recv(clientfd, &n, sizeof(n), 0);
    recv(clientfd, a, sizeof(a), 0);

    printf("\nReceived array:\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }

    printf("\n");
}

void findSum()
{
    sum = 0;

    for (int i = 0; i < n; i++)
    {
        sum = sum + a[i];
    }

    printf("Sum = %d\n", sum);
}

void sendResult()
{
    send(clientfd, &sum, sizeof(sum), 0);

    printf("Sum sent to client\n");
}

void closeServer()
{
    close(clientfd);
    close(sockfd);

    printf("Connection closed\n");
}

int main()
{
    createServer();
    bindServer();
    waitClient();
    receiveData();
    findSum();
    sendResult();
    closeServer();

    return 0;
}
