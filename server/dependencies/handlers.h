#pragma once

#include "models.h"
#include "utils.h"


int read_client_channel(int socket, char *channel) {

    if(recv(socket, channel, sizeof(channel), 0) <= 0)
        return -1;

    return sizeof(channel);
}


int read_username(int socket, char *username) {

    int username_len;

    if(recv(socket, username, username_len, 0) <= 0)
        return -1;

    return username_len;
}

void remove_client(int client_channel, int client_index) {
    
    pthread_mutex_lock(&mutex);

    char *clientUsername = clients[client_channel][client_index].username;

    close(clients[client_channel][client_index].socket);

    printf("Client %s disconnected from channel: %d\n", clientUsername, client_channel);

    fflush(stdout);

    for(int i = client_index; i < MAX_CLIENTS - 1; i++) {
        clients[client_channel][i] = clients[client_channel][i + 1];
    }

    num_clients[client_channel]--;

    pthread_mutex_unlock(&mutex);
}

void handle_entered_channel(int client_channel, char *username) {

    char buffer[sizeof(struct Message)];

    struct Message message;

    char *send_message;

    snprintf(send_message, sizeof(send_message), "%s: entered!", username, message.value);

    strcpy(message.value, send_message);

    serializeMessage(&message, buffer);

    broadcast_data(client_channel, buffer);

    int size_clients = sizeof(clients[client_channel]);

    printf("%d", size_clients);

    fflush(stdout);
}

void *handle_client(void *arg) {

    char buffer[sizeof(struct Message)];

    struct Message message;

    int client_socket = *(int *) arg;

    char client_channel_string[3];

    if(read_client_channel(client_socket, client_channel_string) <= 0) {

        printf("Not possible to retrieve channel!");

        fflush(stdout);

        return NULL;
    }

    int client_channel = atoi(client_channel_string);

    int client_index = -1;

    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(clients[client_channel][i].socket == NULL) {
            clients[client_channel][i].socket = client_socket;
            num_clients[client_channel]++;
            client_index = i;
            break;
        }
    }

    if(client_index == -1) {
        printf("Channel is Full!");

        fflush(stdout);

        return NULL;
    }

    char username[32];

    if(read_username(clients[client_channel][client_index].socket, username) <= 0) {

        remove_client(client_channel, client_index);

        return NULL;
    }

    strcpy(clients[client_channel][client_index].username, username);

    handle_entered_channel(client_channel, username);

    while(1) {
        char send_message[1024];

        int recvBytes = recv(clients[client_channel][client_index].socket, buffer, sizeof(struct Message), 0);

        if(recvBytes <= 0) {

            remove_client(client_channel, client_index);

            return NULL;

        } else {

            struct Message message;

            deserializeMessage(buffer, &message);

            switch(message.type) {
                case Message:
                    snprintf(send_message, sizeof(send_message), "%s: %s", username, message.value);

                    strcpy(message.value, send_message);
                break;
            }

            serializeMessage(&message, buffer);

            for(int i = 0; i < num_clients[client_channel]; i++) {
                send(clients[client_channel][i].socket, buffer, sizeof(struct Message), 0);
            }

        }
    }
}