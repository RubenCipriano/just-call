#include "models.h"

void send_data(int client_socket, char *buffer) {
    send(client_socket, buffer, sizeof(struct Message), 0);
}

void broadcast_data(int client_channel, char *buffer) {
    for(int i = 0; i < num_clients[client_channel]; i++) {
        send_data(clients[client_channel][i].socket, buffer);
    }
}

void serializeClient(client_t * message, char * buffer) {
    memcpy(buffer, message, sizeof(client_t));
}

void deserializeClient(char * buffer, client_t * message) {
    memcpy(message, buffer, sizeof(client_t));
}

void serializeMessage(struct Message * message, char * buffer) {
    memcpy(buffer, message, sizeof(struct Message));
}

void deserializeMessage(char * buffer, struct Message * message) {
    memcpy(message, buffer, sizeof(struct Message));
}