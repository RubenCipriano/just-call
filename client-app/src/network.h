#pragma once
#include <stdio.h>
#include <string.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include "models.h"

#ifdef _WIN32
    #include <winsock2.h> // For sockets on Windows
    #pragma comment(lib, "ws2_32.lib") // Link with the Winsock library
#else
    #include <unistd.h> // For close() on Unix
    #include <arpa/inet.h> // For inet_addr() on Unix
    #include <sys/socket.h> // For socket functions on Unix
#endif

int server_connect(char* url, char* port) {
    int portInt = atoi(port);

    #ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            printf("Failed to initialize Winsock");
            return -1;
        }
    #endif

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1) {
        printf("Error creating socket");
        #ifdef _WIN32
                WSACleanup(); // Cleanup Winsock on Windows
        #endif
        return -1;
    }

    // Server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(portInt);
    serverAddress.sin_addr.s_addr = inet_addr(url);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1) {
        printf("Error connecting to the server");
        #ifdef _WIN32
                WSACleanup(); // Cleanup Winsock on Windows
        #endif
        #ifdef _WIN32
                closesocket(clientSocket); // Close socket on Windows
        #else
                close(clientSocket); // Close socket on Unix
        #endif
        return -1;
    }

    return clientSocket;
}

bool sendData(int clientSocket, const char *data, int dataLength) {
    int status = send(clientSocket, data, dataLength, 0);

    if (status == -1)
        return false;
    return true;
}

// Fun��o para serializar a estrutura Message em um buffer de bytes
void serializeClient(const struct Client* message, char* buffer) {
    memcpy(buffer, message, sizeof(struct Client));
}

// Fun��o para desserializar um buffer de bytes para a estrutura Message
void deserializeClient(const char* buffer, struct Client* client) {
    memcpy(client, buffer, sizeof(struct Client));
}

// Fun��o para desserializar um buffer de bytes para a estrutura Message
void deserializeClients(const char* buffer, struct Client* clients, int num_clients) {
    int size_of_struct = sizeof(struct Client);

    for (int i = 0; i < num_clients; i++) {
        int offset = size_of_struct * i;

        memcpy(clients + i, buffer + offset, sizeof(struct Client));
    }
}

// Fun��o para serializar a estrutura Message em um buffer de bytes
void serializeMessage(const struct Message* message, char* buffer) {
    memcpy(buffer, message, sizeof(struct Message));
}

// Fun��o para desserializar um buffer de bytes para a estrutura Message
void deserializeMessage(const char* buffer, struct Message* message) {
    memcpy(message, buffer, sizeof(struct Message));
}