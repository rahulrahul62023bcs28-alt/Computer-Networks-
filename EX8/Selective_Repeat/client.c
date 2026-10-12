#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define WINDOW 4
#define MAX 20
#define SIZE 20

struct Frame
{
    int seq;
    char data[SIZE];
    int checksum;
};

int checksum(char data[])
{
    int sum = 0;
    int i;

    for(i = 0; data[i] != '\0'; i++)
        sum = sum + data[i];

    return sum;
}

int main()
{
    int sock;

    struct sockaddr_in receiver;
    struct sockaddr_in sender;

    struct Frame frame;

    char message[MAX * SIZE] = "";

    int received[8] = {0};

    int expected = 0;
    int len;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(9000);
    receiver.sin_addr.s_addr = INADDR_ANY;

    bind(sock,
         (struct sockaddr *)&receiver,
         sizeof(receiver));

    printf("Receiver waiting...\n");

    while(1)
    {
        len = sizeof(sender);

        recvfrom(sock,
                 &frame,
                 sizeof(frame),
                 0,
                 (struct sockaddr *)&sender,
                 &len);

        printf("\n-----------------------------\n");

        printf("Frame received\n");
        printf("Sequence Number: %d\n", frame.seq);
        printf("Data: %s\n", frame.data);

        /*
           Check FRAME ERROR
        */

        if(checksum(frame.data) != frame.checksum)
        {
            printf("Frame has ERROR!\n");

            printf("Sending NAK for Frame %d\n",
                   frame.seq);

            char nak[20];

            sprintf(nak, "N%d", frame.seq);

            sendto(sock,
                   nak,
                   strlen(nak) + 1,
                   0,
                   (struct sockaddr *)&sender,
                   len);

            continue;
        }

        /*
           Check if frame is inside
           the receiving window.
        */

        if(frame.seq >= expected &&
           frame.seq < expected + WINDOW)
        {
            if(received[frame.seq] == 0)
            {
                printf("Frame accepted.\n");

                received[frame.seq] = 1;

                /*
                   Store the frame data.
                   Here we simply append it.
                */

                strcat(message, frame.data);
            }
            else
            {
                printf("Duplicate frame.\n");
            }

            /*
               Send ACK.
            */

            char ack[20];

            sprintf(ack, "A%d", frame.seq);

            sendto(sock,
                   ack,
                   strlen(ack) + 1,
                   0,
                   (struct sockaddr *)&sender,
                   len);

            printf("ACK sent for Frame %d\n",
                   frame.seq);

            /*
               Move receiving window
               when expected frame arrives.
            */

            while(received[expected] == 1)
            {
                expected++;

                if(expected == 8)
                    expected = 0;
            }
        }
        else
        {
            /*
               Frame is outside the window.
            */

            printf("Frame is outside receiving window.\n");
        }

        printf("Message so far: %s\n", message);
    }

    close(sock);

    return 0;
}
