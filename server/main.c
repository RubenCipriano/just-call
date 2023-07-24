#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 100
#define MAX_CHANNELS 100

typedef struct {
    int socket;
    char username[32];
    struct sockaddr_in address;
    int channel_id;
} client_t;

typedef struct {
    int channel_id
} channel_t;

client_t clients[];

int numClients = 0;

pthread_mutex_t mutex;

int read_username(int socket, char *username) {
    int username_len;
    if(recv(socket, &username_len, sizeof(username_len), 0) <= 0)
        return -1;

    if(recv(socket, username, username_len, 0) <= 0)
        return -1;

    return username_len;
}

void check_socket_related_error(int validation, int server_socket, const char *error_message) {
    if(validation == -1) {
        printf("ERROR: %s\n", error_message);
        close(server_socket);
        exit(EXIT_FAILURE);
    }
}

void check_socket_error(int socket, const char *error_message) {
    if(socket == -1) {
        printf("ERROR: %s\n", error_message);
        exit(EXIT_FAILURE);
    }
}

void remove_client(int client_index) {
    
    pthread_mutex_lock(&mutex);

    int channelId = clients[client_index].channel_id;

    char *clientUsername = clients[client_index].username;

    close(clients[client_index].socket);

    printf("Client %s disconnected from channel: %d", clientUsername, channelId);

    for(int i = client_index; i < numClients - 1; i++) {
        clients[i] = clients[i + 1];
    }

    numClients--;

    pthread_mutex_unlock(&mutex);
}

void *handle_client(void *arg) {
    int client_index = *(int *) arg;

    char username[32];

    char buffer[256];

    if(read_username(clients[numClients].socket, username) <= 0) {

        remove_client(client_index);

        return NULL;
    }

    strcpy(clients[client_index].username, username);

    while(1) {

        int recvBytes = recv(clients[client_index].socket, buffer, sizeof(buffer), 0);

        if(recvBytes <= 0) {

            remove_client(client_index);

            return NULL;
        }
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);

    int server_socket, client_socket;

    struct sockaddr_in server_address, client_address;

    socklen_t client_address_len = sizeof(client_address);

    pthread_t tid;

    pthread_mutex_init(&mutex, NULL);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    check_socket_error(server_socket, "Creating Socket");

    memset(&server_address, 0, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
    
    check_socket_related_error(bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)), server_socket, "Binding");

    check_socket_related_error(listen(server_socket, 5), server_socket, "Listening");

    printf("Server listening on port %d\n", port);

    while(1) {
        client_socket = accept(server_socket, (struct sockaddr *) &client_address, sizeof(client_address));

        if(client_socket == -1) {
            printf("Error Accepting Connection \n");
            continue;
        }

        pthread_mutex_lock(&mutex);

        if(numClients < MAX_CLIENTS) {
            clients[numClients].socket = client_socket;
            clients[numClients].address = client_address;
            clients[numClients].channel_id = 0;
            numClients++;

            printf("New client connected");
        } else {
            printf("Client Rejected: Max number of clients reached!\n");
            close(client_socket);
        }

        pthread_mutex_unlock(&mutex);

        int *arg = malloc(sizeof(int));

        *arg = numClients - 1;

        pthread_create(&tid, NULL, handle_client, arg);

        pthread_detach(tid);
    }
}