#include "utils.h"


// Variáveis para controlar a janela de conexão
bool showConnectWindow = false;

// Variavel para controlar as pessoas
bool showPersons = false;

// Running Receive Data
bool running = true;

// Variaveis de input
char ipUrlInput[URL_LEN] = "";

// Variaveis de input
char portInput[PORT_LEN] = "";

// Variaveis de input
char usernameInput[USERNAME_LEN] = "";

// Variaveis de input
char channelId[CHANNEL_LEN] = "";

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

    // Client Socket
    int client_socket = -1;

    // receive data thread
    std::thread receive_data_thread;

    // Receove Mutex
    std::condition_variable cv;
    std::mutex cv_mutex;

    // Loop principal
    while (!glfwWindowShouldClose(window)) {

        ImGui_ImplOpenGL3_NewFrame();

        ImGui_ImplGlfw_NewFrame();

        ImGui::NewFrame();

        // Obter tamanho da janela principal
        int windowWidth, windowHeight;

        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        // Top bar
        topBar(windowWidth, windowHeight, &showConnectWindow);

        // Renderização do viewport de conexão (se aberto)
        if (showConnectWindow) {
            client_socket = connectForm(ipUrlInput, portInput, usernameInput, channelId, &showConnectWindow);

            if (client_socket != -1) {

                showConnectWindow = false;

                showPersons = true;
            }
        }

        if (showPersons) {
            showPersonsWindow(client_socket, windowWidth, windowHeight);

            // Stop the previous communication thread if it exists and is joinable
            if (receive_data_thread.joinable()) {

                running = false; // Signal the thread to stop

                receive_data_thread.join(); // Wait for the thread to finish
            }

            // Start the communication thread before entering showPersonsWindow
            running = true; // Set running to true before starting the thread

            receive_data_thread = std::thread([&]() {
                receiveData(client_socket, &running, &cv, &cv_mutex);
            });
        }



        // Renderização da janela principal
        glfwMakeContextCurrent(window);

        ImGui::EndFrame();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        glfwWaitEvents();
    }

    if (receive_data_thread.joinable()) {
        running = false; // Sinaliza o thread para parar a execução
        receive_data_thread.join(); // Aguarda o thread terminar
    }

    // Limpeza
    ImGui_ImplOpenGL3_Shutdown();

    ImGui_ImplGlfw_Shutdown();

    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}