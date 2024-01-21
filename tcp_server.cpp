#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    const int port = 8080;

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        std::cerr << "setsockopt" << std::endl;
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // or use inet_pton
    address.sin_port = htons(port);

    // Bind the socket to the network address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        std::cerr << "bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        std::cerr << "listen" << std::endl;
        exit(EXIT_FAILURE);
    }

    while (true) {
        std::cout << "Waiting for connections..." << std::endl;

        // Accept an incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            std::cerr << "accept" << std::endl;
            continue; // Continue to the next iteration of the loop
        }

        // Read message from client
        char buffer[1024] = {0};
        ssize_t bytes_read = read(new_socket, buffer, 1024);
        if (bytes_read > 0) {
            std::cout << "Message from client: " << buffer << std::endl;
            // Echo back the message
            send(new_socket, buffer, bytes_read, 0);
        }

        // Close the client socket
        close(new_socket);
    }

    // Close the server socket (this line may never be reached if the server runs indefinitely)
    close(server_fd);

    return 0;
}


