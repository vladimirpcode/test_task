#include "listen_socket_connection.h"
#include "storage.h"
#include "ping.h"
#include "string_utils.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>

using namespace std::chrono_literals;
using namespace std::string_literals;

void handle_client_connection_task(ListenSocketConnection* server_connection, socket_descriptor client);
void ping_task();

int main(){
    ListenSocketConnection server_connection(65236);
    while (true){
        auto client = server_connection.accept_client_connection();
        std::thread th(handle_client_connection_task, &server_connection, client);
        th.detach();
    }
}

void handle_client_connection_task(ListenSocketConnection* server_connection, socket_descriptor client){
    if (!server_connection){
        return;
    }
    bool stop_flag = false;
    while (!stop_flag){
        try{
            std::string msg = server_connection->receive_msg(client);
            std::vector<std::string> msg_parts = split_by_space(msg);
            if (msg_parts.size() == 0){
                server_connection->send_msg(client, "неизвестная команда: "s + msg + "\n"s);
                continue;
            }
            if (msg_parts[0] == "ping"s){
                if (msg_parts.size() != 3){
                    server_connection->send_msg(client, "у команды ping должно быть два аргумента:\nping <ip> <count>\n"s);
                    continue;
                }
                if (!is_ip_address(msg_parts[1])){
                    server_connection->send_msg(client, "первый аргумент не является IP-адресом"s);
                    continue;
                }
            } else if (msg_parts[0] == "info"s){
                if (msg_parts.size() != 1){
                    server_connection->send_msg(client, "у команды info не должно быть аргументов\n"s);
                    continue;
                }
                Storage& storage = Storage::instance();
                auto statistics_records = storage.get_statistics_records();
                for (auto& [ip, record] : statistics_records){
                    server_connection->send_msg(client, ip + ": "s + std::to_string(record.success_pings_count) + "/"s + std::to_string(record.all_pings_count) + "\n"s);
                }
            } else {
                server_connection->send_msg(client, "неизвестная команда: "s + msg_parts[0] + "\n"s);
            }
        } catch (std::exception e){
            stop_flag = true;
        }
    }
}


void ping_task(){

}
