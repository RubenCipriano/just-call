#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <cstring> // For std::strlen
#include <cstdlib> // For atoi
#include "network.h"

#define URL_LEN 256
#define PORT_LEN 8
#define USERNAME_LEN 128
#define CHANNEL_LEN 3


int connectWindowWidth = 400;
int connectWindowHeight = 200;

void connectForm(char ipUrlInput[], char portInput[], char usernameInput[], char channelId[], bool showConnectWindow) {
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

    if (ImGui::Button("Connect")) {
        printf("ENTROU!");
        int client_socket = server_connect(ipUrlInput, portInput);
        sendData(client_socket, channelId);
        sendData(client_socket, usernameInput);
    }

    ImGui::End();
}