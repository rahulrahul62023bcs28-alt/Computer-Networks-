#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define CHUNK_SIZE 1024

int main()
{
    int listenSock, connSock;
    struct sockaddr_in svrAddr, cliAddr;
    socklen_t cliLen = sizeof(cliAddr);

    listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0) { perror("Unable to create socket"); exit(1); }

    svrAddr.sin_family = AF_INET;
    svrAddr.sin_port = htons(SERVER_PORT);
    svrAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSock, (struct sockaddr *)&svrAddr, sizeof(svrAddr)) < 0) { perror("Bind operation failed"); close(listenSock); exit(1); }
    if (listen(listenSock, 5) < 0) { perror("Listen operation failed"); close(listenSock); exit(1); }

    printf("FTP Server (concurrent) is listening on port %d...\n", SERVER_PORT);

    while (1)
    {
        cliLen = sizeof(cliAddr);
        connSock = accept(listenSock, (struct sockaddr *)&cliAddr, &cliLen);
        if (connSock < 0) { perror("Could not accept incoming connection"); continue; }

        printf("\nA new client has connected.\n");

        if (fork() == 0)
        {
            close(listenSock);
            char reqFile[100];
            char dataBuf[CHUNK_SIZE];
            FILE *fp;
            int bytesGot;

            bytesGot = recv(connSock, reqFile, sizeof(reqFile) - 1, 0);
            if (bytesGot > 0)
            {
                reqFile[bytesGot] = '\0';
                printf("File requested: %s\n", reqFile);

                fp = fopen(reqFile, "rb");
                if (fp == NULL)
                {
                    printf("Requested file does not exist.\n");
                    send(connSock, "NO", 2, 0);
                }
                else
                {
                    send(connSock, "OK", 2, 0);
                    while ((bytesGot = fread(dataBuf, 1, CHUNK_SIZE, fp)) > 0) send(connSock, dataBuf, bytesGot, 0);
                    printf("File transfer complete.\n");
                    fclose(fp);
                }
            }

            close(connSock);
            printf("Client has disconnected.\n");
            exit(0);
        }

        close(connSock);
    }

    close(listenSock);
    return 0;
}
