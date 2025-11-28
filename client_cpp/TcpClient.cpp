#include "TcpClient.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>

namespace Color {
    constexpr auto GREEN = "\033[1;32m";
    constexpr auto CYAN  = "\033[1;36m";
    constexpr auto RED   = "\033[1;31m";
    constexpr auto YELLOW= "\033[1;33m";
    constexpr auto RESET = "\033[0m";
}

TcpClient::TcpClient(bool debug) : sock_(-1), debug_(debug), running_(false) {}
TcpClient::~TcpClient() { stopAsync(); disconnect(); }

void TcpClient::connectTo(const std::string& ip, uint16_t port) {
    disconnect();

    sock_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_ < 0) throw std::runtime_error("Socket creation failed");

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
        throw std::runtime_error("Invalid IP");

    if (connect(sock_, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("Connection failed");

    if (debug_) log("Connected to server", Color::GREEN);
}

void TcpClient::disconnect() {
    if (sock_ != -1) {
        close(sock_);
        sock_ = -1;
        log("Disconnected", Color::RED);
    }
}

bool TcpClient::isConnected() const { return sock_ != -1; }

void TcpClient::sendCommand(const std::string& cmd) {
    std::lock_guard<std::mutex> lock(queueMutex_);
    sendQueue_.push(cmd);
}

std::optional<std::string> TcpClient::receive() {
    char buffer[512];
    ssize_t bytes = recv(sock_, buffer, sizeof(buffer)-1, 0);
    if (bytes <= 0) return std::nullopt;
    buffer[bytes] = '\0';
    return std::string(buffer);
}

void TcpClient::startAsync() {
    if (running_) return;
    running_ = true;
    recvThread_ = std::thread(&TcpClient::recvLoop, this);
    sendThread_ = std::thread(&TcpClient::sendLoop, this);
}

void TcpClient::stopAsync() {
    if (!running_) return;
    running_ = false;
    if (recvThread_.joinable()) recvThread_.join();
    if (sendThread_.joinable()) sendThread_.join();
}

void TcpClient::sendLoop() {
    while (running_) {
        std::string cmd;
        {
            std::lock_guard<std::mutex> lock(queueMutex_);
            if (!sendQueue_.empty()) {
                cmd = sendQueue_.front();
                sendQueue_.pop();
            }
        }
        if (!cmd.empty() && isConnected()) {
            std::string msg = cmd + "\n";
            send(sock_, msg.c_str(), msg.size(), 0);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void TcpClient::recvLoop() {
    while (running_ && isConnected()) {
        auto resp = receive();
        if (resp) log("Server: " + *resp, Color::CYAN);
        else std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void TcpClient::log(const std::string& msg, const std::string& color) {
    if (color.empty()) std::cout << msg << "\n";
    else std::cout << color << msg << Color::RESET << "\n";
}
