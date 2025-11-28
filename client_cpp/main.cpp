#include <iostream>
#include <csignal>
#include "TcpClient.hpp"

static TcpClient* gClient = nullptr;

void handleSignal(int) {
    if (gClient) gClient->stopAsync();
    std::cout << "\n[!] Exiting...\n";
    std::exit(0);
}

int main() {
    signal(SIGINT, handleSignal);
    TcpClient client(true);
    gClient = &client;

    // Connect with retry
    while (true) {
        try {
            client.connectTo("127.0.0.1", 5000);
            break;
        } catch (...) {
            std::cout << "[!] Retry in 2s...\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    client.startAsync();
    std::cout << "Commands: PING, TIME, EXIT\n";

    while (true) {
        std::cout << "> ";
        std::string cmd;
        std::getline(std::cin, cmd);

        if (cmd.empty()) continue;

        client.sendCommand(cmd);
        if (cmd == "EXIT") break;
    }

    client.stopAsync();
    return 0;
}
