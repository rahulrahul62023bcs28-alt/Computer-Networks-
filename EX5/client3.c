#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 5001

int sockfd;
struct sockaddr_in server;

char data[100];
char augmented[100];
char rem[100];
char code[100];

char gen[] = "10011";

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

void divideCRC()
{
    char temp[100];
    int i, j;
    int len;

    strcpy(temp, augmented);

    len = strlen(augmented);

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

void getData()
{
    printf("Enter binary data: ");
    scanf("%99s", data);
}

void makeCRC()
{
    int i;

    strcpy(augmented, data);

    for (i = 0; i < strlen(gen) - 1; i++)
    {
        strcat(augmented, "0");
    }

    printf("\nGenerator : %s", gen);
    printf("\nAugmented Data : %s", augmented);

    divideCRC();

    printf("\nCRC Remainder : %s", rem);

    strcpy(code, data);
    strcat(code, rem);

    printf("\nCodeword : %s\n", code);
}

void sendData()
{
    send(sockfd, code, strlen(code) + 1, 0);

    printf("Codeword sent to server\n");
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
    makeCRC();
    sendData();
    closeClient();

    return 0;
}
