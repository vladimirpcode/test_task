#include "string_utils.h"

std::string trim(const std::string& str){
    size_t start_index = 0;
    while (start_index != str.size() && std::isspace(str.c_str()[start_index])){
        start_index++;
    }
    if (start_index == str.size()){
        return ""s;
    }
    size_t end_index = str.size() - 1;
    while (end_index != start_index && (std::isspace(str.c_str()[end_index]) || str.c_str()[end_index] == 0)){
        end_index--;
    }

    return str.substr(start_index, (end_index + 1) - start_index);
}

int skip_spaces(const std::string& str, size_t start_index){
    size_t index = start_index;
    while (index < str.size() && std::isspace(str.c_str()[index])){
        index++;
    }
    return index;
}

std::vector<std::string> split_by_space(const std::string& str){
    std::vector<std::string> parts;
    size_t index = 0;
    while (index != str.size()){
        index = skip_spaces(str, index);
        if (index == str.size()){
            return parts;
        }
        std::string part = ""s;
        while (index != str.size() && !std::isspace(str.c_str()[index])){
            part += str.c_str()[index];
            index++;
        }
        if (part.size() != 0){
            parts.push_back(part);
        }
    }
    return parts;
}

std::vector<std::string> split_by_newline(const std::string& str){
    std::vector<std::string> parts;
    size_t index = 0;
    std::string part = ""s;
    while (index != str.size()){
        if (str.c_str()[index] == '\n'){
            if (part.size() != 0){
                parts.push_back(part);
                part = ""s;
            }
        } else {
            part += str.c_str()[index];
        }
        index++;
    }
    if (part.size() != 0){
        parts.push_back(part);
    }
    return parts;
}


std::vector<std::string> split(const std::string& str, char delimiter){
    std::vector<std::string> parts;
    size_t index = 0;
    std::string part = ""s;
    char c;
    while (index != str.size()){
        c = str.c_str()[index];
        if (c == delimiter){
            if (part != ""s){
                parts.push_back(part);
                part = ""s;
            }
        } else {
            part += c;
        }
        index++;
    }
    if (c != delimiter && part != ""s){
        parts.push_back(part);
    }
    return parts;
}