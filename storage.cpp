#include "storage.h"

#include <fstream>
#include "string_utils.h"
#include <pwd.h>
#include <unistd.h>

StatisticRecord::StatisticRecord(const std::string& ip, int success_pings_count, int all_pings_count)
    : ip(ip), success_pings_count(success_pings_count), all_pings_count(all_pings_count)
{

}

Storage& Storage::instance(){
    static Storage storage;
    return storage;
}


std::string Storage::get_home_dir(){
    const char *homedir = getenv("HOME");
    if (homedir == nullptr) {
        passwd* pw = getpwuid(getuid());
        if (pw == nullptr){
            throw HomeDirNotFoundException("не удалось определить домашнюю директорию юзера");
        }
        homedir = pw->pw_dir;
    }
    return std::string(homedir);
}

Storage::Storage(){
    home_dir = get_home_dir();
}

Storage::~Storage(){
}

void Storage::add_statistics(const std::string& ip, int success_pings_count, int all_pings_count){
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    auto records = get_statistics_records_no_block();
    if (records.find(ip) != records.end()){
        records[ip].all_pings_count += all_pings_count;
        records[ip].success_pings_count += success_pings_count;
    } else {
        records[ip] = StatisticRecord(ip, success_pings_count, all_pings_count);
    }
    write_statistics_records(records);
}

std::map<std::string, StatisticRecord> Storage::get_statistics_records(){
    std::lock_guard<std::mutex> lock_guard(m_mutex);
    return get_statistics_records_no_block();
}

std::map<std::string, StatisticRecord> Storage::get_statistics_records_no_block(){
    std::map<std::string, StatisticRecord> records;
    std::ifstream fin(home_dir + "/" + STORAGE_FILE_NAME);
    std::string line;
    while (std::getline(fin, line))
    {
        auto parts = split_by_space(line);
        if (parts.size() != 3){
            throw StorageGetDataErrorException("запись состоит не из трех элементов");
        }
        StatisticRecord record(parts[0], std::stoi(parts[1]), std::stoi(parts[2]));
        records[parts[0]] = record;
    }
    return records;
}


void Storage::reset_data(){
    std::ofstream fout(home_dir + "/" + STORAGE_FILE_NAME, std::ofstream::trunc);
}

void Storage::write_statistics_records(std::map<std::string, StatisticRecord> records){
    std::ofstream fout(home_dir + "/" + STORAGE_FILE_NAME, std::ofstream::trunc);
    for (auto [key, value] : records){
        fout << value.ip << " " << std::to_string(value.success_pings_count) 
            << " " << std::to_string(value.all_pings_count) << "\n";  
    }
}
