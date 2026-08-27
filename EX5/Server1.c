
    socklen_t len = sizeof(client);

    clientfd = accept(sockfd, (struct sockaddr *)&client, &len);

    if (clientfd < 0)
    {
        printf("Client connection failed\n");
        return;
    }

    printf("Client connected\n");
}

void echoData()
{
    memset(data, 0, sizeof(data));

    recv(clientfd, data, sizeof(data), 0);

    printf("Received from client: %s\n", data);

    send(clientfd, data, strlen(data), 0);

    printf("String sent back to client\n");
}

void closeServer()
{
    close(clientfd);
    close(sockfd);

    printf("Connection closed\n");
}

int main()
{
    createServer();
    bindServer();
    waitClient();
    echoData();
    closeServer();

    return 0;
}
