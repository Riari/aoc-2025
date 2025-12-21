#include "07.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "types.h"

#define START 'S'
#define SPLITTER '^'

static int width;
static int height;

static ull process_cell(const Lines* lines, ull* counts, ull* splits, const int x, const int y)
{
    ull result = 0;

    if (x >= width || y >= height) return 1;

    if (counts[y * width + x] > 0) return counts[y * width + x];

    if (lines->lines[y][x] == SPLITTER)
    {
        result += process_cell(lines, counts, splits, x - 1, y + 1);
        result += process_cell(lines, counts, splits, x + 1, y + 1);
        *splits += 1;
    }
    else
    {
        result += process_cell(lines, counts, splits, x, y + 1);
    }

    counts[y * width + x] = result;

    return result;
}

static ull solve(const Lines* lines, const bool p2)
{
    width = strlen(lines->lines[0]);
    height = lines->count;
    ull splits = 0;
    ull paths = 0;

    ull* counts = calloc(width * height, sizeof(ull));

    int start_x = 0, start_y = 0;
    for (int x = 0; x < width; ++x)
    {
        const char c = lines->lines[0][x];
        if (c == START)
        {
            start_x = x;
            break;
        }
    }

    paths = process_cell(lines, counts, &splits, start_x, start_y + 1);

    free(counts);

    return p2 ? paths : splits;
}

void day07_part1(void)
{
    Lines lines = read_lines("07_input.txt");
    if (!lines.lines) return;

    printf("%llu", solve(&lines, false));

    free_lines(lines);
}

void day07_part2(void)
{
    Lines lines = read_lines("07_input.txt");
    if (!lines.lines) return;

    printf("%llu", solve(&lines, true));

    free_lines(lines);
}

bool day07_test_part1(void)
{
    Lines lines = read_lines("07_test_input.txt");
    if (!lines.lines) return false;

    int result = solve(&lines, false);

    free_lines(lines);
    return result == 21;
}

bool day07_test_part2(void)
{
    Lines lines = read_lines("07_test_input.txt");
    if (!lines.lines) return false;

    int result = solve(&lines, true);

    free_lines(lines);
    return result == 40;
}

const Solution day07 = {
    day07_part1,
    day07_part2,
    day07_test_part1,
    day07_test_part2
};
