#include "01.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define DIAL_MAX 100 // Exclusive

static int wrap(int val)
{
    val = val % DIAL_MAX;
    if (val < 0) val += DIAL_MAX;
    return val;
}

static int count_zero_hits(int start, int delta)
{
    int steps_to_zero;
    if (delta > 0)
    {
        steps_to_zero = (DIAL_MAX - start) % DIAL_MAX;
        if (steps_to_zero == 0) steps_to_zero = DIAL_MAX;
    }
    else
    {
        steps_to_zero = start ? start : DIAL_MAX;
    }

    int delta_abs = abs(delta);
    return delta_abs < steps_to_zero ? 0 : 1 + (delta_abs - steps_to_zero) / DIAL_MAX;
}

static int solve(const Lines* lines, bool p2)
{
    int dial = 50;
    int result = 0;

    for (size_t i = 0; i < lines->count; ++i)
    {
        const char* line = lines->lines[i];
        const bool add = line[0] == 'R';
        const int num = atoi(&line[1]);

        int was = dial;
        int delta = add ? num : -num;
        dial = wrap(dial + delta);

        if (p2)
        {
            int zero_hits = count_zero_hits(was, delta);
            result += zero_hits;
        }
        else if (dial == 0) ++result;
    }

    return result;
}

void day01_part1(void)
{
    Lines lines = read_lines("01_input.txt");
    if (!lines.lines) return;

    printf("%d", solve(&lines, false));

    free_lines(lines);
}

void day01_part2(void)
{
    Lines lines = read_lines("01_input.txt");
    if (!lines.lines) return;

    printf("%d", solve(&lines, true));

    free_lines(lines);
}

bool day01_test_part1(void)
{
    Lines lines = read_lines("01_test_input.txt");
    if (!lines.lines) return false;

    const int result = solve(&lines, false);

    free_lines(lines);
    return result == 3;
}

bool day01_test_part2(void)
{
    Lines lines = read_lines("01_test_input.txt");
    if (!lines.lines) return false;

    const int result = solve(&lines, true);

    free_lines(lines);
    return result == 6;
}

const Solution day01 = {
    day01_part1,
    day01_part2,
    day01_test_part1,
    day01_test_part2
};
