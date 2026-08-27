#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5001

int sockfd, clientfd;
struct sockaddr_in server, client;

char code[100];
char gen[] = "10011";
char rem[100];

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

void divideCRC()
{
    char temp[100];
    int i, j;
    int len;

    strcpy(temp, code);

    len = strlen(code);

    for (i = 0; i <= len - strlen(gen); i++)
    {
        if (temp[i] == '1')
        {
            for (j = 0; j < strlen(gen); j++)
            {
                if (temp[i + j] == gen[j])
                    temp[i + j] = '0';
                else
                    temp[i + j] = '1';
            }
        }
    }

    strcpy(rem, temp + len - strlen(gen) + 1);
}

void receiveData()
{
    memset(code, 0, sizeof(code));

    recv(clientfd, code, sizeof(code), 0);

    printf("\nReceived Codeword : %s\n", code);

    divideCRC();

    printf("CRC Remainder     : %s\n", rem);

    if (strspn(rem, "0") == strlen(rem))
    {
        printf("No Error Detected\n");
    }
    else
    {
        printf("Error Detected\n");
    }
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
    closeServer();

    return 0;
}
