#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include "common/AoCDay.hpp"
#include "common/string_split.hpp"
#include "common/time_block.hpp"

using Report = std::vector<std::int64_t>;
using InputData = std::vector<Report>;

class Day02 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    std::int64_t PartOne(const InputData& data) const override;
    std::int64_t PartTwo(const InputData& data) const override;

  public:
    Day02() { AoCDay<InputData>::day_number_ = "day_02"; };
};

InputData Day02::ParseInputs(std::ifstream& data)
{
    // Actual outputs
    InputData reports{};

    std::string line;
    while (std::getline(data, line))
    {
        // Separate all the values in the report
        const auto report_values = string_split(line, " ");

        // Converts the values from string to integers
        Report report{};
        std::transform(
            report_values.cbegin(), report_values.cend(), std::back_inserter(report), [](const auto& value_str) {
                return std::stol(value_str);
            });

        // Cache the report
        reports.push_back(report);
    }

    return reports;
}

Report::const_iterator FindUnsafeIncrease(const Report& report)
{
    // Function to detect if the increase is unsafe
    constexpr auto unsafe_increase = [](const auto& lhs, const auto& rhs) {
        const auto diff = lhs - rhs;
        const auto diff_abs = std::abs(diff);
        return diff >= 0 || diff_abs >= 4;
    };

    // Try to find an unsafe increase
    return std::adjacent_find(report.cbegin(), report.cend(), unsafe_increase);
}

bool IsSafelyIncreasing(const Report& report)
{
    // True if safe
    return FindUnsafeIncrease(report) == report.cend();
}

Report::const_iterator FindUnsafeDecrease(const Report& report)
{
    // Function to detect if the decrease is unsafe
    constexpr auto unsafe_decrease = [](const auto& lhs, const auto& rhs) {
        const auto diff = lhs - rhs;
        const auto diff_abs = std::abs(diff);
        return diff <= 0 || diff_abs >= 4;
    };

    // Try to find an unsafe decrease
    return std::adjacent_find(report.cbegin(), report.cend(), unsafe_decrease);
}

bool IsSafelyDecreasing(const Report& report)
{
    // True if safe
    return FindUnsafeDecrease(report) == report.cend();
}

std::int64_t Day02::PartOne(const InputData& data) const
{
    return std::accumulate(data.cbegin(), data.cend(), 0L, [](std::int64_t res, const Report& report) {
        const bool is_safe = IsSafelyIncreasing(report) || IsSafelyDecreasing(report);
        return is_safe ? (res + 1) : res;
    });
}

Report RemoveElementFromReport(const Report& report, const Report::const_iterator& element)
{
    Report report_new = report;                                     // Copy the original report
    const auto position = std::distance(report.cbegin(), element);  // Element position
    report_new.erase(report_new.cbegin() + position);               // Remove the element
    return report_new;
}

bool IsSafelyIncreasingWithError(const Report& report)
{
    // Check if the whole report is safely increasing
    const auto pos = FindUnsafeIncrease(report);
    if (pos == report.cend())
    {
        return true;
    }

    // Current report is unsafe, detected from "pos" to "pos + 1"
    // > To try to make it safe, we can create two new reports,
    //   one without "pos", and the other without "pos + 1"

    // Setup the new reports
    Report report_fix_lhs = RemoveElementFromReport(report, pos);             // Without "pos"
    Report report_fix_rhs = RemoveElementFromReport(report, std::next(pos));  // Without "pos + 1"

    // Check if any report is safely increasing
    return IsSafelyIncreasing(report_fix_lhs) || IsSafelyIncreasing(report_fix_rhs);
}

bool IsSafelyDecreasingWithError(const Report& report)
{
    // Check if the whole report is safely decreasing
    const auto pos = FindUnsafeDecrease(report);
    if (pos == report.cend())
    {
        return true;
    }

    // Current report is unsafe, detected from "pos" to "pos + 1"
    // > To try to make it safe, we can create two new reports,
    //   one without "pos", and the other without "pos + 1"

    // Setup the new reports
    Report report_fix_lhs = RemoveElementFromReport(report, pos);             // Without "pos"
    Report report_fix_rhs = RemoveElementFromReport(report, std::next(pos));  // Without "pos + 1"

    // Check if any report is safely increasing
    return IsSafelyDecreasing(report_fix_lhs) || IsSafelyDecreasing(report_fix_rhs);
}

std::int64_t Day02::PartTwo(const InputData& data) const
{
    return std::accumulate(data.cbegin(), data.cend(), 0L, [](std::int64_t res, const Report& report) {
        const bool is_safe = IsSafelyIncreasingWithError(report) || IsSafelyDecreasingWithError(report);
        return is_safe ? (res + 1) : res;
    });
}

int main(int argc, char* argv[])
{
    Day02{}.Run(argc, argv);
    return 0;
}
