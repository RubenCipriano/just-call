#pragma once

#include "models.h"

void check_socket_related_error(int validation, int server_socket, const char *error_message) {
    if(validation == -1) {

        printf("ERROR: %s\n", error_message);

        fflush(stdout);

        close(server_socket);

        exit(EXIT_FAILURE);
    }
}

void check_socket_error(int socket, const char *error_message) {
    
    if(socket == -1) {

        printf("ERROR: %s\n", error_message);

        fflush(stdout);

        exit(EXIT_FAILURE);
    }
}