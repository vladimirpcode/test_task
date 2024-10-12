#pragma once
// спасибо github.com/makstamoian за пример реализации

#include <cstdint>
#include <string>
#include <chrono>

struct IcmpHeader{
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    // специфичные для эхо-ICMP поля
    uint16_t identifier;
    uint16_t sequence;
    uint64_t payload;
};

struct PingInfo{
    std::string destination_ip;
    size_t echo_requests;
    size_t echo_replies;
};

using icmp_checksum = uint16_t;

icmp_checksum calculate_checksum(const void* data, size_t len);
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