#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define TABLE_SIZE 20
#define SERVER_PORT 5600

struct ArpEntry { char ipAddr[20]; char macAddr[20]; };

void printTable(struct ArpEntry arpTable[])
{
    printf("\nARP TABLE RECEIVED FROM SERVER\n-------------------------------\nIP Address\tMAC Address\n-------------------------------\n");
    for (int i = 0; i < TABLE_SIZE; i++) if (strlen(arpTable[i].ipAddr) > 0) printf("%s\t%s\n", arpTable[i].ipAddr, arpTable[i].macAddr);
    printf("-------------------------------\n");
}

int main()
{
    char ipAddr[20], macAddr[20];
    int connFd;
    struct sockaddr_in svrAddr;
    int choice;
    struct ArpEntry arpTable[TABLE_SIZE];

    connFd = socket(AF_INET, SOCK_STREAM, 0);
    if (connFd < 0) { perror("Unable to create socket"); return 1; }

    svrAddr.sin_family = AF_INET;
    svrAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    svrAddr.sin_port = htons(SERVER_PORT);

    if (connect(connFd, (struct sockaddr*)&svrAddr, sizeof(svrAddr)) < 0) { printf("Unable to reach the ARP server.\n"); close(connFd); return 1; }

    printf("Connected to the ARP Server (concurrent).\n");

    while (1)
    {
        printf("\nSelect option (1. Lookup, 2. Show Table, 3. Quit): ");
        scanf("%d", &choice);
        send(connFd, &choice, sizeof(choice), 0);

        if (choice == 3) break;
        else if (choice == 2)
        {
            recv(connFd, arpTable, sizeof(arpTable), 0);
            printTable(arpTable);
        }
        else if (choice == 1)
        {
            printf("\nEnter IP address to look up: ");
            scanf("%s", ipAddr);
            send(connFd, ipAddr, sizeof(ipAddr), 0);

            int matchFound;
            recv(connFd, &matchFound, sizeof(matchFound), 0);

            if (matchFound == 1)
            {
                recv(connFd, macAddr, sizeof(macAddr), 0);
                printf("Match found. MAC Address: %s\n", macAddr);
            }
            else
            {
                char freshIp[20], freshMac[20];
                printf("No existing entry for this IP.\n");
                recv(connFd, freshIp, sizeof(freshIp), 0);
                recv(connFd, freshMac, sizeof(freshMac), 0);
                printf("Storing new entry locally...\nIP  : %s\nMAC : %s\n", freshIp, freshMac);
            }
        }
        else printf("That option is not recognized.\n");
    }

    close(connFd);
    return 0;
}
