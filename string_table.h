#pragma once

#include <vector>
#include <string>
#include <unordered_map>

std::vector<std::string> string_table;
std::unordered_map<std::string, size_t> string_index_map;

const char* get_str(size_t index)
{
    if (index < string_table.size())
        return string_table[index].c_str();

    return nullptr;
}

size_t get_index(const std::string& str)
{
    auto it = string_index_map.find(str);

    if (it != string_index_map.end())
        return it->second;

    size_t index = string_table.size();

    string_table.push_back(str);
    string_index_map[str] = index;

    return index;
}