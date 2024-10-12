#include "parser.h"

#include <cstdint>

using namespace std::string_literals;

constexpr char EOT = 4;

bool is_whitespace(char c){
    return c == 32 || c == 9 || c == 13 || c == 10;
}

bool is_number(char c){
    return c >= '0' && c <= '9';
}

class ExcpectedException : public std::exception{
public:
    ExcpectedException(const std::string& msg) : m_msg(msg) {}
    const char* what() const noexcept override{return m_msg.c_str();}
private:
    std::string m_msg;
};

class ParsingWrapper{
public:
    char ch;
    ParsingWrapper(const std::string& source) : _source(source), _index(0){
        get_next();
    }
    void get_next(){
        if (_index >= _source.size()){
            ch = EOT;
            return;
        }
        ch = _source[_index];
        _index++;
    }
    void check(char c){
        if (ch != c){
            throw ExcpectedException("ожидалось '"s  + c + "' но '"s + ch + "'"s);
        }
        get_next();
    }
private:
    std::string _source;
    int64_t _index;
};

uint32_t parse_number(ParsingWrapper& wrap){
    
}

bool is_ip_address(const std::string& str){
    try{
        ParsingWrapper wrap(str);
        uint32_t octet1 = parse_number();

    } catch (ExcpectedException e){
        return false;
    }
}

bool is_int_number(const std::string& str){

}