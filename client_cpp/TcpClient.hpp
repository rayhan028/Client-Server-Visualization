#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <string>
#include <optional>
#include <atomic>
#include <thread>
#include <mutex>
#include <queue>

class TcpClient {
public:
    TcpClient(bool debug = false);
    ~TcpClient();

    void connectTo(const std::string& ip, uint16_t port);
    void disconnect();
    bool isConnected() const;

    void sendCommand(const std::string& cmd);
    std::optional<std::string> receive();

    void startAsync();
    void stopAsync();

private:
    int sock_;
    bool debug_;
    std::atomic<bool> running_;

    std::thread recvThread_;
    std::thread sendThread_;

    std::mutex queueMutex_;
    std::queue<std::string> sendQueue_;

    void sendLoop();
    void recvLoop();
    void log(const std::string& msg, const std::string& color="");
};

#endif
