#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int sockfd;
struct sockaddr_in server;

int a[20];
int n;
int sum;

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

void getData()
{
    printf("Enter number of elements: ");
    scanf("%d", &n);

    printf("Enter %d elements:\n", n);

    for (int i = 0; i < n; i++)
    {
        scanf("%d", &a[i]);
    }
}

void sendData()
{
    send(sockfd, &n, sizeof(n), 0);
    send(sockfd, a, sizeof(a), 0);

    printf("Array sent to server\n");
}

void receiveResult()
{
    recv(sockfd, &sum, sizeof(sum), 0);

    printf("Sum received from server = %d\n", sum);
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
    getData();
    sendData();
    receiveResult();
    closeClient();

    return 0;
}
