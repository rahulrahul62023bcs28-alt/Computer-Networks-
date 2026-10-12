#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int listenSock, connSock;
    struct sockaddr_in svrAddr, cliAddr;
    socklen_t cliLen = sizeof(cliAddr);

    listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0) { printf("Unable to create socket\n"); return 1; }

    memset(&svrAddr, 0, sizeof(svrAddr));
    svrAddr.sin_family = AF_INET;
    svrAddr.sin_port = htons(5000);
    svrAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSock, (struct sockaddr *)&svrAddr, sizeof(svrAddr)) < 0) { printf("Bind operation failed\n"); close(listenSock); return 1; }
    if (listen(listenSock, 5) < 0) { printf("Listen operation failed\n"); close(listenSock); return 1; }

    printf("Chat Server (concurrent) started. Waiting for clients...\n");

    while (1)
    {
        connSock = accept(listenSock, (struct sockaddr *)&cliAddr, &cliLen);
        if (connSock < 0) { printf("Could not accept incoming connection\n"); continue; }

        if (fork() == 0)
        {
            close(listenSock);
            char inMsg[1024], outMsg[1024];
            printf("\nA new client has joined.\n");

            while (1)
            {
                int bytesRead = recv(connSock, inMsg, sizeof(inMsg) - 1, 0);
                if (bytesRead <= 0) { printf("Client connection closed.\n"); break; }
                inMsg[bytesRead] = '\0';
                if (strcmp(inMsg, "exit") == 0) { printf("Client connection closed.\n"); break; }

                printf("Client says: %s\n", inMsg);
                printf("Your reply: ");
                fgets(outMsg, sizeof(outMsg), stdin);
                outMsg[strcspn(outMsg, "\n")] = '\0';
                send(connSock, outMsg, strlen(outMsg), 0);
                if (strcmp(outMsg, "exit") == 0) { printf("Server ended the chat session.\n"); break; }
            }

            close(connSock);
            exit(0);
        }

        close(connSock);
    }

    close(listenSock);
    return 0;
}

rahul_24@Rahul-chloo:~/SEM5/CN/EX7/chat$ cat client.c
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int connFd;
    char outMsg[1024], inMsg[1024];
    struct sockaddr_in svrAddr;

    connFd = socket(AF_INET, SOCK_STREAM, 0);
    if (connFd < 0) { printf("Unable to create socket\n"); return 1; }

    memset(&svrAddr, 0, sizeof(svrAddr));
    svrAddr.sin_family = AF_INET;
    svrAddr.sin_port = htons(5000);
    svrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(connFd, (struct sockaddr *)&svrAddr, sizeof(svrAddr)) < 0) { printf("Unable to connect to server\n"); close(connFd); return 1; }

    printf("Connected to Chat Server. Type 'exit' to quit.\n");

    while (1)
    {
        printf("You: ");
        fgets(outMsg, sizeof(outMsg), stdin);
        outMsg[strcspn(outMsg, "\n")] = '\0';
        send(connFd, outMsg, strlen(outMsg), 0);
        if (strcmp(outMsg, "exit") == 0) break;

        int bytesRead = recv(connFd, inMsg, sizeof(inMsg) - 1, 0);
        if (bytesRead <= 0) { printf("Connection to server was lost.\n"); break; }
        inMsg[bytesRead] = '\0';
        if (strcmp(inMsg, "exit") == 0) { printf("Server has closed the chat.\n"); break; }

        printf("Server: %s\n", inMsg);
    }

    close(connFd);
    return 0;
}
