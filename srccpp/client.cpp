#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

// Define the tick rate (ticks per second)
const int TICK_RATE = 30;
const double TICK_INTERVAL = 1.0 / TICK_RATE;

// Game state
struct GameState {
    // Your game state data here
};

// Function to update the game state
void updateGameState(GameState& game_state) {
    // Your game logic here
    // Example: game_state.player_position += game_state.player_velocity;
}

// Function to send updates to a connected client
void sendUpdatesToClient(const GameState& game_state, int client_socket) {
    // Your code to send game state updates to the connected client here
    // Example: send game state data to the connected client
    std::string game_state_data = "Your game state data";
    send(client_socket, game_state_data.c_str(), game_state_data.size(), 0);
}

// Function to handle a client connection in a separate thread
void handleClientConnection(int client_socket, GameState& game_state) {
    std::cout << "Accepted connection on socket " << client_socket << std::endl;

    while (true) {
        char buffer[1024];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytes_received <= 0) {
            // Client disconnected or an error occurred
            break;
        }

        // Process client input if needed
        // Example: Handle player input

        // Send game state updates to the client
        sendUpdatesToClient(game_state, client_socket);
    }

    close(client_socket);
    std::cout << "Client on socket " << client_socket << " disconnected." << std::endl;
}

int main() {
    // Initialize the game state
    GameState game_state;

    // Create a server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Bind the socket to a specific host and port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345); // Replace with your desired port
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr) ) == -1) {
        perror("Bind failed");
        close(server_socket);
        return 1;
    }

    // Start listening for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listen failed");
        close(server_socket);
        return 1;
    }

    std::cout << "Server is listening for clients." << std::endl;

    std::vector<std::thread> client_threads;

    // Main game loop
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_size);

        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        // Create a new thread to handle the client connection and pass game_state
        client_threads.emplace_back(handleClientConnection, client_socket, std::ref(game_state));
        
        // Record the current time before processing the tick
        auto start_time = std::chrono::steady_clock::now();

        // Update game state and handle player input
        updateGameState(game_state);

        // Calculate the time taken to process the tick
        auto end_time = std::chrono::steady_clock::now();
        double elapsed_time = std::chrono::duration<double>(end_time - start_time).count();

        // Sleep to maintain the tick rate
        if (elapsed_time < TICK_INTERVAL) {
            std::this_thread::sleep_for(std::chrono::duration<double>(TICK_INTERVAL - elapsed_time));
        }
    }

    // Close all client sockets (threads will take care of this)
    for (std::thread& thread : client_threads) {
        thread.join();
    }

    // Close the server socket
    close(server_socket);

    return 0;
}
