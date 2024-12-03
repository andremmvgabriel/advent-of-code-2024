#include <algorithm>
#include <iterator>
#include <numeric>
#include <tuple>
#include <unordered_map>
#include <vector>
#include "common/AoCDay.hpp"
#include "common/string_split.hpp"
#include "common/time_block.hpp"

using List = std::vector<std::int64_t>;
using InputData = std::tuple<List, List>;

class Day01 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    std::int64_t PartOne(const InputData& data) const override;
    std::int64_t PartTwo(const InputData& data) const override;

  public:
    Day01() { AoCDay<InputData>::day_number_ = "day_01"; };
};

InputData Day01::ParseInputs(std::ifstream& data)
{
    // Actual outputs
    List list_lhs{};
    List list_rhs{};

    std::string line;
    while (std::getline(data, line))
    {
        const std::string delimiter = "   ";
        const auto entry = string_split(line, delimiter);
        list_lhs.push_back(std::stol(entry[0]));
        list_rhs.push_back(std::stol(entry[1]));
    }

    return {list_lhs, list_rhs};
}

std::int64_t Day01::PartOne(const InputData& data) const
{
    // Inputs
    auto [list_lhs, list_rhs] = data;

    // Ascendent sorting of the first list
    std::sort(list_lhs.begin(), list_lhs.end());

    // Ascendent sorting of the second lint
    std::sort(list_rhs.begin(), list_rhs.end());

    // Compute the distance between each i-th element
    List list_distances{};
    std::transform(list_lhs.cbegin(),
                   list_lhs.cend(),
                   list_rhs.cbegin(),
                   std::back_inserter(list_distances),
                   [](const auto& lhs, const auto& rhs) { return std::abs(lhs - rhs); });

    // Sums all the distances and outputs the result
    return std::accumulate(list_distances.cbegin(),
                           list_distances.cend(),
                           0L  // Initial value of the sum
    );
}

std::int64_t Day01::PartTwo(const InputData& data) const
{
    // Inputs
    auto [list_lhs, list_rhs] = data;

    // Map to register the frequency of each number
    std::unordered_map<std::int64_t, std::int64_t> number_counters{};

    // Register all the expected numbers from the first list
    for (const auto& value : list_lhs)
    {
        number_counters[value] = 0;
    }

    // Registers the frequency of each number from the second list
    for (const auto& value : list_rhs)
    {
        ++number_counters[value];
    }

    // Compute num*freq of all numbers and output the result
    return std::accumulate(
        list_lhs.cbegin(),
        list_lhs.cend(),
        0L,  // Initial value of the sum
        [&number_counters](std::int64_t res, const auto& value) { return res + value * number_counters[value]; });
}

int main(int argc, char* argv[])
{
    Day01{}.Run(argc, argv);
    return 0;
}
