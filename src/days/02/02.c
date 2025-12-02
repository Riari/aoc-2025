#include "02.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void day02_part1(void)
{
    Lines lines = read_lines("02_input.txt");
    if (!lines.lines) return;

    free_lines(lines);
}

void day02_part2(void)
{
    Lines lines = read_lines("02_input.txt");
    if (!lines.lines) return;

    free_lines(lines);
}

bool day02_test_part1(void)
{
    Lines lines = read_lines("02_test_input.txt");
    if (!lines.lines) return false;

    free_lines(lines);
    return false;
}

bool day02_test_part2(void)
{
    Lines lines = read_lines("02_test_input.txt");
    if (!lines.lines) return false;

    free_lines(lines);
    return false;
}

const Solution day02 = {
    day02_part1,
    day02_part2,
    day02_test_part1,
    day02_test_part2
};
