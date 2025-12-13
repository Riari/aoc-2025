#include "05.h"

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

static ull solve(const Lines* lines, bool check_ingredients)
{
    cc_vec(Range) ranges;
    cc_init(&ranges);

    cc_vec(ull) ingredients;
    cc_init(&ingredients);

    ull result = 0;
    const char* delimiter = "-";
    char *from_str, *to_str;
    bool parsing_ingredients = false;
    for (size_t i = 0; i < lines->count; ++i)
    {
        const char* line = lines->lines[i];
        if (is_str_empty(line))
        {
            parsing_ingredients = true;
            continue;
        }

        if (parsing_ingredients)
        {
            cc_push(&ingredients, strtoull(lines->lines[i], NULL, 10));
            continue;
        }

        char temp[32];
        strcpy(temp, lines->lines[i]);

        from_str = strtok(temp, delimiter);
        to_str = strtok(NULL, delimiter);

        ull from = strtoull(from_str, NULL, 10);
        ull to = strtoull(to_str, NULL, 10);

        Range range;
        range.from = from;
        range.to = to;
        cc_push(&ranges, range);
    }

    if (check_ingredients)
    {
        cc_for_each(&ingredients, id)
        {
            cc_for_each(&ranges, range)
            {
                ull from = (*range).from;
                ull to = (*range).to;

                if (*id >= from && *id <= to)
                {
                    ++result;
                    break;
                }
            }
        }
    }
    else
    {
        bool changed;
        do
        {
            changed = false;
            for (size_t i = 0; i < cc_size(&ranges); ++i)
            {
                Range* a = cc_get(&ranges, i);

                for (size_t j = i + 1; j < cc_size(&ranges);)
                {
                    Range* b = cc_get(&ranges, j);

                    if (a->from <= b->to && b->from <= a->to)
                    {
                        // Merge b into a and erase b
                        a->from = a->from < b->from ? a->from : b->from;
                        a->to = a->to > b->to ? a->to : b->to;

                        cc_erase(&ranges, j);
                        changed = true;
                    }
                    else j++;
                }
            }
        } while (changed);

        cc_for_each(&ranges, range)
        {
            result += (range->to - range->from) + 1;
        }
    }

    cc_cleanup(&ranges);
    cc_cleanup(&ingredients);

    return result;
}

void day05_part1(void)
{
    Lines lines = read_lines("05_input.txt");
    if (!lines.lines) return;

    ull result = solve(&lines, true);
    printf("%llu", result);

    free_lines(lines);
}

void day05_part2(void)
{
    Lines lines = read_lines("05_input.txt");
    if (!lines.lines) return;

    ull result = solve(&lines, false);
    printf("%llu", result);

    free_lines(lines);
}

bool day05_test_part1(void)
{
    Lines lines = read_lines("05_test_input.txt");
    if (!lines.lines) return false;

    ull result = solve(&lines, true);

    free_lines(lines);
    return result == 3;
}

bool day05_test_part2(void)
{
    Lines lines = read_lines("05_test_input.txt");
    if (!lines.lines) return false;

    ull result = solve(&lines, false);

    free_lines(lines);
    return result == 14;
}

const Solution day05 = {
    day05_part1,
    day05_part2,
    day05_test_part1,
    day05_test_part2
};
