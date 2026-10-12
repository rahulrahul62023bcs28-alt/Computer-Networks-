#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5060
#define BUFFER_SIZE 1024
#define KEY 3 // Cipher key shift

// Simple Caesar Cipher Encryption
void encrypt(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] != '\n' && text[i] != '\r') {
            text[i] = text[i] + KEY;
        }
    }
}

// Simple Caesar Cipher Decryption
void decrypt(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] != '\n' && text[i] != '\r') {
            text[i] = text[i] - KEY;
        }
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];

    // 1. Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 2. Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // 3. Get message from user
    printf("Enter message to send: ");
    fgets(message, BUFFER_SIZE, stdin);
    message[strcspn(message, "\n")] = 0; // Remove newline

    // 4. Encrypt message and send
    encrypt(message);
    send(sock, message, strlen(message), 0);
    printf("Encrypted message sent: %s\n", message);

    // 5. Receive encrypted response from server
    read(sock, buffer, BUFFER_SIZE);
    printf("Received response (Encrypted): %s\n", buffer);

    // 6. Decrypt response
    decrypt(buffer);
    printf("Decrypted Response           : %s\n", buffer);

    close(sock);
    return 0;
}
