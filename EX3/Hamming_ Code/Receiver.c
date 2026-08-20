#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX 2000

char received[MAX];
char original[MAX];
char originalText[MAX];

int dataBits;
int parityBits;
int totalBits;

int syndrome[MAX];

void readFromChannel();
void askForBitChange();
void detectAndCorrect();
void extractOriginalData();
void convertBinaryToText();
void displayReceiver();
int isPowerOfTwo(int n);

int main()
{
    printf("\n========================================");
    printf("\n          HAMMING CODE - RECEIVER");
    printf("\n========================================\n");

    readFromChannel();

    askForBitChange();

    detectAndCorrect();

    extractOriginalData();

    convertBinaryToText();

    displayReceiver();

    return 0;
}


/* =========================================
          CHECK POWER OF TWO
   ========================================= */

int isPowerOfTwo(int n)
{
    if(n == 0)
        return 0;

    return (n & (n - 1)) == 0;
}


/* =========================================
       READ CODE FROM CHANNEL FILE
   ========================================= */

void readFromChannel()
{
    FILE *fp;

    char sentCode[MAX];

    int i;

    fp = fopen("channel.txt", "r");

    if(fp == NULL)
    {
        printf("\nError: channel.txt not found.");
        printf("\nRun the sender program first.\n");

        exit(1);
    }

    /*
       Read data bits, parity bits
       and total bits
    */
    fscanf(fp,
           "%d %d %d",
           &dataBits,
           &parityBits,
           &totalBits);

    /*
       Read Hamming code
    */
    fscanf(fp,
           "%s",
           sentCode);

    fclose(fp);

    /*
       Store code starting from position 1
    */
    for(i = 1; i <= totalBits; i++)
    {
        received[i] = sentCode[i - 1];
    }

    received[totalBits + 1] = '\0';

    printf("\n========================================");
    printf("\n        RECEIVED FROM CHANNEL");
    printf("\n========================================\n");

    printf("Hamming Code : ");

    for(i = 1; i <= totalBits; i++)
    {
        printf("%c", received[i]);
    }

    printf("\n");

    printf("Data Bits    : %d", dataBits);
    printf("\nParity Bits  : %d", parityBits);
    printf("\nTotal Bits   : %d\n", totalBits);
}


/* =========================================
        CHANGE BIT / NO CHANGE
   ========================================= */

void askForBitChange()
{
    char choice;

    int position;
    int i;

    printf("\n========================================");
    printf("\n       CHANNEL ERROR SIMULATION");
    printf("\n========================================\n");

    printf("\nDo you want to change a bit? (y/n): ");

    scanf(" %c", &choice);

    if(choice == 'y' || choice == 'Y')
    {
        printf("\nEnter the bit position to change (1-%d): ",
               totalBits);

        scanf("%d", &position);

        if(position < 1 || position > totalBits)
        {
            printf("\nInvalid bit position.");
            printf("\nNo bit was changed.\n");

            return;
        }

        printf("\nBefore Bit Change : ");

        for(i = 1; i <= totalBits; i++)
        {
            printf("%c", received[i]);
        }

        printf("\n");

        /*
           Flip the selected bit
        */
        if(received[position] == '0')
            received[position] = '1';
        else
            received[position] = '0';

        printf("Changed Bit Position : %d\n",
               position);

        printf("After Bit Change  : ");

        for(i = 1; i <= totalBits; i++)
        {
            printf("%c", received[i]);
        }

        printf("\n");
    }
    else
    {
        printf("\nNo bit changed.");
        printf("\nOriginal transmitted code will be checked.\n");
    }
}


/* =========================================
        DETECT AND CORRECT ERROR
   ========================================= */

void detectAndCorrect()
{
    int i;
    int j;
    int k;

    int parity;

    int errorPosition = 0;

    printf("\n========================================");
    printf("\n      DETECTING ERROR USING SYNDROME");
    printf("\n========================================\n");

    /*
       Check each parity bit
    */
    for(i = 0; i < parityBits; i++)
    {
        int position = (int)pow(2, i);

        parity = 0;

        printf("\nP%d checks positions : ",
               position);

        int first = 1;

        /*
           Check positions covered
           by this parity bit
        */
        for(j = position;
            j <= totalBits;
            j += 2 * position)
        {
            for(k = j;
                k < j + position && k <= totalBits;
                k++)
            {
                if(first)
                {
                    printf("[%d", k);
                    first = 0;
                }
                else
                {
                    printf(",%d", k);
                }

                /*
                   Count number of 1s
                */
                if(received[k] == '1')
                    parity++;
            }
        }

        printf("]");

        /*
           Odd parity means error
        */
        if(parity % 2 != 0)
        {
            syndrome[i] = 1;

            errorPosition += position;

            printf(" -> ERROR (S%d = 1)",
                   i);
        }
        else
        {
            syndrome[i] = 0;

            printf(" -> OK (S%d = 0)",
                   i);
        }
    }

    printf("\n");


    /* =====================================
             DISPLAY SYNDROME
       ===================================== */

    printf("\n----------------------------------------");

    printf("\nSyndrome Word (Binary) : ");

    for(i = parityBits - 1;
        i >= 0;
        i--)
    {
        printf("%d", syndrome[i]);
    }

    printf("\n");

    printf("Syndrome (Decimal)     : %d\n",
           errorPosition);


    /* =====================================
                  NO ERROR
       ===================================== */

    if(errorPosition == 0)
    {
        printf("\nNo Error Detected.\n");
    }


    /* =====================================
                ERROR FOUND
       ===================================== */

    else
    {
        printf("\nError Detected at Bit Position : %d\n",
               errorPosition);

        printf("\nBefore Correction : ");

        for(i = 1; i <= totalBits; i++)
        {
            printf("%c", received[i]);
        }

        printf("\n");

        /*
           Correct the erroneous bit
        */
        if(received[errorPosition] == '0')
            received[errorPosition] = '1';
        else
            received[errorPosition] = '0';

        printf("After Correction  : ");

        for(i = 1; i <= totalBits; i++)
        {
            printf("%c", received[i]);
        }

        printf("\n");
    }
}


/* =========================================
           EXTRACT ORIGINAL DATA
   ========================================= */

void extractOriginalData()
{
    int i;

    int j = 0;

    /*
       Remove parity bit positions
    */
    for(i = 1; i <= totalBits; i++)
    {
        if(!isPowerOfTwo(i))
        {
            original[j++] = received[i];
        }
    }

    original[j] = '\0';
}


/* =========================================
        CONVERT BINARY TO TEXT
   ========================================= */

void convertBinaryToText()
{
    int i;

    int j = 0;

    int value;

    int bit;

    /*
       Every character = 8 bits
    */
    for(i = 0; i < dataBits; i += 8)
    {
        value = 0;

        for(bit = 0; bit < 8; bit++)
        {
            value = value * 2
                   + (original[i + bit] - '0');
        }

        originalText[j++] = (char)value;
    }

    originalText[j] = '\0';
}


/* =========================================
             FINAL OUTPUT
   ========================================= */

void displayReceiver()
{
    int i;

    printf("\n\n========================================");
    printf("\n             FINAL OUTPUT");
    printf("\n========================================");

    printf("\nCorrected Hamming Code : ");

    for(i = 1; i <= totalBits; i++)
    {
        printf("%c", received[i]);
    }

    printf("\n");

    printf("Original Binary Data  : %s\n",
           original);

    printf("Original Text         : %s\n",
           originalText);

    printf("\n========================================\n");
}
