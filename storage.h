#pragma once

#include <string>
#include <map>
#include <mutex>

using namespace std::string_literals;

struct StatisticRecord
{
    StatisticRecord()=default;
    StatisticRecord(const std::string& ip, int success_pings_count, int all_pings_count);
    std::string ip;
    int success_pings_count;
    int all_pings_count;
};

class Storage{
public:
    static Storage& instance();
    void add_statistics(const std::string& ip, int success_pings_count, int all_pings_count);
    std::map<std::string, StatisticRecord> get_statistics_records();
    void reset_data();
private:
    static const inline std::string STORAGE_FILE_NAME = "test-pinger-data.txt"s;
    std::string home_dir;
    std::mutex m_mutex;
    Storage();
    ~Storage();
    Storage(Storage const&) = delete;
    Storage& operator= (Storage const&) = delete;
    std::string get_home_dir();
    std::map<std::string, StatisticRecord> get_statistics_records_no_block();
    void write_statistics_records(std::map<std::string, StatisticRecord> records);
};

class StorageGetDataErrorException : public std::exception{
public:
    StorageGetDataErrorException(const std::string& msg) : m_msg(msg) {}
    const char* what() const noexcept override{return m_msg.c_str();}
private:
    std::string m_msg;
};

class HomeDirNotFoundException : public std::exception{
public:
    HomeDirNotFoundException(const std::string& msg) : m_msg(msg) {}
    const char* what() const noexcept override{return m_msg.c_str();}
private:
    std::string m_msg;
};