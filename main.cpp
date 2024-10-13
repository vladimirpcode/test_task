#include "listen_socket_connection.h"
#include "storage.h"
#include "ping.h"
#include "string_utils.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <unistd.h>
#include <filesystem>
#include <fstream>

using namespace std::chrono_literals;
using namespace std::string_literals;

void handle_client_connection_task(ListenSocketConnection* server_connection, socket_descriptor client);
void ping_task(const std::string& ip, int count_echo_requests);
bool is_other_process_running(int my_pid, const std::string& my_process_name);

int main(){
    std::cout << "test-pinger запущен\n";
    if(is_other_process_running(getpid(), "test-pinger"s)) {
        std::cout << "запущена другая копия процесса\n";
        exit(0);
    }

    if (daemon(0, 1) == -1){
        std::cout << "не удалось запустить в режиме фонового процесса\n";
        exit(0);
    }
    ListenSocketConnection server_connection(65236);
    while (true){
        auto client = server_connection.accept_client_connection();
        std::thread th(handle_client_connection_task, &server_connection, client);
        th.detach();
    }
}

bool is_other_process_running(int my_pid, const std::string& my_process_name){
    for (auto const& dir_entry : std::filesystem::directory_iterator("/proc")){
        try
        {
            std::string proc_dir = dir_entry.path().filename();
            if (!is_int_number(proc_dir)){
                continue;
            }
            if (std::stoi(proc_dir) == my_pid){
                continue;
            }
            std::ifstream fin(dir_entry.path().string() + "/cmdline"s);
            std::string line;
            std::getline(fin, line);
            auto parts = split(line, '/');
            if (parts.size() == 0){
                continue;
            }
            auto proc_name = trim(parts[parts.size()-1]);
            if (proc_name == my_process_name){
                return true;
            }
        }
        catch(const std::exception& e)
        {

        }
        
    }
    return false;
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
                    server_connection->send_msg(client, "первый аргумент не является IP-адресом\n"s);
                    continue;
                }
                if (!is_int_number(msg_parts[2])){
                    server_connection->send_msg(client, "второй аргумент не является числом\n"s);
                    continue;
                }

                try{
                    std::string ip = msg_parts[1];
                    int echo_requests_count = std::stoi(msg_parts[2]);
                    std::thread th(ping_task, ip, echo_requests_count);
                    th.detach();
                    server_connection->send_msg(client, "ok\n"s);
                } catch (std::exception e){
                    server_connection->send_msg(client, "error\n"s);
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


void ping_task(const std::string& ip, int count_echo_requests){
    PingInfo ping_info = ping(ip, count_echo_requests, 300ms);
    Storage& storage = Storage::instance();
    storage.add_statistics(ip, ping_info.echo_replies, ping_info.echo_requests);
}
