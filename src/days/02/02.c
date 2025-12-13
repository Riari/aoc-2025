#include "02.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC_NO_SHORT_NAMES
#include "cc.h"

#include "utils.h"
#include "types.h"

typedef struct Range
{
    ull from;
    ull to;
} Range;

static void parse(const char* line, Range* out_ranges, size_t* out_count)
{
    Range current_range;
    int range_i = 0;
    int parsing_from = true;
    char buf[32];
    int buf_i = 0;
    for (size_t i = 0; i < strlen(line); ++i)
    {
        char c = line[i];
        if (parsing_from)
        {
            if (c == '-')
            {
                buf[buf_i] = 0;
                current_range.from = strtoull(buf, NULL, 10);
                buf[0] = 0;
                buf_i = 0;
                parsing_from = false;
            }
            else buf[buf_i++] = c;
        }
        else
        {
            if (c == ',')
            {
                buf[buf_i] = 0;
                current_range.to = strtoull(buf, NULL, 10);
                out_ranges[range_i++] = current_range;
                buf[0] = 0;
                buf_i = 0;
                parsing_from = true;
            }
            else buf[buf_i++] = c;
        }
    }

    buf[buf_i] = 0;
    current_range.to = strtoull(buf, NULL, 10);
    out_ranges[range_i] = current_range;
    *out_count = range_i + 1;
}

static int count_digits(ull num)
{
    if (num == 0) return 1;
    int count = 0;
    while (num > 0)
    {
        ++count;
        num /= 10;
    }
    return count;
}

static ull power_of_10(int n)
{
    ull result = 1;
    for (int i = 0; i < n; ++i)
    {
        result *= 10;
    }
    return result;
}

static bool is_invalid_p1(ull id)
{
    int digits = count_digits(id);

    if (digits % 2 != 0) return false;

    int half = digits / 2;
    ull divisor = power_of_10(half);

    ull first_half = id / divisor;
    ull second_half = id % divisor;

    return first_half == second_half;
}

static bool is_invalid_p2(ull id)
{
    int digits = count_digits(id);

    for (int pattern_length = 1; pattern_length <= digits / 2; ++pattern_length)
    {
        if (digits % pattern_length != 0) continue;

        ull divisor = power_of_10(pattern_length);
        ull pattern = id % divisor;

        ull temp = id;
        bool matches = true;
        while (temp > 0)
        {
            if (temp % divisor != pattern)
            {
                matches = false;
                break;
            }
            temp /= divisor;
        }

        if (matches) return true;
    }

    return false;
}

static ull solve(const Lines* lines, bool p2)
{
    Range ranges[100];
    size_t range_count = 0;
    parse(lines->lines[0], ranges, &range_count);

    ull result = 0;
    for (size_t i = 0; i < range_count; ++i)
    {
        const Range* range = &ranges[i];
        for (ull id = range->from; id <= range->to; ++id)
        {
            if (p2)
            {
                if (is_invalid_p2(id)) result += id;
                continue;
            }

            if (is_invalid_p1(id)) result += id;
        }
    }

    return result;
}

void day02_part1(void)
{
    Lines lines = read_lines("02_input.txt");
    if (!lines.lines) return;

    ull result = solve(&lines, false);
    printf("%llu", result);

    free_lines(lines);
}

void day02_part2(void)
{
    Lines lines = read_lines("02_input.txt");
    if (!lines.lines) return;

    ull result = solve(&lines, true);
    printf("%llu", result);

    free_lines(lines);
}

bool day02_test_part1(void)
{
    Lines lines = read_lines("02_test_input.txt");
    if (!lines.lines) return false;

    ull result = solve(&lines, false);

    free_lines(lines);
    return result == 1227775554;
}

bool day02_test_part2(void)
{
    Lines lines = read_lines("02_test_input.txt");
    if (!lines.lines) return false;

    ull result = solve(&lines, true);

    free_lines(lines);
    return result == 4174379265;
}

const Solution day02 = {
    day02_part1,
    day02_part2,
    day02_test_part1,
    day02_test_part2
};
