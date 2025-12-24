#include "08.h"

#include <float.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC_NO_SHORT_NAMES
#include <cc.h>

#include "utils.h"
#include "types.h"

typedef struct
{
    int x;
    int y;
    int z;
} Vec3;

typedef struct
{
    size_t a, b;
    float distance;
} Edge;

static int compare_edges(const void* a, const void* b)
{
    const Edge* edge_a = (const Edge*)a;
    const Edge* edge_b = (const Edge*)b;

    return edge_a->distance - edge_b->distance;
}

static float distance(const Vec3* a, const Vec3* b)
{
    ull dx = a->x - b->x;
    ull dy = a->y - b->y;
    ull dz = a->z - b->z;
    return (float)sqrt((double)(dx * dx + dy * dy + dz * dz));
}

static int find(int* circuits, int i)
{
    if (circuits[i] < 0) return i;
    return circuits[i] = find(circuits, circuits[i]);
}

static void union_set(int* circuits, int a, int b)
{
    int root_a = find(circuits, a);
    int root_b = find(circuits, b);

    if (root_a != root_b) circuits[root_b] = root_a;
}

static void init(const Lines* lines, Vec3** out_boxes, Edge** out_edges, size_t* out_edge_count, int** out_circuits)
{
    size_t box_count = lines->count;
    Vec3* boxes = malloc(box_count * sizeof(Vec3));
    for (size_t i = 0; i < box_count; ++i)
    {
        Vec3* box = &boxes[i];
        sscanf_s(lines->lines[i], "%d,%d,%d", &box->x, &box->y, &box->z);
    }

    size_t edge_count = (box_count * (box_count - 1)) / 2;
    Edge* edges = malloc(edge_count * sizeof(Edge));
    size_t e = 0;
    for (size_t i = 0; i < box_count - 1; ++i)
    {
        for (size_t j = i + 1; j < box_count; ++j)
        {
            edges[e].a = i;
            edges[e].b = j;
            edges[e++].distance = distance(&boxes[i], &boxes[j]);
        }
    }

    qsort(edges, edge_count, sizeof(Edge), compare_edges);

    int* circuits = malloc(box_count * sizeof(int));
    for (size_t i = 0; i < box_count; ++i)
        circuits[i] = -1;

    *out_boxes = boxes;
    *out_edges = edges;
    *out_edge_count = edge_count;
    *out_circuits = circuits;
}

static ull solve_p1(const Lines* lines, const size_t iterations)
{
    Vec3* boxes;
    Edge* edges;
    size_t edge_count;
    int* circuits;
    init(lines, &boxes, &edges, &edge_count, &circuits);

    for (size_t i = 0; i < iterations; ++i)
         union_set(circuits, edges[i].a, edges[i].b);

    int* sizes = calloc(lines->count, sizeof(int));
    for (size_t i = 0; i < lines->count; ++i)
    {
        int root = find(circuits, i);
        ++sizes[root];
    }

    qsort(sizes, lines->count, sizeof(int), compare_int_desc);

    ull result = 1;
    for (size_t i = 0; i < 3; ++i)
        result *= sizes[i];

    free((void*)boxes);
    free((void*)edges);
    free(circuits);

    return result;
}

static ull solve_p2(const Lines* lines)
{
    Vec3* boxes;
    Edge* edges;
    size_t edge_count;
    int* circuits;
    init(lines, &boxes, &edges, &edge_count, &circuits);

    int set_count = lines->count;
    size_t i = 0;
    while (set_count > 1)
    {
        int a = find(circuits, edges[i].a);
        int b = find(circuits, edges[i].b);

        if (a != b) --set_count;

        union_set(circuits, edges[i].a, edges[i].b);
        ++i;
    }

    Edge* last_edge = &edges[i - 1];
    ull result = boxes[last_edge->a].x * boxes[last_edge->b].x;

    free((void*)boxes);
    free((void*)edges);
    free(circuits);

    return result;
}

void day08_part1(void)
{
    Lines lines = read_lines("08_input.txt");
    if (!lines.lines) return;

    printf("%llu", solve_p1(&lines, 1000));

    free_lines(lines);
}

void day08_part2(void)
{
    Lines lines = read_lines("08_input.txt");
    if (!lines.lines) return;

    printf("%llu", solve_p2(&lines));

    free_lines(lines);
}

bool day08_test_part1(void)
{
    Lines lines = read_lines("08_test_input.txt");
    if (!lines.lines) return false;

    ull result = solve_p1(&lines, 10);

    free_lines(lines);
    return result == 40;
}

bool day08_test_part2(void)
{
    Lines lines = read_lines("08_test_input.txt");
    if (!lines.lines) return false;

    ull result = solve_p2(&lines);

    free_lines(lines);
    return result == 25272;
}

const Solution day08 = {
    day08_part1,
    day08_part2,
    day08_test_part1,
    day08_test_part2
};
