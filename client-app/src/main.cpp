#include "utils.h"


// Variáveis para controlar a janela de conexão
bool showConnectWindow = false;

void connectForm(char ipUrlInput[], char portInput[], char usernameInput[], char channelId[]) {
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

int main() {
    // Inicialização do GLFW e criação da janela principal
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "JustCall", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // Ativar V-Sync

    // Inicialização do ImGui com OpenGL
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Variáveis para armazenar o IP/URL, porta e nome de usuário
    char ipUrlInput[URL_LEN] = "";
    char portInput[PORT_LEN] = "";
    char usernameInput[USERNAME_LEN] = "";
    char channelId[CHANNEL_LEN] = "";

    // Loop principal
    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Obter tamanho da janela principal
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        // Definir a posição e tamanho da janela principal
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));

        // Criar janela principal (top bar fixa, sem título)
        ImGui::Begin("Only for ImGuiRe", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);

        // Barra superior
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("General")) {
                // Botão "Conectar" na barra superior
                if (ImGui::MenuItem("Conectar")) {
                    // Abrir o viewport de conexão
                    showConnectWindow = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::End(); // Fim da janela principal

        // Renderização do viewport de conexão (se aberto)
        if (showConnectWindow) {
            connectForm(ipUrlInput, portInput, usernameInput, channelId);
        }

        // Renderização da janela principal
        glfwMakeContextCurrent(window);

        ImGui::EndFrame();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Limpeza
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}