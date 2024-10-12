#include "ping.h"

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <random>
#include <iostream>
#include <unistd.h>


icmp_checksum calculate_checksum(const void* data, size_t len){
    auto word16_ptr = reinterpret_cast<const uint16_t*>(data);
    uint32_t sum = 0;
    if (len & 1){
        throw BadIcmpPacketException("Длина ICMP пакета не кратна двум");
    }
    size_t words16_count = len / 2;
    for (int i = 0; i < words16_count; ++i){
        sum += *word16_ptr;
        if (sum & 0xffff0000u){
            sum = (sum >> 16) + (sum & 0xffffu);
        }
    }
    return static_cast<icmp_checksum>(~sum);
}

using socket_descriptor = int;

int get_rand_id(){
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_int_distribution<int> dist(1000,60000);
    return dist(rng);
}

PingInfo ping(const std::string& ip, size_t count_requests, std::chrono::milliseconds response_timeout_ms){
    sockaddr_in in_addr;
    in_addr.sin_family = AF_INET;
    in_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    in_addr.sin_port = htons(0);
    socket_descriptor sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
    if (sock == -1){
        throw PingErrorException("не удалось создать сокет");
    }
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = response_timeout_ms.count() * 1000;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1){
        throw PingErrorException("не удалось установить опцию сокета");
    }
    size_t count_sent_requests = 0;
    size_t count_echo_replies = 0;
    for (int i = 0; i < count_requests; ++i){
        IcmpHeader icmp_packet;
        icmp_packet.type = 8;
        icmp_packet.code = 0;
        icmp_packet.checksum = 0;
        icmp_packet.identifier = get_rand_id();
        icmp_packet.sequence = i;
        icmp_packet.payload = 0;
        icmp_packet.checksum = calculate_checksum(&icmp_packet, sizeof(icmp_packet));
        if (sendto(sock, &icmp_packet, sizeof(icmp_packet), 0, (sockaddr *)&in_addr, sizeof(in_addr)) == -1){
            throw PingErrorException("отправка ICMP-пакета не удалась");
        }
        count_sent_requests++;
        uint8_t receive_buffer[1024];
        auto *icmp_response = (IcmpHeader *)receive_buffer;
        sockaddr_in response_address_info;
        socklen_t response_address_info_size = sizeof(response_address_info);
        int received_bytes = recvfrom(sock, receive_buffer, sizeof(receive_buffer), 0, (sockaddr*)&response_address_info, (socklen_t*)&response_address_info_size);
        if (received_bytes >= 8 && icmp_response->type == 0 
                && icmp_response->code == 0
                && std::string(inet_ntoa(response_address_info.sin_addr)) == ip){
            count_echo_replies++;
            std::cout << "получен ответ от " << std::string(inet_ntoa(response_address_info.sin_addr)) << "\n";
        } else {
            std::cout << ip << " не доступен\n";
        }
        usleep(1000000);
    }
    PingInfo info;
    info.destination_ip = ip;
    info.echo_requests = count_sent_requests;
    info.echo_replies = count_echo_replies;
    return info;
}
