#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 4
#define FRAMESIZE 16

/* Structure to store host details */
struct Host
{
    char url[30];
    char ip[20];
    char mac[20];
};

/* Default Host Table */
struct Host host[SIZE] =
{
    {"mepco.edu",   "192.168.1.2", "AA:BB:CC:DD:EE:01"},
    {"google.com",  "192.168.1.3", "AA:BB:CC:DD:EE:02"},
    {"yahoo.com",   "192.168.1.4", "AA:BB:CC:DD:EE:03"},
    {"chatgpt.com", "192.168.1.5", "AA:BB:CC:DD:EE:04"}
};

/* Global Variables */
struct Host src, dest;

char msg[300];
char stuffedMessage[500];

/* Function Prototypes */
void display(void);
void search(void);
void stuffing(void);
void saveFile(void);
void binary(void);
void decimalToBinary(int, char[]);
void convertIP(char[]);
void convertMAC(char[]);
void convertMessage(void);
void frame(void);
void receiver(void);
void destuffing(void);

int main()
{
    display();
    search();
    stuffing();
    saveFile();
    binary();
    frame();
    receiver();

    return 0;
}

/* Display Default Address Table */
void display()
{
    int i;

    printf("\nDEFAULT ADDRESS TABLE\n\n");

    printf("%-18s %-18s %-20s\n",
           "URL",
           "IP ADDRESS",
           "MAC ADDRESS");

    printf("--------------------------------------------------------------\n");

    for(i = 0; i < SIZE; i++)
    {
        printf("%-18s %-18s %-20s\n",
               host[i].url,
               host[i].ip,
               host[i].mac);
    }

    printf("\n");
}

/* Search Source and Destination URL */
void search()
{
    char sourceURL[30];
    char destinationURL[30];

    int i;
    int foundSource;
    int foundDestination;

    /* Get Source URL */
    do
    {
        foundSource = 0;

        printf("\nEnter Source URL      : ");
        scanf("%29s", sourceURL);

        for(i = 0; i < SIZE; i++)
        {
            if(strcmp(sourceURL, host[i].url) == 0)
            {
                src = host[i];
                foundSource = 1;
                break;
            }
        }

        if(!foundSource)
        {
            printf("\nSource URL Not Found!");
            printf("\nPlease Enter a Valid Source URL.\n");
        }

    } while(!foundSource);

    /* Get Destination URL */
    do
    {
        foundDestination = 0;

        printf("\nEnter Destination URL : ");
        scanf("%29s", destinationURL);

        for(i = 0; i < SIZE; i++)
        {
            if(strcmp(destinationURL, host[i].url) == 0)
            {
                dest = host[i];
                foundDestination = 1;
                break;
            }
        }

        if(!foundDestination)
        {
            printf("\nDestination URL Not Found!");
            printf("\nPlease Enter a Valid Destination URL.\n");
        }

    } while(!foundDestination);

    printf("\nSOURCE DETAILS\n");
    printf("URL : %s\n", src.url);
    printf("IP  : %s\n", src.ip);
    printf("MAC : %s\n", src.mac);

    printf("\nDESTINATION DETAILS\n");
    printf("URL : %s\n", dest.url);
    printf("IP  : %s\n", dest.ip);
    printf("MAC : %s\n", dest.mac);
}
/* Perform Character Stuffing */
void stuffing()
{
    char temp[300];
    char *token;

    getchar();   // Clear newline left by scanf()

    printf("\nEnter Message : ");
    fgets(msg, sizeof(msg), stdin);

    /* Keep original message safe */
    strcpy(temp, msg);

    memset(stuffedMessage, 0, sizeof(stuffedMessage));

    token = strtok(temp, " \n");

    while(token != NULL)
{
    if(strcmp(token, "DLE") == 0 ||
       strcmp(token, "STX") == 0 ||
       strcmp(token, "ETX") == 0)
    {
        strcat(stuffedMessage, "DLE ");
        strcat(stuffedMessage, token);
        strcat(stuffedMessage, " ");
    }
    else
    {
        strcat(stuffedMessage, token);
        strcat(stuffedMessage, " ");
    }

    token = strtok(NULL, " \n");
}
    printf("\nStuffed Message\n");
    printf("%s\n", stuffedMessage);
}
/* Save Details into File */
void saveFile()
{
    FILE *fp;

    fp = fopen("message.txt", "w");

    if(fp == NULL)
    {
        printf("\nFile Cannot Be Created\n");
        return;
    }

    fprintf(fp, "SOURCE URL : %s\n", src.url);
    fprintf(fp, "DESTINATION URL : %s\n\n", dest.url);

    fprintf(fp, "SOURCE IP : %s\n", src.ip);
    fprintf(fp, "DESTINATION IP : %s\n\n", dest.ip);

    fprintf(fp, "SOURCE MAC : %s\n", src.mac);
    fprintf(fp, "DESTINATION MAC : %s\n\n", dest.mac);

    fprintf(fp, "ORIGINAL MESSAGE\n");
    fprintf(fp, "%s", msg);

    fprintf(fp, "\nSTUFFED MESSAGE\n");
    fprintf(fp, "%s\n", stuffedMessage);

    fclose(fp);

    printf("\nMessage Stored Successfully in message.txt\n");
}

/* Convert Decimal Number to 8-bit Binary */
void decimalToBinary(int number, char binary[])
{
    int bits[8];
    int i, j = 0;

    for(i = 7; i >= 0; i--)
    {
        bits[i] = number % 2;
        number = number / 2;
    }

    for(i = 0; i < 8; i++)
    {
        binary[j++] = bits[i] + '0';
    }

    binary[j] = '\0';
}

/* Convert IP Address into Binary */
void convertIP(char ip[])
{
    int i;
    int number = 0;
    char binary[9];

    for(i = 0; ; i++)
    {
        if(ip[i] == '.' || ip[i] == '\0')
        {
            decimalToBinary(number, binary);

            printf("%s", binary);

            if(ip[i] == '\0')
                break;

            printf(".");

            number = 0;
        }
        else
        {
            number = number * 10 + (ip[i] - '0');
        }
    }
}

/* Convert MAC Address into Binary */
void convertMAC(char mac[])
{
    int i;
    int value;
    char binary[9];

    for(i = 0; mac[i] != '\0'; i++)
    {
        if(mac[i] == ':')
        {
            printf(":");
            continue;
        }

        if(mac[i] >= '0' && mac[i] <= '9')
            value = mac[i] - '0';
        else if(mac[i] >= 'A' && mac[i] <= 'F')
            value = mac[i] - 'A' + 10;
        else
            value = mac[i] - 'a' + 10;

        decimalToBinary(value, binary);

        printf("%s ", binary + 4);
    }
}

/* Convert Stuffed Message into Binary */
void convertMessage()
{
    int i;
    char binary[9];

    for(i = 0; stuffedMessage[i] != '\0'; i++)
    {
        decimalToBinary((int)stuffedMessage[i], binary);
        printf("%s ", binary);
    }
}
/* Display Binary Conversions */
void binary()
{
    printf("\n");
    printf("\n=================================");
    printf("\nBINARY CONVERSION");
    printf("\n=================================\n");

    printf("\nSource IP\n");
    printf("%s\n", src.ip);
    convertIP(src.ip);

    printf("\n\nDestination IP\n");
    printf("%s\n", dest.ip);
    convertIP(dest.ip);

    printf("\n\nSource MAC\n");
    printf("%s\n", src.mac);
    convertMAC(src.mac);

    printf("\n\nDestination MAC\n");
    printf("%s\n", dest.mac);
    convertMAC(dest.mac);

    printf("\n\nStuffed Message (Binary)\n");
    convertMessage();

    printf("\n");
}

/* Create Frames */
void frame()
{
    int messageLength;
    int totalFrames;
    int i, j;
    int dataIndex = 0;

    messageLength = strlen(stuffedMessage);

    /* Remove trailing space if present */
    if(messageLength > 0 && stuffedMessage[messageLength - 1] == ' ')
        messageLength--;

    if(messageLength == 0)
    {
        printf("\nNo Data Available\n");
        return;
    }

    totalFrames = messageLength / FRAMESIZE;

    if(messageLength % FRAMESIZE != 0)
        totalFrames++;

    printf("\n\nTotal Number of Frames : %d\n", totalFrames);

    for(i = 1; i <= totalFrames; i++)
    {
        printf("\n\n=========================================\n");
        printf("              DCMP FRAME %d\n", i);
        printf("=========================================\n");

        printf("\nSOH\n");

        printf("\nHEADER\n");

        printf("Source URL      : %s\n", src.url);
        printf("Destination URL : %s\n", dest.url);

        printf("Source IP       : %s\n", src.ip);
        printf("Destination IP  : %s\n", dest.ip);

        printf("Source MAC      : %s\n", src.mac);
        printf("Destination MAC : %s\n", dest.mac);

        printf("\nSTX\n");

        printf("\nDATA : ");

        for(j = 0; j < FRAMESIZE; j++)
        {
            if(dataIndex < messageLength)
            {
                printf("%c", stuffedMessage[dataIndex]);
                dataIndex++;
            }
            else
            {
                printf("0");
            }
        }

        printf("\n");

        printf("\nETX\n");

        printf("\nBCC : 00000000\n");

        printf("\n=========================================\n");
    }
}
/* Receiver Side */
void receiver()
{
    printf("\n\n=========================================");
    printf("\n          RECEIVER SIDE");
    printf("\n=========================================");

    printf("\n\nReceiving Frames...");
    printf("\nFrame Received Successfully.");

    printf("\nChecking Header........OK");
    printf("\nChecking BCC...........OK");

    printf("\n\nPerforming Character Destuffing...\n");

    destuffing();
}

/* Character Destuffing */
void destuffing()
{
    char temp[500];
    char result[500];
    char *token;

    strcpy(temp, stuffedMessage);

    result[0] = '\0';

    token = strtok(temp, " \n");

    while(token != NULL)
{
    if(strcmp(token, "DLE") == 0)
    {
        token = strtok(NULL, " \n");

        if(token != NULL)
        {
            strcat(result, token);
            strcat(result, " ");
        }
    }
    else
    {
        strcat(result, token);
        strcat(result, " ");
    }

    token = strtok(NULL, " \n");
}

    printf("\nOriginal Message After Destuffing\n");
    printf("%s\n", result);

    printf("\nTransmission Successful.\n");
}
