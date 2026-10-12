#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define TABLE_SIZE 20
#define SERVER_PORT 5600

struct ArpEntry { char ipAddr[20]; char macAddr[20]; };

int hashFunc(char ipAddr[])
{
    int total = 0, i;
    for (i = 0; ipAddr[i] != '\0'; i++) total += ipAddr[i];
    return total % TABLE_SIZE;
}

int main()
{
    FILE *pipeFp;
    char lineBuf[200], ipAddr[20], macAddr[20];
    int slot;
    struct ArpEntry arpTable[TABLE_SIZE];
    int listenSock, connSock;
    struct sockaddr_in svrAddr, cliAddr;
    socklen_t cliLen;
    int choice;

    for (int i = 0; i < TABLE_SIZE; i++) { arpTable[i].ipAddr[0] = '\0'; arpTable[i].macAddr[0] = '\0'; }

    srand(time(NULL));
    pipeFp = popen("ip neigh", "r");
    if (pipeFp != NULL)
    {
        while (fgets(lineBuf, sizeof(lineBuf), pipeFp))
        {
            sscanf(lineBuf, "%s", ipAddr);
            if (strstr(lineBuf, "lladdr") != NULL)
            {
                sscanf(strstr(lineBuf, "lladdr") + 7, "%s", macAddr);
                slot = hashFunc(ipAddr);
                strcpy(arpTable[slot].ipAddr, ipAddr);
                strcpy(arpTable[slot].macAddr, macAddr);
            }
        }
        pclose(pipeFp);
    }

    listenSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSock < 0) { perror("Unable to create socket"); return 1; }

    svrAddr.sin_family = AF_INET;
    svrAddr.sin_addr.s_addr = INADDR_ANY;
    svrAddr.sin_port = htons(SERVER_PORT);

    int reuse = 1;
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    if (bind(listenSock, (struct sockaddr*)&svrAddr, sizeof(svrAddr)) < 0) { perror("Bind operation failed"); close(listenSock); return 1; }
    if (listen(listenSock, 5) < 0) { perror("Listen operation failed"); close(listenSock); return 1; }

    printf("ARP Server (concurrent) listening on port %d...\n", SERVER_PORT);

    while (1)
    {
        cliLen = sizeof(cliAddr);
        connSock = accept(listenSock, (struct sockaddr*)&cliAddr, &cliLen);
        if (connSock < 0) { perror("Could not accept incoming connection"); continue; }

        if (fork() == 0)
        {
            close(listenSock);
            printf("\nA new client has connected.\n");

            while (1)
            {
                if (recv(connSock, &choice, sizeof(choice), 0) <= 0) break;

                if (choice == 3) break;
                else if (choice == 2)
                {
                    send(connSock, arpTable, sizeof(arpTable), 0);
                }
                else if (choice == 1)
                {
                    if (recv(connSock, ipAddr, sizeof(ipAddr), 0) <= 0) break;
                    slot = hashFunc(ipAddr);

                    if (strcmp(arpTable[slot].ipAddr, ipAddr) == 0)
                    {
                        int matchFound = 1;
                        send(connSock, &matchFound, sizeof(matchFound), 0);
                        send(connSock, arpTable[slot].macAddr, sizeof(arpTable[slot].macAddr), 0);
                    }
                    else
                    {
                        int matchFound = 0;
                        send(connSock, &matchFound, sizeof(matchFound), 0);
                        sprintf(macAddr, "AA:BB:CC:DD:EE:%02X", rand() % 256);
                        strcpy(arpTable[slot].ipAddr, ipAddr);
                        strcpy(arpTable[slot].macAddr, macAddr);
                        send(connSock, arpTable[slot].ipAddr, sizeof(arpTable[slot].ipAddr), 0);
                        send(connSock, arpTable[slot].macAddr, sizeof(arpTable[slot].macAddr), 0);
                    }
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
