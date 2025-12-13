#include "06.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CC_NO_SHORT_NAMES
#include <cc.h>

#include "utils.h"
#include "maths.h"
#include "types.h"

#define MAX_ROW_SIZE 1000

typedef enum
{
    PRODUCT,
    SUM
} Op;

typedef struct Problem
{
    ull* operands;
    size_t count;
    Op op;
} Problem;

static Problem* get_or_create_problem(cc_vec(Problem)* problems, size_t problem_id, size_t problem_size)
{
    if (problem_id >= cc_size(problems))
    {
        Problem new_prob = {
            .operands = malloc(sizeof(ull) * problem_size),
            .count = 0
        };

        cc_push(problems, new_prob);
    }

    return (Problem*)cc_get(problems, problem_id);
}

static ull solve_p1(const Lines* lines)
{
    ull result = 0;

    size_t problem_size = lines->count - 1;
    cc_vec(Problem) problems;
    cc_init(&problems);

    for (size_t i = 0; i < lines->count; ++i)
    {
        const char* line = lines->lines[i];
        size_t problem_id = 0;
        char number[8] = {0};
        size_t digit_index = 0;

        for (size_t j = 0; j < strlen(line); ++j)
        {
            Problem* problem = get_or_create_problem(&problems, problem_id, problem_size);
            char c = line[j];

            if (i < lines->count - 1)
            {
                // Operand
                if (isdigit(c))
                {
                    number[digit_index++] = c;
                }
                else if (digit_index > 0)
                {
                    number[digit_index] = 0;
                    ull operand = strtoull(number, NULL, 10);
                    problem->operands[problem->count++] = operand;
                    number[0] = 0;
                    digit_index = 0;
                    problem_id++;
                }
            }
            else
            {
                // Operator
                if (c == '*')
                {
                    problem->op = PRODUCT;
                    problem_id++;

                }
                else if (c == '+')
                {
                    problem->op = SUM;
                    problem_id++;
                }
            }
        }
    }

    cc_for_each(&problems, problem)
    {
        switch (problem->op)
        {
            case SUM: result += sum_ull(problem->operands, problem->count); break;
            case PRODUCT: result += product_ull(problem->operands, problem->count); break;
        }

        free(problem->operands);
    }

    cc_cleanup(&problems);

    return result;
}

static ull solve_p2(const Lines* lines)
{
    ull result = 0;

    size_t line_length = strlen(lines->lines[0]);

    ull operands[4];
    size_t operand_index = 0;
    Op op = SUM;
    bool complete;
    char number[8];
    size_t digit_index = 0;
    for (size_t i = line_length - 1; i != (size_t)-1; --i)
    {
        complete = false;
        for (size_t j = 0; j < lines->count; ++j)
        {
            const char c = lines->lines[j][i];
            bool is_op_line = j == lines->count - 1;
            if (is_op_line)
            {
                if (c == '+')
                {
                    op = SUM;
                    complete = true;
                }
                else if (c == '*')
                {
                    op = PRODUCT;
                    complete = true;
                }

                if (digit_index > 0)
                {
                    number[digit_index] = 0;
                    operands[operand_index++] = strtoull(number, NULL, 10);
                    number[0] = 0;
                    digit_index = 0;
                }
            }
            else if (isdigit(c))
            {
                number[digit_index++] = c;
            }
        }

        if (complete)
        {
            switch (op)
            {
                case SUM: result += sum_ull(operands, operand_index); break;
                case PRODUCT: result += product_ull(operands, operand_index); break;
            }

            operand_index = 0;
            complete = false;
        }
    }

    return result;
}

void day06_part1(void)
{
    Lines lines = read_lines("06_input.txt");
    if (!lines.lines) return;

    ull result = solve_p1(&lines);
    printf("%llu", result);

    free_lines(lines);
}

void day06_part2(void)
{
    Lines lines = read_lines("06_input.txt");
    if (!lines.lines) return;

    ull result = solve_p2(&lines);
    printf("%llu", result);

    free_lines(lines);
}

bool day06_test_part1(void)
{
    Lines lines = read_lines("06_test_input.txt");
    if (!lines.lines) return false;

    ull result = solve_p1(&lines);

    free_lines(lines);
    return result == 4277556;
}

bool day06_test_part2(void)
{
    Lines lines = read_lines("06_test_input.txt");
    if (!lines.lines) return false;

    ull result = solve_p2(&lines);

    free_lines(lines);
    return result == 3263827;
}

const Solution day06 = {
    day06_part1,
    day06_part2,
    day06_test_part1,
    day06_test_part2
};
