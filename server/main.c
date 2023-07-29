#include "dependencies/handlers.h"
#include "dependencies/models.h"
#include "dependencies/errors.h"
#include "dependencies/models.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    for(int i = 0; i < MAX_CHANNELS; i++)
        num_clients[i] = 0;

    int port = atoi(argv[1]);

    int server_socket, client_socket;

    struct sockaddr_in server_address, client_address;

    socklen_t client_address_len = sizeof(server_address);

    pthread_t tid;

    pthread_mutex_init(&mutex, NULL);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    check_socket_error(server_socket, "Creating Socket");

    server_address.sin_family = AF_INET;

    server_address.sin_addr.s_addr = INADDR_ANY;

    server_address.sin_port = htons(port);

    check_socket_related_error(bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)), server_socket, "Binding");

    check_socket_related_error(listen(server_socket, 5), server_socket, "Listening");

    printf("Server listening on port %d\n", port);

    while(1) {

        client_socket = accept(server_socket, NULL, NULL);

        if(client_socket == -1) {

            printf("Error Accepting Connection \n");

            fflush(stdout);
            
            continue;
        }
        
        pthread_mutex_lock(&mutex);

        pthread_mutex_unlock(&mutex);

        int* socket_ptr = (int*) malloc(sizeof(int));

        *socket_ptr = client_socket;

        if (pthread_create(&tid, NULL, handle_client, socket_ptr) != 0) {
            
            printf("Error creating thread.\n");

            fflush(stdout);

            close(client_socket);
            
            free(socket_ptr);
        } else {
            pthread_detach(tid);
        }
    }
}