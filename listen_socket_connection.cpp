#include "listen_socket_connection.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

ListenSocketConnection::ListenSocketConnection(tcp_port port_number)
    : m_port_number(port_number)
{
    m_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket_descriptor == -1){
        throw ServerConnectionErrorException("Не удалось создать сокет");
    }
    int opt_tmp_value = 1;
    if (setsockopt(m_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &opt_tmp_value, sizeof(opt_tmp_value)) == -1){
        throw ServerConnectionErrorException("Не удалось установить опцию сокета SO_REUSEADDR");
    }
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(m_port_number);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(m_socket_descriptor, (sockaddr *)&addr, sizeof(addr)) == -1){
        throw ServerConnectionErrorException("Не удалось поставить сокет на прослушку (bind)");
    }
    if (listen(m_socket_descriptor, MAX_CLIENT_CONNECTIONS) == -1){
        throw ServerConnectionErrorException("Не удалось поставить сокет на прослушку (listen)");
    }
}

ListenSocketConnection::~ListenSocketConnection(){
    close(m_socket_descriptor);
}


socket_descriptor ListenSocketConnection::accept_client_connection(){
    socket_descriptor client = accept(m_socket_descriptor, nullptr, nullptr);
    if (client == -1){
        throw ClientConnectionErrorException("Не удалось принять клиентское подключение");
    }
    return client;
}

std::string ListenSocketConnection::receive_msg(socket_descriptor client){
    memset(m_receive_buffer, 0, RECEIVE_BUFFER_SIZE);
    int bytes_read = recv(client, m_receive_buffer, RECEIVE_BUFFER_SIZE, 0);
    if (bytes_read == -1){
        throw SendRecvErrorException("Не удалось получить сообщение клиента");
    }
    return std::string(m_receive_buffer);
}

void ListenSocketConnection::send_msg(socket_descriptor client, const std::string msg){
    if (send(client, msg.c_str(), msg.size() + 1, 0) == -1){
        throw SendRecvErrorException("Не удалось отправить сообщение клиенту");
    }
}
