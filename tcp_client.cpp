#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

int main() {
    // Server address
    const char* server_ip = "127.0.0.1"; // Localhost for testing
    const int server_port = 8080;

    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // Set up the server address structure
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return -1;
    }

    std::cout << "Connected to server!" << std::endl;

    // Send data
    const char* message = "Hello, server!";
    if (send(sock, message, strlen(message), 0) < 0) {
        std::cerr << "Failed to send message" << std::endl;
        return -1;
    }

    std::cout << "Message sent to server" << std::endl;

    // Receive response
    char buffer[1024] = {0};
    ssize_t bytes_received = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        std::cerr << "Error in receiving data from server" << std::endl;
        return -1;
    }

    std::cout << "Server replied: " << buffer << std::endl;

    // Close the socket
    close(sock);

    return 0;
}


