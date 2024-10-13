#pragma once

#include <string>
#include <vector>

using namespace std::string_literals;

std::string trim(const std::string& str);
std::vector<std::string> split_by_space(const std::string& str);
std::vector<std::string> split_by_newline(const std::string& str);
std::vector<std::string> split(const std::string& str, char delimiter);