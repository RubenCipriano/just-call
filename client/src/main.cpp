#include <stdio.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <cstring> // For std::strlen
#include <unistd.h> // For close()
#include <arpa/inet.h> // For inet_addr()
#include <sys/socket.h> // For socket functions
#include <cstdlib> // For atoi

#define URL_LEN 256
#define PORT_LEN 8
#define USERNAME_LEN 128

// Variáveis para controlar a janela de conexão
int connectWindowWidth = 400;
int connectWindowHeight = 200;
bool showConnectWindow = false;

// Callback para redimensionamento da janela GLFW
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool server_connect(char *url, char *port, char *nickname) {
    int portInt = atoi(port);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1) {
        printf("Error creating socket");
        return false;
    }

    // Server address
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(portInt);
    serverAddress.sin_addr.s_addr = inet_addr(url);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        printf("Error connecting to the server");
        close(clientSocket);
        return false;
    }

    // Send the nickname to the server
    size_t nicknameLen = std::strlen(nickname);
    if (send(clientSocket, nickname, nicknameLen, 0) == -1) {
        printf("Error sending data to the server");
        close(clientSocket);
        return false;
    }

    // Close the socket
    close(clientSocket);
    return true;
}

void renderSecondaryViewport(char ipUrlInput[], char portInput[], char usernameInput[]) {
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

    // Input box para o nome de usuário
    ImGui::Text("Nome de Usuário:");
    ImGui::InputText("##username", usernameInput, USERNAME_LEN - 1);

    if(ImGui::Button("Connect")) {
        printf("ENTROU!");
        server_connect(ipUrlInput, portInput, usernameInput);
    }

    ImGui::End();
}

int main() {
    // Inicialização do GLFW e criação da janela principal
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "JustCall", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // Ativar V-Sync
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Inicialização do ImGui com OpenGL
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Variáveis para armazenar o IP/URL, porta e nome de usuário
    char ipUrlInput[URL_LEN] = "";
    char portInput[PORT_LEN] = "";
    char usernameInput[USERNAME_LEN] = "";

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
        ImGui::Begin("Only for ImGuiRe", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

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
            renderSecondaryViewport(ipUrlInput, portInput, usernameInput);
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
