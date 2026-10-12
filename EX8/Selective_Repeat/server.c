#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define MAX 20
#define SIZE 20
#define WINDOW 4

struct Frame
{
    int seq;
    char data[SIZE];
    int checksum;
};

struct Queue
{
    struct Frame frame[MAX];
    int front;
    int rear;
    int count;
};

void initQueue(struct Queue *q)
{
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

void enqueue(struct Queue *q, struct Frame f)
{
    q->rear = (q->rear + 1) % MAX;
    q->frame[q->rear] = f;
    q->count++;
}

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
    struct Queue q;

    struct Frame frame;
    struct Frame temp;

    char message[200];
    char response[20];

    int acked[MAX] = {0};

    int totalFrames = 0;
    int base = 0;
    int next = 0;

    int i;
    int len;
    int choice;
    int specialFrame;

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(9000);
    receiver.sin_addr.s_addr = inet_addr("127.0.0.1");

    initQueue(&q);

    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);

    message[strcspn(message, "\n")] = '\0';

    /*
       Divide message into frames
       and store them in circular queue.
    */

    for(i = 0; i < strlen(message); i = i + SIZE - 1)
    {
        frame.seq = totalFrames % 8;

        strncpy(frame.data, message + i, SIZE - 1);
        frame.data[SIZE - 1] = '\0';

        frame.checksum = checksum(frame.data);

        enqueue(&q, frame);

        totalFrames++;
    }

    printf("\nTotal frames = %d\n", totalFrames);

    printf("\n1. Timeout\n");
    printf("2. Frame Lost\n");
    printf("3. Frame Error\n");
    printf("4. Normal Transmission\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    specialFrame = -1;

    if(choice == 2)
    {
        printf("Enter frame number to lose: ");
        scanf("%d", &specialFrame);
        specialFrame--;
    }

    if(choice == 3)
    {
        printf("Enter frame number to make error: ");
        scanf("%d", &specialFrame);
        specialFrame--;
    }

    /*
       Selective Repeat:
       Send frames within the window.
    */

    while(base < totalFrames)
    {
        /*
           Send frames while window is not full.
        */

        while(next < totalFrames && next < base + WINDOW)
        {
            frame = q.frame[(q.front + next) % MAX];

            /*
               FRAME LOST
            */

            if(choice == 2 && next == specialFrame)
            {
                printf("\nFrame %d is LOST!\n", next + 1);

                specialFrame = -1;
            }
            else
            {
                temp = frame;

                /*
                   FRAME ERROR
                */

                if(choice == 3 && next == specialFrame)
                {
                    printf("\nFrame %d has ERROR!\n", next + 1);

                    /*
                       Change data but keep old checksum.
                    */

                    temp.data[0] = 'X';

                    specialFrame = -1;
                }

                printf("\nSending Frame %d\n", next + 1);
                printf("Sequence Number: %d\n", temp.seq);
                printf("Data: %s\n", temp.data);

                sendto(sock,
                       &temp,
                       sizeof(temp),
                       0,
                       (struct sockaddr *)&receiver,
                       sizeof(receiver));
            }

            next++;
        }

        /*
           Wait for ACK or NAK.
           Timeout = 3 seconds.
        */

        fd_set readfds;
        struct timeval timeout;

        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);

        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        int result = select(sock + 1,
                            &readfds,
                            NULL,
                            NULL,
                            &timeout);

        /*
           TIMEOUT
        */

        if(result == 0)
        {
            printf("\nTIMEOUT!\n");

            /*
               Selective Repeat:
               resend only the unacknowledged frame.
            */

            for(i = base; i < next; i++)
            {
                if(acked[i] == 0)
                {
                    frame = q.frame[(q.front + i) % MAX];

                    printf("Resending Frame %d\n", i + 1);

                    sendto(sock,
                           &frame,
                           sizeof(frame),
                           0,
                           (struct sockaddr *)&receiver,
                           sizeof(receiver));
                }
            }
        }
        else
        {
            len = sizeof(receiver);

            recvfrom(sock,
                     response,
                     sizeof(response),
                     0,
                     (struct sockaddr *)&receiver,
                     &len);

            int number = atoi(response + 1);

            /*
               ACK
            */

            if(response[0] == 'A')
            {
                printf("ACK received for Frame %d\n",
                       number + 1);

                acked[number] = 1;
            }

            /*
               NAK
            */

            if(response[0] == 'N')
            {
                printf("NAK received for Frame %d\n",
                       number + 1);

                frame = q.frame[(q.front + number) % MAX];

                printf("Resending Frame %d\n",
                       number + 1);

                sendto(sock,
                       &frame,
                       sizeof(frame),
                       0,
                       (struct sockaddr *)&receiver,
                       sizeof(receiver));
            }

            /*
               Move the window forward.
               Only when the first frame is ACKed.
            */

            while(base < totalFrames && acked[base] == 1)
            {
                base++;
            }
        }
    }

    printf("\n-----------------------------\n");
    printf("All frames transmitted successfully.\n");

    close(sock);

    return 0;
}
