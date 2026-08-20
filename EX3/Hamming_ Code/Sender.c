#include <stdio.h>
#include <math.h>
#include <string.h>

#define MAX 2000

char data[MAX];
char hamming[MAX];

int dataBits;
int parityBits;
int totalBits;

void getData();
void convertToBinary(char text[]);
void calculateParityBits();
void placeDataBits();
void calculateParityBitsValue();
void displayParityCheckingPositions();
void displaySender();
void writeToChannel();
int isPowerOfTwo(int n);

int main()
{
    printf("\n========================================");
    printf("\n          HAMMING CODE - SENDER");
    printf("\n========================================\n");

    getData();

    calculateParityBits();

    placeDataBits();

    calculateParityBitsValue();

    displayParityCheckingPositions();

    displaySender();

    writeToChannel();

    return 0;
}


/* =========================================
              GET DATA
   ========================================= */

void getData()
{
    FILE *fp;
    char text[MAX];

    fp = fopen("input.txt", "r");

    if(fp == NULL)
    {
        printf("Error opening input.txt\n");
        return;
    }

    fgets(text, MAX, fp);

    text[strcspn(text, "\n")] = '\0';

    fclose(fp);

    printf("\nInput from input.txt : %s\n", text);

    convertToBinary(text);

    dataBits = strlen(data);

    printf("Binary Data           : %s\n", data);
    printf("Number of Data Bits   : %d\n", dataBits);
}


/* =========================================
          CONVERT TEXT TO BINARY
   ========================================= */

void convertToBinary(char text[])
{
    int i;
    int bit;
    int j = 0;
    int value;

    for(i = 0; text[i] != '\0'; i++)
    {
        value = (unsigned char)text[i];

        for(bit = 7; bit >= 0; bit--)
        {
            if(value & (1 << bit))
                data[j++] = '1';
            else
                data[j++] = '0';
        }
    }

    data[j] = '\0';
}


/* =========================================
       CALCULATE PARITY BITS
   ========================================= */

void calculateParityBits()
{
    parityBits = 0;

    while(pow(2, parityBits) <
          (dataBits + parityBits + 1))
    {
        parityBits++;
    }

    totalBits = dataBits + parityBits;

    printf("Parity Bits Required : %d\n", parityBits);
    printf("Total Hamming Bits   : %d\n", totalBits);
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
          PLACE DATA BITS
   ========================================= */

void placeDataBits()
{
    int i;
    int j = 0;

    for(i = 1; i <= totalBits; i++)
    {
        if(isPowerOfTwo(i))
        {
            hamming[i] = '0';
        }
        else
        {
            hamming[i] = data[j++];
        }
    }
}


/* =========================================
       CALCULATE PARITY VALUES
   ========================================= */

void calculateParityBitsValue()
{
    int i;
    int j;
    int k;
    int parity;

    for(i = 0; i < parityBits; i++)
    {
        int position = (int)pow(2, i);

        parity = 0;

        for(j = position;
            j <= totalBits;
            j += 2 * position)
        {
            for(k = j;
                k < j + position && k <= totalBits;
                k++)
            {
                /*
                   Do not count the parity bit
                   while calculating its value
                */
                if(k != position)
                {
                    if(hamming[k] == '1')
                        parity++;
                }
            }
        }

        /*
           Even parity
        */
        hamming[position] =
            (parity % 2 == 0) ? '0' : '1';
    }
}


/* =========================================
     DISPLAY PARITY CHECKING POSITIONS
   ========================================= */

void displayParityCheckingPositions()
{
    int i;
    int j;
    int k;

    printf("\n========================================");
    printf("\n       PARITY CHECKING POSITIONS");
    printf("\n========================================\n");

    for(i = 0; i < parityBits; i++)
    {
        int position = (int)pow(2, i);

        printf("\nP%d checks positions : [",
               position);

        int first = 1;

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
                    printf("%d", k);
                    first = 0;
                }
                else
                {
                    printf(",%d", k);
                }
            }
        }

        printf("]\n");
    }
}


/* =========================================
          DISPLAY SENDER OUTPUT
   ========================================= */

void displaySender()
{
    int i;
    int dataNumber = 1;

    printf("\n========================================");
    printf("\n             SENDER OUTPUT");
    printf("\n========================================\n");

    printf("Generated Hamming Code : ");

    for(i = 1; i <= totalBits; i++)
    {
        printf("%c", hamming[i]);
    }

    printf("\n\n");

    /* Display bit positions correctly */
    printf("Bit Position : ");

    for(i = 1; i <= totalBits; i++)
    {
        printf("%3d ", i);
    }

    printf("\n");

    /* Display P1, P2, D1, P4, D2... */
    printf("Bit Label    : ");

    dataNumber = 1;

    for(i = 1; i <= totalBits; i++)
    {
        if(isPowerOfTwo(i))
        {
            printf("P%-2d ", i);
        }
        else
        {
            printf("D%-2d ", dataNumber);
            dataNumber++;
        }
    }

    printf("\n");

    /* Display actual bit values */
    printf("Bit Value    : ");

    for(i = 1; i <= totalBits; i++)
    {
        printf(" %2c ", hamming[i]);
    }

    printf("\n");
}

/* =========================================
        WRITE TO CHANNEL FILE
   ========================================= */

void writeToChannel()
{
    FILE *fp;
    int i;

    fp = fopen("channel.txt", "w");

    if(fp == NULL)
    {
        printf("Error opening channel.txt\n");
        return;
    }

    fprintf(fp,
            "%d %d %d\n",
            dataBits,
            parityBits,
            totalBits);

    for(i = 1; i <= totalBits; i++)
    {
        fprintf(fp, "%c", hamming[i]);
    }

    fclose(fp);

    printf("\nHamming Code written to channel.txt successfully.\n");
}
