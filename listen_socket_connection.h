#pragma once

#include <cstdint>
#include <exception>
#include <string>

using tcp_port = uint16_t;
using socket_descriptor = int;

class ListenSocketConnection{
public:
    ListenSocketConnection(tcp_port port_number);
    ~ListenSocketConnection();
    socket_descriptor accept_client_connection();
    std::string receive_msg(socket_descriptor client);
    void send_msg(socket_descriptor client, const std::string msg);
private:
    static constexpr inline size_t RECEIVE_BUFFER_SIZE = 1024;
    static constexpr inline size_t MAX_CLIENT_CONNECTIONS = 1;
    tcp_port m_port_number;
    socket_descriptor m_socket_descriptor;
    char m_receive_buffer[RECEIVE_BUFFER_SIZE];
};

class ServerConnectionErrorException : public std::exception{
public:
    ServerConnectionErrorException(const std::string& msg) : m_msg(msg) {}
    const char* what() const noexcept override{return m_msg.c_str();}
private:
    std::string m_msg;
};

class ClientConnectionErrorException : public std::exception{
public:
    ClientConnectionErrorException(const std::string& msg) : m_msg(msg) {}
    const char* what() const noexcept override{return m_msg.c_str();}
private:
    std::string m_msg;
};

class SendRecvErrorException : public std::exception{
public:
    SendRecvErrorException(const std::string& msg) : m_msg(msg) {}
    const char* what() const noexcept override{return m_msg.c_str();}
private:
    std::string m_msg;
};