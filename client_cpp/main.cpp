#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        return 1;
    }

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed\n";
        return 1;
    }

    std::cout << "Connected to server!\n";
    std::cout << "Commands: PING, TIME, EXIT\n\n";

    while (true) {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);

        input += "\n";
        send(sock, input.c_str(), input.size(), 0);

        char buffer[1024] = {0};
        ssize_t bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);

        if (bytes <= 0) {
            std::cout << "Server disconnected.\n";
            break;
        }

        std::cout << "Server: " << buffer;

        if (input == "EXIT\n")
            break;
    }

    close(sock);
    return 0;
}
