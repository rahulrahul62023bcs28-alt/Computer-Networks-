/*=========================================================
        NETWORK LAYERS SIMULATION
        PART 1 - HEADER FILES & GLOBAL DATA
=========================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*---------------- Program Limits ----------------*/

#define TOTAL_URLS      10
#define MAX_MESSAGE     1000
#define MAX_BITS        8000
#define PACKET_SIZE     16
#define FRAME_SIZE      8
#define MAX_PACKETS     600
#define MAX_FRAMES      1200

/*---------------- Website Information ----------------*/

struct Website
{
    char url[50];
    char ip[20];
    char mac[20];
};

/*---------------- Website Database ----------------*/

struct Website network[TOTAL_URLS] =
{
    {"www.google.com",        "142.250.10.1",    "AA:11:22:33:44:55"},
    {"www.youtube.com",       "142.250.10.2",    "AA:11:22:33:44:66"},
    {"www.facebook.com",      "157.240.1.1",     "BB:22:33:44:55:66"},
    {"www.amazon.com",        "205.251.1.1",     "CC:33:44:55:66:77"},
    {"www.wikipedia.org",     "198.35.26.1",     "DD:44:55:66:77:88"},
    {"www.mepcoeng.ac.in",    "192.168.100.1",   "00:1A:2B:3C:4D:5E"},
    {"mail.google.com",       "142.250.10.3",    "10:9F:8E:7D:6C:5B"},
    {"web.whatsapp.com",      "157.240.12.1",    "2C:54:91:A7:B8:C9"},
    {"discord.com",           "162.159.136.232", "48:6F:73:12:34:56"},
    {"teams.microsoft.com",   "52.112.0.1",      "9A:BC:DE:F1:23:45"}
};

/*---------------- Global Variables ----------------*/

char message[MAX_MESSAGE];

char binaryMessage[MAX_BITS];

char packets[MAX_PACKETS][PACKET_SIZE + 1];

char frames[MAX_FRAMES][FRAME_SIZE + 1];

int totalBits = 0;

int totalPackets = 0;

int totalFrames = 0;

int sourcePort;

int destinationPort;

int sourceIndex;

int destinationIndex;

/*---------------- Function Prototypes ----------------*/

/* Utility Functions */

void showWebsites();
int searchWebsite(char url[]);
void readMessage(char fileName[]);
void decimalToBinary(int number,int bits,char binary[]);
void characterToBinary(char ch,char binary[]);
void ipToBinary(char ip[],char binary[]);
void macToBinary(char mac[],char binary[]);

/* Layer Functions */

void applicationLayer();
void transportLayer();
void networkLayer();
void dataLinkLayer();

/* Summary */

void displaySummary();

/* Utility: Convert 8-bit binary string to byte */
unsigned char binaryToByte(char *bin)
{
    unsigned char val = 0;
    int i;
    for(i = 0; i < 8; i++)
    {
        val = val << 1;
        if(bin[i] == '1')
            val |= 1;
    }
    return val;
}

/*=========================================================
        PART 2 - UTILITY FUNCTIONS
=========================================================*/

void showWebsites()
{
    int i;
    printf("\n================ WEBSITE DATABASE ================\n");
    printf("%-3s %-28s %-18s %-20s\n",
            "No","Website","IP Address","MAC Address");
    printf("---------------------------------------------------------------\n");
    for(i=0;i<TOTAL_URLS;i++)
    {
        printf("%-3d %-28s %-18s %-20s\n",
                i+1,
                network[i].url,
                network[i].ip,
                network[i].mac);
    }
    printf("---------------------------------------------------------------\n");
}

int searchWebsite(char url[])
{
    int i;
    for(i=0;i<TOTAL_URLS;i++)
    {
        if(strcmp(url,network[i].url)==0)
        {
            return i;
        }
    }
    return -1;
}

void readMessage(char fileName[])
{
    FILE *fp;
    int ch;
    int index=0;
    fp=fopen(fileName,"r");
    if(fp==NULL)
    {
        printf("\nUnable to open %s\n",fileName);
        exit(0);
    }
    while((ch=fgetc(fp))!=EOF)
    {
        if(ch!='\n' && ch!='\r')
        {
            message[index]=ch;
            index++;
        }
    }
    message[index]='\0';
    fclose(fp);
}

void decimalToBinary(int number,int bits,char binary[])
{
    int i;
    binary[bits]='\0';
    for(i=bits-1;i>=0;i--)
    {
        binary[i]=(number%2)+'0';
        number=number/2;
    }
}

void characterToBinary(char ch,char binary[])
{
    decimalToBinary((unsigned char)ch,8,binary);
}

void ipToBinary(char ip[],char binary[])
{
    int a,b,c,d;
    char temp[9];
    binary[0]='\0';
    sscanf(ip,"%d.%d.%d.%d",&a,&b,&c,&d);
    decimalToBinary(a,8,temp);
    strcat(binary,temp);
    decimalToBinary(b,8,temp);
    strcat(binary,temp);
    decimalToBinary(c,8,temp);
    strcat(binary,temp);
    decimalToBinary(d,8,temp);
    strcat(binary,temp);
}

void macToBinary(char mac[],char binary[])
{
    unsigned int value[6];
    int i;
    char temp[9];
    binary[0]='\0';
    sscanf(mac,"%x:%x:%x:%x:%x:%x",
           &value[0],
           &value[1],
           &value[2],
           &value[3],
           &value[4],
           &value[5]);
    for(i=0;i<6;i++)
    {
        decimalToBinary(value[i],8,temp);
        strcat(binary,temp);
    }
}

/*=========================================================
        PART 3 - APPLICATION & TRANSPORT LAYER
=========================================================*/

void applicationLayer()
{
    int i;
    char temp[9];
    binaryMessage[0]='\0';
    printf("\n");
    printf("=====================================================\n");
    printf("              APPLICATION LAYER\n");
    printf("=====================================================\n");
    printf("\nOriginal Message\n");
    printf("%s\n",message);
    for(i=0;message[i]!='\0';i++)
    {
        characterToBinary(message[i],temp);
        strcat(binaryMessage,temp);
    }
    totalBits=strlen(binaryMessage);
    printf("\nBinary Message\n");
    printf("%s\n",binaryMessage);
    printf("\nTotal Number of Bits : %d\n",totalBits);
}

int generatePort()
{
    int port;
    port=(rand()%50000)+1024;
    return port;
}

void displayPortBinary(int port)
{
    char binary[17];
    decimalToBinary(port,16,binary);
    printf("%s",binary);
}

void transportLayer()
{
    printf("\n");
    printf("=====================================================\n");
    printf("               TRANSPORT LAYER\n");
    printf("=====================================================\n");
    sourcePort=generatePort();
    destinationPort=generatePort();
    printf("\nSource Port      : %d\n",sourcePort);
    printf("Source Binary    : ");
    displayPortBinary(sourcePort);
    printf("\n\n");
    printf("Destination Port : %d\n",destinationPort);
    printf("Destination Binary : ");
    displayPortBinary(destinationPort);
    printf("\n");
}

void displayConnection()
{
    printf("\n");
    printf("=====================================================\n");
    printf("            CONNECTION INFORMATION\n");
    printf("=====================================================\n");
    printf("\nSource URL\n");
    printf("%s\n",network[sourceIndex].url);
    printf("\nDestination URL\n");
    printf("%s\n",network[destinationIndex].url);
    printf("\nSource IP\n");
    printf("%s\n",network[sourceIndex].ip);
    printf("\nDestination IP\n");
    printf("%s\n",network[destinationIndex].ip);
    printf("\nSource MAC\n");
    printf("%s\n",network[sourceIndex].mac);
    printf("\nDestination MAC\n");
    printf("%s\n",network[destinationIndex].mac);
    printf("\nSource Port : %d\n",sourcePort);
    printf("Destination Port : %d\n",destinationPort);
}

/*=========================================================
        PART 4 - NETWORK & DATA LINK LAYER
=========================================================*/

void networkLayer()
{
    int i,j;
    int position=0;
    char finalOutput[200];
    finalOutput[0] = '\0';
    char sourceIPBinary[33];
    char destinationIPBinary[33];
    ipToBinary(network[sourceIndex].ip,sourceIPBinary);
    ipToBinary(network[destinationIndex].ip,destinationIPBinary);
    printf("\n");
    printf("=====================================================\n");
    printf("                 NETWORK LAYER\n");
    printf("=====================================================\n");
    printf("\nSource IP              : %s\n",network[sourceIndex].ip);
    printf("Destination IP         : %s\n",network[destinationIndex].ip);
    printf("\nSource IP Binary\n");
    printf("%s\n",sourceIPBinary);
    printf("\nDestination IP Binary\n");
    printf("%s\n",destinationIPBinary);
    totalPackets=0;
    while(position<totalBits)
    {
        j=0;
        while(j<PACKET_SIZE)
        {
            if(position<totalBits)
            {
                packets[totalPackets][j]=binaryMessage[position];
                position++;
            }
            else
            {
                packets[totalPackets][j]='0';
            }
            j++;
        }
        packets[totalPackets][PACKET_SIZE]='\0';
        totalPackets++;
    }
    printf("\n------------- PACKETS CREATED -------------\n");
    for(i=0;i<totalPackets;i++)
    {
        printf("\nPacket %d\n",i+1);
        printf("Data : %s\n",packets[i]);
        strcat(finalOutput, packets[i]);
    }
    strcat(finalOutput, sourceIPBinary);
    strcat(finalOutput, destinationIPBinary);
    printf("\nFinal Output  : %s\n", finalOutput);
    printf("\nTotal Packets : %d\n",totalPackets);
}

void dataLinkLayer()
{
    int i,j,k;
    int frameIndex=0;
    char finalOutput[300];
    finalOutput[0] = '\0';
    char sourceMACBinary[49];
    char destinationMACBinary[49];
    char sourceIPBinary[33];
    char destinationIPBinary[33];
    char sourcePortBinary[17];
    char destinationPortBinary[17];

    macToBinary(network[sourceIndex].mac,sourceMACBinary);
    macToBinary(network[destinationIndex].mac,destinationMACBinary);
    ipToBinary(network[sourceIndex].ip,sourceIPBinary);
    ipToBinary(network[destinationIndex].ip,destinationIPBinary);
    decimalToBinary(sourcePort, 16, sourcePortBinary);
    decimalToBinary(destinationPort, 16, destinationPortBinary);

    printf("\n");
    printf("=====================================================\n");
    printf("               DATA LINK LAYER\n");
    printf("=====================================================\n");

    printf("\nSource MAC             : %s\n",network[sourceIndex].mac);
    printf("Destination MAC        : %s\n",network[destinationIndex].mac);

    printf("\nSource MAC Binary\n");
    printf("%s\n",sourceMACBinary);

    printf("\nDestination MAC Binary\n");
    printf("%s\n",destinationMACBinary);

    totalFrames=0;

    // Buffer to hold combined header bytes for all frames
    // Each header is 192 bits = 24 bytes per frame
    unsigned char combinedHeaderBytes[MAX_FRAMES * 24];
    int combinedHeaderByteCount = 0;

    for(i=0;i<totalPackets;i++)
    {
        for(j=0;j<PACKET_SIZE;j+=FRAME_SIZE)
        {
            // Copy frame payload
            strncpy(frames[frameIndex],packets[i]+j,FRAME_SIZE);
            frames[frameIndex][FRAME_SIZE]='\0';

            // Build header binary string by concatenating all header fields
            // Order: source port (16 bits), source IP (32 bits), source MAC (48 bits),
            // destination port (16 bits), destination IP (32 bits), destination MAC (48 bits)
            char headerBinary[193]; // 192 bits + null terminator
            headerBinary[0] = '\0';

            strcat(headerBinary, sourcePortBinary);    // 16 bits
            strcat(headerBinary, sourceIPBinary);      // 32 bits
            strcat(headerBinary, sourceMACBinary);     // 48 bits
            strcat(headerBinary, destinationPortBinary);// 16 bits
            strcat(headerBinary, destinationIPBinary); // 32 bits
            strcat(headerBinary, destinationMACBinary);// 48 bits

            // Convert this 192-bit headerBinary to bytes and append to combinedHeaderBytes
            for(k=0; k<192; k+=8)
            {
                unsigned char byte = binaryToByte(headerBinary + k);
                combinedHeaderBytes[combinedHeaderByteCount++] = byte;
            }

            // Print frame info
            printf("\n--------------------------------------\n");
            printf("Frame %d\n",frameIndex+1);
            printf("--------------------------------------\n");
            printf("Header Binary (192 bits): %s\n", headerBinary);
            printf("Payload : %s\n",frames[frameIndex]);
            printf("Trailer : 00000000\n");

            frameIndex++;
            totalFrames++;
        }
    }

    strcat(finalOutput, sourceMACBinary);
    strcat(finalOutput, destinationMACBinary);

    printf("\nTotal Frames : %d\n",totalFrames);
    printf("\n");
}

/*------------ Summary ------------*/

void displaySummary()
{
    printf("\n");
    printf("=====================================================\n");
    printf("                    SUMMARY\n");
    printf("=====================================================\n");
    printf("Total Bits        : %d\n",totalBits);
    printf("Total Packets     : %d\n",totalPackets);
    printf("Total Frames      : %d\n",totalFrames);
    printf("\nSimulation Completed Successfully.\n");
}

int main()
{
    char sourceURL[50];
    char destinationURL[50];

    showWebsites();

    printf("\nEnter Source Website URL: ");
    scanf("%49s", sourceURL);

    printf("Enter Destination Website URL: ");
    scanf("%49s", destinationURL);

    sourceIndex = searchWebsite(sourceURL);
    if(sourceIndex == -1)
    {
        printf("\nSource website not found.\n");
        return 0;
    }

    destinationIndex = searchWebsite(destinationURL);
    if(destinationIndex == -1)
    {
        printf("\nDestination website not found.\n");
        return 0;
    }

    if(sourceIndex == destinationIndex)
    {
        printf("\nSource and destination cannot be the same.\n");
        return 0;
    }

    srand((unsigned int)time(NULL));

    readMessage("message.txt");

    applicationLayer();
    transportLayer();
    displayConnection();
    networkLayer();
    dataLinkLayer();
    displaySummary();

    return 0;
}
