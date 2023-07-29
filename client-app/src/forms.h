#pragma once

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "network.h"

#define URL_LEN 256
#define PORT_LEN 8
#define USERNAME_LEN 128
#define CHANNEL_LEN 3


int connectWindowWidth = 400;
int connectWindowHeight = 200;

std::vector<std::string> messages;
std::vector<Client> clients;

static char input_text[256] = "";

void topBar(int windowWidth, int windowHeight, bool *showConnectWindow) {
    // Definir a posição e tamanho da janela principal
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

    // Criar janela principal (top bar fixa, sem título)
    ImGui::Begin("Top Bar Section", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

    // Barra superior
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("General")) {
            // Botão "Conectar" na barra superior
            if (ImGui::MenuItem("Conectar")) {
                // Abrir o viewport de conexão
                (*showConnectWindow) = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End(); // Fim da janela principal
}

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

            // Set Input Text of send button to null
            memset(input_text, 0, sizeof(input_text));

            // Clear all messages
            messages.clear();

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

    for (auto client : clients) {

        ImGui::TextUnformatted(client.username);
    }

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