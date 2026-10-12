#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define CHUNK_SIZE 1024

int main()
{
    int connFd, bytesGot;
    struct sockaddr_in svrAddr;
    char reqFile[100];
    char dataBuf[CHUNK_SIZE];
    FILE *fp;

    connFd = socket(AF_INET, SOCK_STREAM, 0);
    if (connFd < 0) { perror("Unable to create socket"); exit(1); }

    svrAddr.sin_family = AF_INET;
    svrAddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &svrAddr.sin_addr) <= 0) { perror("Server address is invalid"); close(connFd); exit(1); }
    if (connect(connFd, (struct sockaddr *)&svrAddr, sizeof(svrAddr)) < 0) { perror("Unable to reach the server"); close(connFd); exit(1); }

    printf("Connected to FTP Server.\n");
    printf("Enter the filename you want to request: ");
    scanf("%s", reqFile);
    send(connFd, reqFile, strlen(reqFile), 0);

    bytesGot = recv(connFd, dataBuf, sizeof(dataBuf) - 1, 0);
    if (bytesGot <= 0) { printf("Server did not respond.\n"); close(connFd); return 0; }

    dataBuf[bytesGot] = '\0';
    if (strcmp(dataBuf, "NO") == 0) { printf("The requested file was not found on the server.\n"); close(connFd); return 0; }

    fp = fopen(reqFile, "wb");
    if (fp == NULL) { printf("Unable to create a local copy of the file.\n"); close(connFd); return 0; }

    while ((bytesGot = recv(connFd, dataBuf, CHUNK_SIZE, 0)) > 0) fwrite(dataBuf, 1, bytesGot, fp);

    printf("File download complete.\n");
    fclose(fp);
    close(connFd);
    return 0;
}
