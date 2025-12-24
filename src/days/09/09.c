#include "09.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "types.h"

typedef struct
{
    ll x;
    ll y;
} Vec2;

typedef struct
{
    Vec2 pos;
    Vec2 size;
} Rect;

static Vec2* parse(const Lines* lines)
{
    Vec2* tiles = malloc(lines->count * sizeof(Vec2));
    for (size_t i = 0; i < lines->count; ++i)
    {
        Vec2* tile = &tiles[i];
        sscanf_s(lines->lines[i], "%lld,%lld", &tile->x, &tile->y);
    }

    return tiles;
}

static ull solve_p1(const Lines* lines)
{
    Vec2* tiles = parse(lines);

    ull max_area = 0;
    for (size_t i = 0; i < lines->count - 1; ++i)
    {
        for (size_t j = i + 1; j < lines->count; ++j)
        {
            Vec2* a = &tiles[i];
            Vec2* b = &tiles[j];

            ull width = llabs(a->x - b->x) + 1;
            ull height = llabs(a->y - b->y) + 1;
            ull area = width * height;
            if (area > max_area)
                max_area = area;
        }
    }

    free(tiles);

    return max_area;
}
static ull solve_p2(const Lines* lines)
{
    Vec2* tiles = parse(lines);

    ull max_area = 0;

    // TODO: finish me!
    // I think this approach should work:
    //  1. Generate polygon from input, recording min and max X and Y
    //  2. Partition polygon
    //  3. For each pair of vertices in the input:
    //      3a. Define rect
    //      3b. Do horizontal raycasts starting at min_x for each row of the rect
    //      3c. If the left or right edges of the rect fall outside of the polygon based on 3b, skip this rect
    //          (Note: partitioning should enable skipping parts of the polygon that are completely external to the rect)
    //      3d. Otherwise, if the rect's area exceeds max_area, update max_area

    free(tiles);

    return max_area;
}

void day09_part1(void)
{
    Lines lines = read_lines("09_input.txt");
    if (!lines.lines) return;

    ull result = solve_p1(&lines);
    printf("%lld", result);

    free_lines(lines);
}

void day09_part2(void)
{
    Lines lines = read_lines("09_input.txt");
    if (!lines.lines) return;

    ull result = solve_p2(&lines);
    printf("%llu", result);

    free_lines(lines);
}

bool day09_test_part1(void)
{
    Lines lines = read_lines("09_test_input.txt");
    if (!lines.lines) return false;

    ull result = solve_p1(&lines);

    free_lines(lines);
    return result == 50;
}

bool day09_test_part2(void)
{
    Lines lines = read_lines("09_test_input.txt");
    if (!lines.lines) return false;

    ull result = solve_p2(&lines);
    printf("%llu ", result);

    free_lines(lines);
    return result == 24;
}

const Solution day09 = {
    day09_part1,
    day09_part2,
    day09_test_part1,
    day09_test_part2
};
