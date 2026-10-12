#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX 10
#define SIZE 20

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

struct Frame dequeue(struct Queue *q)
{
    struct Frame f;

    f = q->frame[q->front];
    q->front = (q->front + 1) % MAX;
    q->count--;

    return f;
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
    struct Frame frame;
    struct Queue q;

    char message[100];
    char ack[20];

    int i;
    int seq = 0;
    int len;
    int choice;
    int specialFrame;

    initQueue(&q);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(9000);
    receiver.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);

    message[strcspn(message, "\n")] = '\0';

    /*
       Divide message into frames
       and insert into circular queue.
    */

    for(i = 0; i < strlen(message); i = i + SIZE - 1)
    {
        frame.seq = seq;

        strncpy(frame.data, message + i, SIZE - 1);
        frame.data[SIZE - 1] = '\0';

        frame.checksum = checksum(frame.data);

        enqueue(&q, frame);

        seq = 1 - seq;
    }

    printf("\nMessage divided into frames.\n");

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

    i = 0;

    while(q.count > 0)
    {
        frame = dequeue(&q);

        printf("\n-------------------------\n");
        printf("Sending Frame %d\n", i + 1);
        printf("Sequence Number: %d\n", frame.seq);
        printf("Data: %s\n", frame.data);

        /*
           FRAME LOST
        */

        if(choice == 2 && i == specialFrame)
        {
            printf("Frame is LOST!\n");

            specialFrame = -1;
        }
        else
        {
            /*
               FRAME ERROR
            */

            if(choice == 3 && i == specialFrame)
            {
                printf("Frame has ERROR!\n");

                /*
                   Change the data.
                   Checksum remains unchanged.
                */

                frame.data[0] = 'X';

                specialFrame = -1;
            }

            sendto(sock,
                   &frame,
                   sizeof(frame),
                   0,
                   (struct sockaddr *)&receiver,
                   sizeof(receiver));

            printf("Frame sent.\n");
        }

        /*
           Wait for ACK.
           Timeout is 3 seconds.
        */

        struct timeval timeout;

        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        setsockopt(sock,
                   SOL_SOCKET,
                   SO_RCVTIMEO,
                   &timeout,
                   sizeof(timeout));

        len = sizeof(receiver);

        int result = recvfrom(sock,
                              ack,
                              sizeof(ack),
                              0,
                              (struct sockaddr *)&receiver,
                              &len);

        /*
           TIMEOUT
        */

        if(result < 0)
        {
            printf("TIMEOUT!\n");
            printf("Resending Frame %d...\n", i + 1);

            sendto(sock,
                   &frame,
                   sizeof(frame),
                   0,
                   (struct sockaddr *)&receiver,
                   sizeof(receiver));

            recvfrom(sock,
                     ack,
                     sizeof(ack),
                     0,
                     (struct sockaddr *)&receiver,
                     &len);

            printf("ACK received: %s\n", ack);
        }
        else
        {
            printf("ACK received: %s\n", ack);
        }

        i++;
    }

    printf("\n-------------------------\n");
    printf("All frames sent successfully.\n");

    close(sock);

    return 0;
}
