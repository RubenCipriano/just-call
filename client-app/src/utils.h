#pragma once

#include "forms.h"
#include "network.h"



void setSocketNonBlocking(SOCKET socket) {
    #ifdef _WIN32
        u_long mode = 1; // 1 for non-blocking, 0 for blocking
        ioctlsocket(socket, FIONBIO, &mode);
    #else
        // For Unix-like systems, use the fcntl approach
        int flags = fcntl(socket, F_GETFL, 0);
        fcntl(socket, F_SETFL, flags | O_NONBLOCK);
    #endif
}

void receiveData(int client_socket, bool* running, std::condition_variable* cv, std::mutex* cv_mutex) {

    setSocketNonBlocking(client_socket);

    char message_send_buffer[sizeof(struct Message)];

    char client_send_buffer[sizeof(struct Client)];

    while (true) {
        // Data is available to be read from the socket
        int bytesRead = recv(client_socket, message_send_buffer, sizeof(struct Message), 0);

        if (bytesRead > 0) {
            struct Message* message = (struct Message*)malloc(sizeof(struct Message));

            deserializeMessage(message_send_buffer, message);

            switch (message->type) {
            case Message:

                messages.push_back(message->value);

                break;
            case Joined:

                // TODO Add Joined Persons
                messages.push_back(message->value);

                char client_buffer[sizeof(struct Client)];


                bytesRead = recv(client_socket, client_buffer, sizeof(struct Client), 0);

                if (bytesRead > 0) {
                    struct Client client;

                    deserializeClient(client_buffer, &client);

                    clients.push_back(client);
                }

                break;
            case ClientList:

                char num_of_clients[sizeof(int)];

                int num_clients = 0;

                bytesRead = recv(client_socket, num_of_clients, sizeof(int), 0);

                if (bytesRead > 0) {

                    memcpy(&num_clients, num_of_clients, sizeof(int));

                    struct Client* client_list = (struct Client*)malloc(sizeof(struct Client) * num_clients);

                    deserializeClients(message->value, client_list, num_clients);

                    for (int i = 0; i < num_clients; i++) {

                        clients.push_back(client_list[i]);
                    }
                }

                break;
            }

            free(message);
        }

        bytesRead = recv(client_socket, message_send_buffer, sizeof(struct Message), 0);

        std::this_thread::sleep_for(std::chrono::microseconds(20));

        if (running)
        {
            std::unique_lock<std::mutex> lock(*cv_mutex);
            if (!(*running)) {
                // Notify the main thread that the communication thread is done
                cv->notify_all();
                break;
            }
        }
    }
}