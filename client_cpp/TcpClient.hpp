#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <string>
#include <vector>
#include <chrono>

class TcpClient {
public:
    TcpClient(bool debug = false);
    ~TcpClient();

    void connectTo(const std::string& ip, uint16_t port);
    void sendLine(const std::string& line);
    std::string recvLine();
    bool isConnected() const { return connected; }

private:
    int sock;
    bool debug;
    bool connected;

    void closeSocket();
    std::string timestamp() const;
};

#endif
