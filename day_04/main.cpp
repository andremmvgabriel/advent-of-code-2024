#include <numeric>
#include <string>
#include <vector>
#include "common/AoCDay.hpp"
#include "common/string_split.hpp"
#include "common/time_block.hpp"

using Line = std::string;
using InputData = std::vector<Line>;

class Day04 : public AoCDay<InputData>
{
    InputData ParseInputs(std::ifstream& data) override;
    std::int64_t PartOne(const InputData& data) const override;
    std::int64_t PartTwo(const InputData& data) const override;

  public:
    Day04() { AoCDay<InputData>::day_number_ = "day_04"; };
};

InputData Day04::ParseInputs(std::ifstream& data)
{
    // Actual outputs
    InputData puzzle{};

    std::string line;
    while (std::getline(data, line))
    {
        puzzle.push_back(line);
    }

    return puzzle;
}

struct position_t
{
    std::int64_t x{};
    std::int64_t y{};
};

using direction_t = position_t;

template <typename T>
direction_t operator*(const T& val, const direction_t& pos)
{
    const auto cast_val = static_cast<std::int64_t>(val);
    return {cast_val * pos.x, cast_val * pos.y};
}

position_t operator+(const position_t& pos, const direction_t& dir)
{
    return {pos.x + dir.x, pos.y + dir.y};
}

bool IsPositionInsideMap(const position_t& pos, const position_t& min, const position_t& max)
{
    return (pos.x >= min.x) && (pos.y >= min.y) && (pos.x <= max.x) && (pos.y <= max.y);
}

std::vector<position_t> FindAllPositionsOfElement(const InputData& puzzle,
                                                  const char& element,
                                                  const std::size_t margin = 0)
{
    std::vector<position_t> positions{};

    for (std::size_t y{margin}; y < puzzle.size() - margin; ++y)
    {
        for (std::size_t x{margin}; x < puzzle[y].size() - margin; ++x)
        {
            if (puzzle[y][x] == element)
            {
                const position_t cur_pos{static_cast<std::int64_t>(x), static_cast<std::int64_t>(y)};
                positions.push_back(cur_pos);
            }
        }
    }

    return positions;
}

std::int64_t SearchAndCountWordInAllDirections(const InputData& puzzle,
                                               const position_t& start_pos,
                                               const std::string& target_word)
{
    // Puzzle position limits
    static const position_t puzzle_min{0, 0};
    static const position_t puzzle_max{puzzle.empty() ? 0 : static_cast<std::int64_t>(puzzle[0].size() - 1),
                                       static_cast<std::int64_t>(puzzle.size() - 1)};

    // Creates all search directions
    std::vector<direction_t> search_directions{
        {1, 0},    // E
        {1, 1},    // SE
        {0, 1},    // S
        {-1, 1},   // SW
        {-1, 0},   // W
        {-1, -1},  // NW
        {0, -1},   // N
        {1, -1}    // NE
    };

    // Go through every character in the target word
    for (std::size_t i{1}; i < target_word.size(); ++i)
    {
        // Search in all remaining directions
        for (std::size_t j{search_directions.size()}; j > 0; --j)
        {
            // Current index
            const auto cur_idx = static_cast<std::ptrdiff_t>(j - 1);

            // Current direction and position
            const direction_t cur_dir = search_directions[cur_idx];
            const position_t cur_pos = start_pos + i * cur_dir;

            // Remove direction if the search went out-of-bounds
            if (!IsPositionInsideMap(cur_pos, puzzle_min, puzzle_max))
            {
                search_directions.erase(search_directions.cbegin() + cur_idx);
                continue;
            }

            // Get the current char for the position
            const char cur_char = puzzle[cur_pos.y][cur_pos.x];

            // Remove direction if the char does not match the expected one
            if (cur_char != target_word[i])
            {
                search_directions.erase(search_directions.cbegin() + cur_idx);
            }
        }
    }

    // All remaining directions mean that the whole word was matched in those directions
    return static_cast<std::int64_t>(search_directions.size());
}

std::int64_t Day04::PartOne(const InputData& data) const
{
    // Get the positions of all the 'X's in the puzzle
    const std::vector<position_t> x_positions = FindAllPositionsOfElement(data, 'X');

    return std::accumulate(
        x_positions.cbegin(), x_positions.cend(), 0L, [&data](const std::int64_t& res, const position_t& x_pos) {
            return res + SearchAndCountWordInAllDirections(data, x_pos, "XMAS");
        });
}

bool HasCrossWord(const InputData& puzzle, const position_t& center_pos)
{
    // Get all the cross edge positions
    const position_t tl = center_pos + direction_t{-1, -1};  // Top Left
    const position_t bl = center_pos + direction_t{-1, 1};   // Bottom Left
    const position_t tr = center_pos + direction_t{1, -1};   // Top Right
    const position_t br = center_pos + direction_t{1, 1};    // Bottom Right

    // Get the chars in each cross edge positions
    const char& tl_char = puzzle[tl.y][tl.x];
    const char& bl_char = puzzle[bl.y][bl.x];
    const char& tr_char = puzzle[tr.y][tr.x];
    const char& br_char = puzzle[br.y][br.x];

    // Cross word not found if 'X' or 'A' is present in an edge
    if ((tl_char == 'X' || bl_char == 'X' || tr_char == 'X' || br_char == 'X') ||
        (tl_char == 'A' || bl_char == 'A' || tr_char == 'A' || br_char == 'A'))
    {
        return false;
    }

    // Cross word not found if a diagonal contains the same char
    if ((tl_char == 'M' && br_char == 'M') || (tl_char == 'S' && br_char == 'S') ||
        (tr_char == 'M' && bl_char == 'M') || (tr_char == 'S' && bl_char == 'S'))
    {
        return false;
    }

    return true;
}

std::int64_t Day04::PartTwo(const InputData& data) const
{
    // Get the positions of all the 'A's in the puzzle
    const std::vector<position_t> a_positions = FindAllPositionsOfElement(data, 'A', 1);

    return std::accumulate(
        a_positions.cbegin(), a_positions.cend(), 0L, [&data](const std::int64_t& res, const position_t& a_pos) {
            return HasCrossWord(data, a_pos) ? (res + 1) : res;
        });
}

int main(int argc, char* argv[])
{
    Day04{}.Run(argc, argv);
    return 0;
}
