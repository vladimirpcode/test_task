#pragma once
// спасибо github.com/makstamoian за пример реализации

#include <cstdint>
#include <string>
#include <chrono>

struct PingInfo{
    std::string destination_ip;
    size_t echo_requests;
    size_t echo_replies;
};

PingInfo ping(const std::string& ip, size_t count_requests, std::chrono::milliseconds response_timeout_ms);


class BadIcmpPacketException : public std::exception{
public:
    BadIcmpPacketException(const std::string& msg) : m_msg(msg) {}
    const char* what() const noexcept override{return m_msg.c_str();}
private:
    std::string m_msg;
};

class PingErrorException : public std::exception{
public:
    PingErrorException(const std::string& msg) : m_msg(msg) {}
    const char* what() const noexcept override{return m_msg.c_str();}
private:
    std::string m_msg;
};