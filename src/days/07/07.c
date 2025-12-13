#include "07.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC_NO_SHORT_NAMES
#include <cc.h>

#include "utils.h"

typedef struct Vec2
{
    int x;
    int y;
} Vec2;

static Vec2 id_to_coords(const int id, const int width)
{
    Vec2 vec = {
        .x = id % width,
        .y = id / width
    };

    return vec;
}

static int coords_to_id(const Vec2* coords, const int width)
{
    return coords->y * width + coords->x;
}

static bool try_create_beam(cc_map(int, int)* beams, cc_map(int, int)* new_beams, const Vec2* start_coords, const int width)
{
    const int new_id = coords_to_id(start_coords, width);
    if (cc_get(beams, new_id) == NULL)
    {
        cc_insert(new_beams, new_id, new_id);
        return true;
    }

    return false;
}

static int solve(const Lines* lines)
{
    int result = 0;

    cc_map(int, int) beams;
    cc_init(&beams);

    const int width = strlen(lines->lines[0]);
    const int height = lines->count;
    const int last_id = (width * height) - 1;
    for (int x = 0; x < width; ++x)
    {
        const char c = lines->lines[0][x];
        if (c == 'S')
        {
            cc_insert(&beams, x, x);
            break;
        }
    }

    cc_map(int, int) new_beams;
    cc_init(&new_beams);

    // Finished beams by end ID
    cc_set(int) finished_beams;
    cc_init(&finished_beams);

    cc_map(int, int) splits;
    cc_init(&splits);

    bool changed;
    do
    {
        changed = false;
        cc_for_each(&beams, start, end)
        {
            if (cc_get(&finished_beams, *end) != NULL) continue;

            // Try to move the beam
            const int new_end = *end + width;

            if (new_end > last_id) continue;

            const Vec2 coords = id_to_coords(new_end, width);
            const char c = lines->lines[coords.y][coords.x];
            if (c == '^')
            {
                int left = coords.x - 1;
                int right = coords.x + 1;

                if (left > 0)
                {
                    const Vec2 start = { .x = left, .y = coords.y };
                    if (try_create_beam(&beams, &new_beams, &start, width))
                        changed = true;
                }

                if (right < width)
                {
                    const Vec2 start = { .x = right, .y = coords.y };
                    if (try_create_beam(&beams, &new_beams, &start, width))
                        changed = true;
                }

                cc_insert(&finished_beams, *end);
                result += 1;

                const int* split_count = cc_get(&splits, *end);
                if (split_count != NULL)
                {
                    cc_insert(&splits, *end, *split_count + 1);
                }
                else
                {
                    cc_insert(&splits, *end, 1);
                }
            }
            else
            {
                *end = new_end;
                changed = true;
            }
        }

        cc_for_each(&new_beams, start, end)
        {
            cc_insert(&beams, *start, *end);
        }

        cc_cleanup(&new_beams);
    } while (changed);

    cc_cleanup(&beams);
    cc_cleanup(&finished_beams);

    return result;
}

void day07_part1(void)
{
    Lines lines = read_lines("07_input.txt");
    if (!lines.lines) return;

    printf("%d", solve(&lines));

    free_lines(lines);
}

void day07_part2(void)
{
    Lines lines = read_lines("07_input.txt");
    if (!lines.lines) return;

    free_lines(lines);
}

bool day07_test_part1(void)
{
    Lines lines = read_lines("07_test_input.txt");
    if (!lines.lines) return false;

    int result = solve(&lines);

    free_lines(lines);
    return result == 21;
}

bool day07_test_part2(void)
{
    Lines lines = read_lines("07_test_input.txt");
    if (!lines.lines) return false;

    free_lines(lines);
    return false;
}

const Solution day07 = {
    day07_part1,
    day07_part2,
    day07_test_part1,
    day07_test_part2
};
