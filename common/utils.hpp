#pragma once

#include <chrono>
#include <vector>
#include <string>

//
// Times the execution of a given function
//
template <typename Func>
float time_block(Func func)
{
    auto begin = std::chrono::steady_clock::now();
    func();
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0;
}

//
// Splits a string into a vector of strings accordingly to a given delimiter
//
inline std::vector<std::string> string_split(std::string string, std::string delimiter) {
    std::vector<std::string> output;

    size_t index = string.find(delimiter);
    while (index != -1UL) {
        output.push_back(std::string(&string[0], &string[index]));
        string = std::string(&string[index+delimiter.size()]);
        index = string.find(delimiter);
    }

    output.push_back(string);

    return output;
}
