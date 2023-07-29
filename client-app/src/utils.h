#pragma once
#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <cstring> // For std::strlen
#include <cstdlib> // For atoi
#include "network.h"
#include <string>
#include <fcntl.h>

#define URL_LEN 256
#define PORT_LEN 8
#define USERNAME_LEN 128
#define CHANNEL_LEN 3


int connectWindowWidth = 400;
int connectWindowHeight = 200;

std::vector<std::string> messages;
std::vector<std::string> clients;

static char input_text[256] = "";

int connectForm(char ipUrlInput[], char portInput[], char usernameInput[], char channelId[], bool showConnectWindow) {
    // Set the size of the secondary viewport
    ImGui::SetNextWindowSize(ImVec2(connectWindowWidth, connectWindowHeight));

    // Renderizar a interface do viewport de conexão aqui
    ImGui::Begin("Connect", &showConnectWindow);

    // Input boxes para o IP/URL e porta
    ImGui::Text("IP/URL:");
    ImGui::SameLine();
    ImGui::InputText("##ipUrl", ipUrlInput, URL_LEN - 1);

    ImGui::Text("Port:");
    ImGui::SameLine();
    ImGui::InputText("##port", portInput, PORT_LEN - 1, ImGuiInputTextFlags_CharsDecimal);

    ImGui::Text("Channel Id:");
    ImGui::SameLine();
    ImGui::InputText("##Channel Id", channelId, CHANNEL_LEN - 1, ImGuiInputTextFlags_CharsDecimal);

    // Input box para o nome de usuário
    ImGui::Text("Username:");
    ImGui::InputText("##username", usernameInput, USERNAME_LEN - 1);

    int client_socket = -1;

    if (ImGui::Button("Connect")) {
        client_socket = server_connect(ipUrlInput, portInput);

        if (client_socket == -1) {
            ImGui::Text("Error trying to connect to server");
        }
        else {
            // Send Channel Selected
            sendData(client_socket, channelId, strlen(channelId));

            // Send Username
            sendData(client_socket, usernameInput, strlen(usernameInput));

            // Close the window after successful connection
            ImGui::CloseCurrentPopup();
        }       
    }

    ImGui::End();

    return client_socket;
}

void showPersonsWindow(int client_socket, int windowWidth, int windowHeight) {

    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeightWithSpacing()));

    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

    // Begin your new section here
    ImGui::Begin("Persons Section", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

    // Persons Child
    ImGui::BeginChild("PersonsList", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 100), false, ImGuiWindowFlags_HorizontalScrollbar);

    // Your section content goes here
    ImGui::Text("This is a new section below the top bar!");

    // EndChild  PersonsList
    ImGui::EndChild();

    // Adds a separator
    ImGui::Separator();

    // Message Child
    ImGui::BeginChild("MessageList", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);

    // adds messages to text
    for (const auto& message : messages) {
        ImGui::TextUnformatted(message.c_str());
    }

    // EndChild  PersonsList
    ImGui::EndChild();

    // Adds a separator
    ImGui::Separator();

    // Message Child
    ImGui::BeginChild("Input", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    ImGui::InputText("##MessageInput", input_text, sizeof(input_text), ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::SameLine();

    if (ImGui::Button("Send")) {
        // Create Buffer
        char message_send_buffer[sizeof(struct Message)];

        // Create message Pointer
        struct Message* message = (struct Message*)malloc(sizeof(struct Message*));

        message->type = Message;

        strcpy(message->value, input_text);

        serializeMessage(message, message_send_buffer);

        sendData(client_socket, message_send_buffer, sizeof(struct Message));

        // Clear the input text buffer after sending the message
        memset(input_text, 0, sizeof(input_text));
    }

    // EndChild  PersonsList
    ImGui::EndChild();

    ImGui::End(); // End of your new section
}

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

    while (true) {        
        // Data is available to be read from the socket
        int bytesRead = recv(client_socket, message_send_buffer, sizeof(struct Message), 0);

        if (bytesRead > 0) {
            struct Message message;

            deserializeMessage(message_send_buffer, &message);

            switch (message.type) {
                case Message:

                    messages.push_back(message.value);

                    break;
                case Joined:

                    // TODO Add Joined Persons
                    clients.push_back(message.value);

                    break;
            }
        }

        std::this_thread::sleep_for(std::chrono::microseconds(20));

        if(running)
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