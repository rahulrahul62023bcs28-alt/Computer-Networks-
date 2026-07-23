#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FRAME_SIZE 16

char message[300];

/* Function Prototypes */
void readMessage();
void writeMessageToFile(const char *filename);
void readMessageFromFile(const char *filename);
void decimalToBinary(int number, char binary[]);
void numberToBinary(int number,int bits,char binary[]);
void createFrame();
void receiver();

/* Main Function */
int main()
{
    readMessage();

    // Write message to file (sender side)
    writeMessageToFile("message.txt");

    // Read message from file (receiver side)
    readMessageFromFile("message.txt");

    createFrame();

    receiver();

    return 0;
}

/* Read Message from user */
void readMessage()
{
    printf("Enter Message : ");

    fgets(message,sizeof(message),stdin);

    message[strcspn(message,"\n")] = '\0';

    if(strlen(message) == 0)
    {
        printf("Message is Empty.\n");
        exit(0);
    }
}

/* Write message to file (Sender) */
void writeMessageToFile(const char *filename)
{
    FILE *fp = fopen(filename, "w");
    if(fp == NULL)
    {
        printf("Error opening file for writing.\n");
        exit(1);
    }

    fprintf(fp, "%s", message);
    fclose(fp);
}

/* Read message from file (Receiver) */
void readMessageFromFile(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("Error opening file for reading.\n");
        exit(1);
    }

    // Clear message buffer before reading
    memset(message, 0, sizeof(message));

    if(fgets(message, sizeof(message), fp) == NULL)
    {
        printf("Error reading message from file.\n");
        fclose(fp);
        exit(1);
    }

    // Remove newline if present
    message[strcspn(message,"\n")] = '\0';

    fclose(fp);
}

/* Convert Decimal to 8-bit Binary */
void decimalToBinary(int number,char binary[])
{
    int i;

    for(i=7;i>=0;i--)
    {
        if((number>>i)&1)
            binary[7-i]='1';
        else
            binary[7-i]='0';
    }

    binary[8]='\0';
}

/* Convert Any Number into Binary */
void numberToBinary(int number,int bits,char binary[])
{
    int i;

    for(i=bits-1;i>=0;i--)
    {
        if((number>>i)&1)
            binary[bits-1-i]='1';
        else
            binary[bits-1-i]='0';
    }

    binary[bits]='\0';
}

/* Create DDCMP Byte Count Frames */
void createFrame()
{
    int length;
    int totalFrames;
    int totalBytes;
    int sourcePort = 5000;
    int destinationPort = 6000;

    int i,j,index=0;
    int currentBytes;

    char binary[9];
    char countBinary[15];

    const char syn1[] = "00010110";
    const char syn2[] = "00010110";
    const char class[] = "00000001";
    const char crc[] = "0000000000000000";

    length = strlen(message);

    totalBytes = length;

    totalFrames = length / FRAME_SIZE;

    if(length % FRAME_SIZE != 0)
        totalFrames++;

    printf("\n==========================================");
    printf("\n      DDCMP BYTE COUNT APPROACH");
    printf("\n==========================================");

    printf("\nTotal Bytes  : %d", totalBytes);
    printf("\nTotal Frames : %d\n", totalFrames);

    for(i=1;i<=totalFrames;i++)
    {
        printf("\n==========================================");
        printf("\nDDCMP FRAME %d",i);
        printf("\n==========================================");

        printf("\nSYN1  : %s",syn1);
        printf("\nSYN2  : %s",syn2);

        printf("\nCLASS : %s",class);

        currentBytes = length-index;

        if(currentBytes > FRAME_SIZE)
            currentBytes = FRAME_SIZE;

        numberToBinary(currentBytes,14,countBinary);

        printf("\nCOUNT : %s",countBinary);
        printf(" (%d Bytes)",currentBytes);

        printf("\n\nHEADER");

        printf("\nSource Port      : %d",sourcePort);
        printf("\nDestination Port : %d",destinationPort);

        printf("\n\nBODY (Binary)");

        for(j=0;j<currentBytes;j++)
        {
            decimalToBinary((unsigned char)message[index],binary);

            printf("\n%c -> %s",message[index],binary);

            index++;

        }

        printf("\n\nCRC : %s",crc);

        printf("\n==========================================\n");

    }

}

/* Receiver Side */
void receiver()
{
    int senderBytes;
    int receiverBytes;

    const char senderSYN[] = "00010110";
    const char receiverSYN[] = "00010110";

    char senderCount[15];
    char receiverCount[15];

    /* Sender and Receiver Byte Count */
    senderBytes = strlen(message);

    /* Assume receiver received all bytes correctly */
    receiverBytes = senderBytes;

    /* Convert Byte Count into 14-bit Binary */
    numberToBinary(senderBytes,14,senderCount);
    numberToBinary(receiverBytes,14,receiverCount);

    printf("\n==========================================");
    printf("\n            RECEIVER SIDE");
    printf("\n==========================================");

    /* SYN Verification */
    printf("\n\nChecking SYN... ");

    if(strcmp(senderSYN,receiverSYN)==0)
        printf("SUCCESS");
    else
    {
        printf("FAILED");
        return;
    }

    /* COUNT Verification */
    printf("\nChecking COUNT... ");

    if(strcmp(senderCount,receiverCount)==0)
        printf("SUCCESS");
    else
    {
        printf("FAILED");
        return;
    }

    /* CRC Verification (Dummy) */
    printf("\nChecking CRC... SUCCESS");

    printf("\n\nFrame Accepted Successfully.");

    printf("\nTotal Bytes Sent     : %d",senderBytes);
    printf("\nTotal Bytes Received : %d",receiverBytes);

    printf("\nTotal Frames Received : %d",
           (receiverBytes + FRAME_SIZE - 1) / FRAME_SIZE);

    printf("\n\nMessage Received : %s",message);

    printf("\n\nTransmission Successful.");

    printf("\n==========================================\n");

}

