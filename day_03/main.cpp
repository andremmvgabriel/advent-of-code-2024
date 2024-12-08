#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <utility>
#include <vector>
#include "common/AoCDay.hpp"
#include "common/string_split.hpp"
#include "common/time_block.hpp"

using InputData = std::string;

class Day03 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    std::int64_t PartOne(const InputData& data) const override;
    std::int64_t PartTwo(const InputData& data) const override;

  public:
    Day03() { AoCDay<InputData>::day_number_ = "day_03"; };
};

InputData Day03::ParseInputs(std::ifstream& data)
{
    // Actual outputs
    InputData command{};

    std::string line;
    while (std::getline(data, line))
    {
        command.insert(command.cend(), line.cbegin(), line.cend());
    }

    return command;
}

using mul_t = std::pair<std::int64_t, std::int64_t>;

std::vector<mul_t> FindMulValues(const std::string& command)
{
    const std::regex pattern{R"(mul\((-?\d+),(-?\d+)\))"};
    const std::sregex_iterator end;

    // Find all the matches in the command
    std::sregex_iterator iterator(command.cbegin(), command.cend(), pattern);

    // Go through every match, and extract its values
    std::vector<mul_t> values{};
    while (iterator != end)
    {
        std::smatch match = *iterator;
        values.emplace_back(std::stol(match[1]), std::stol(match[2]));
        ++iterator;
    }

    return values;
}

std::int64_t SumMulValues(const std::vector<mul_t>& mul_values)
{
    return std::accumulate(mul_values.cbegin(), mul_values.cend(), 0L, [](const std::int64_t res, const mul_t val) {
        return res + val.first * val.second;
    });
}

std::int64_t Day03::PartOne(const InputData& data) const
{
    const std::vector<mul_t> values = FindMulValues(data);
    return SumMulValues(values);
}

std::string GetDoSectionsFromCommand(const std::string& command)
{
    std::string do_command{};

    // Separate the whole command on every "do()" found
    const auto line_split = string_split(command, "do()");

    for (const auto& split : line_split)
    {
        // Separate the splitted section on every "don't()" found
        const auto do_section = string_split(split, "don't()");

        // We know that a single "do()" section is present, and it is the first one
        do_command.insert(do_command.cend(), do_section.front().cbegin(), do_section.front().cend());
    }

    return do_command;
}

std::int64_t Day03::PartTwo(const InputData& data) const
{
    const std::string command = GetDoSectionsFromCommand(data);
    const std::vector<mul_t> values = FindMulValues(command);
    return SumMulValues(values);
}

int main(int argc, char* argv[])
{
    Day03{}.Run(argc, argv);
    return 0;
}
