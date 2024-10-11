#include "listen_socket_connection.h"
#include <iostream>
#include "storage.h"
#include <string>

using namespace std::string_literals;

int main(){
    Storage& storage = Storage::instance();
    try{
        storage.add_statistics("192.168.1.1", 1, 2);
        storage.add_statistics("192.168.1.1", 3, 8);
        storage.add_statistics("192.168.21.11", 30, 248);
        for (auto [key, value] : storage.get_statistics_records()){
            std::cout << key << ": отправлено " << value.all_pings_count << " вернулось " << value.success_pings_count << "\n";
        }
    } catch (std::exception e){
        storage.reset_data();
    }
    return 0;
    ListenSocketConnection server_connection(65236);
    auto client = server_connection.accept_client_connection();
    std::cout << "client: " << server_connection.receive_msg(client) << "\n";
    server_connection.send_msg(client, "Hello, client!");
}