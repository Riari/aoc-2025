#include "04.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static int solve(Lines* lines, bool remove)
{
    int result = 0;
    while (1)
    {
        int accessible = 0;
        for (size_t y = 0; y < lines->count; ++y)
        {
            const char* line = lines->lines[y];
            const size_t len = strlen(line);

            for (size_t x = 0; x < len; ++x)
            {
                if (line[x] != '@') continue;

                int rolls = 0;
                const size_t start_x = x > 0 ? x - 1 : x;
                const size_t start_y = y > 0 ? y - 1 : y;
                const size_t end_x = x < len - 1 ? x + 1 : x;
                const size_t end_y = y < lines->count - 1 ? y + 1 : y;
                for (size_t nx = start_x; nx <= end_x; ++nx)
                {
                    for (size_t ny = start_y; ny <= end_y; ++ny)
                    {
                        if (nx == x && ny == y) continue;
                        if (lines->lines[ny][nx] == '@') ++rolls;
                    }
                }

                if (rolls < 4)
                {
                    ++accessible;
                    if (remove) lines->lines[y][x] = '.';
                }
            }
        }

        result += accessible;

        if (!remove || accessible == 0) break;
    }

    return result;
}

void day04_part1(void)
{
    Lines lines = read_lines("04_input.txt");
    if (!lines.lines) return;

    printf("%d", solve(&lines, false));

    free_lines(lines);
}

void day04_part2(void)
{
    Lines lines = read_lines("04_input.txt");
    if (!lines.lines) return;

    printf("%d", solve(&lines, true));

    free_lines(lines);
}

bool day04_test_part1(void)
{
    Lines lines = read_lines("04_test_input.txt");
    if (!lines.lines) return false;

    int result = solve(&lines, false);

    free_lines(lines);
    return result == 13;
}

bool day04_test_part2(void)
{
    Lines lines = read_lines("04_test_input.txt");
    if (!lines.lines) return false;

    int result = solve(&lines, true);

    free_lines(lines);
    return result == 43;
}

const Solution day04 = {
    day04_part1,
    day04_part2,
    day04_test_part1,
    day04_test_part2
};
