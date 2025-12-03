#include "03.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

typedef unsigned long long ull;

static ull solve(const Lines* lines, const size_t line_length, const size_t digit_count)
{
    ull result = 0;
    size_t start_index = 0;
    char digits[16] = {0};
    size_t digit_index = 0;

    for (size_t i = 0; i < lines->count; ++i)
    {
        const char* line = lines->lines[i];
        const size_t line_len = strlen(line);

        if (strlen(line) != line_length) continue;

        for (size_t j = 0; j < digit_count; ++j)
        {
            const size_t digits_remaining = digit_count - j - 1;
            const size_t end_index = (line_len > digits_remaining) ? (line_len - digits_remaining) : line_len;

            char max_digit = '0';
            size_t max_index = start_index;
            for (size_t k = start_index; k < end_index; ++k)
            {
                const char digit = line[k];
                if (digit > max_digit)
                {
                    max_digit = digit;
                    max_index = k;
                }
            }

            digits[digit_index] = max_digit;
            start_index = max_index + 1;
            ++digit_index;
        }

        digits[digit_index] = 0;

        ull joltage = strtoull(digits, NULL, 10);
        result += joltage;

        // Reset for next line
        memset(&digits[0], 0, sizeof(digits));
        start_index = 0;
        digit_index = 0;
    }

    return result;
}

void day03_part1(void)
{
    Lines lines = read_lines("03_input.txt");
    if (!lines.lines) return;

    int result = solve(&lines, 100, 2);
    printf("%d", result);

    free_lines(lines);
}

void day03_part2(void)
{
    Lines lines = read_lines("03_input.txt");
    if (!lines.lines) return;

    ull result = solve(&lines, 100, 12);
    printf("%llu", result);

    free_lines(lines);
}

bool day03_test_part1(void)
{
    Lines lines = read_lines("03_test_input.txt");
    if (!lines.lines) return false;

    int result = solve(&lines, 15, 2);

    free_lines(lines);
    return result == 357;
}

bool day03_test_part2(void)
{
    Lines lines = read_lines("03_test_input.txt");
    if (!lines.lines) return false;

    ull result = solve(&lines, 15, 12);

    free_lines(lines);
    return result == 3121910778619;
}

const Solution day03 = {
    day03_part1,
    day03_part2,
    day03_test_part1,
    day03_test_part2
};
