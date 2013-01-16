#include "backend.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

#define check_valid(v1,v2) if (v1<1||v1>sudoku_table_side) return v2;
#define get_y(index) index / sudoku_table_side + 1
#define get_x(index) index % sudoku_table_side + 1
#define init_rand srand((unsigned)time(NULL))

typedef struct {
    int data[sudoku_table_num_count];
    int count;
} sudoku_table_data;

sudoku_table sudoku_init()
{
    sudoku_table_data * tb =
        (sudoku_table_data *) malloc(sizeof(sudoku_table_data));
    int i;

    for (i = 0; i < sudoku_table_num_count; ++i) {
        tb->data[i] = 0;
    }

    tb->count = 0;
    return (sudoku_table) tb;
}

void sudoku_clear(sudoku_table tb)
{
    free(tb);
}

static int sudoku_get_index(int x, int y)
{
    return (y - 1) * sudoku_table_side + (x - 1);
}

int sudoku_get_value(sudoku_table tb, int x, int y)
{
    check_valid(x, -1);
    check_valid(y, -1);
    sudoku_table_data * tbd = (sudoku_table_data *) tb;
    return tbd->data[sudoku_get_index(x, y)];
}

int sudoku_check_value(sudoku_table tb, int x, int y, int v)
{
    check_valid(x, 0);
    check_valid(y, 0);
    check_valid(v, 0);
    int i, j;
    int rx =
        ((x -
          1) / sudoku_table_minor_side) * sudoku_table_minor_side + 1,
        ry =
            ((y -
              1) / sudoku_table_minor_side) * sudoku_table_minor_side + 1;

    for (i = 1; i <= sudoku_table_side; ++i) {
        if (i == x)
            continue;

        if (sudoku_get_value(tb, i, y) == v)
            return 0;
    }

    for (i = 1; i <= sudoku_table_side; ++i) {
        if (i == y)
            continue;

        if (sudoku_get_value(tb, x, i) == v)
            return 0;
    }

    for (i = rx; i < rx + sudoku_table_minor_side; ++i) {
        for (j = ry; j < ry + sudoku_table_minor_side; ++j) {
            if (i == x && j == y)
                continue;

            if (sudoku_get_value(tb, i, j) == v)
                return 0;

        }
    }

    return 1;
}

int sudoku_set_value(sudoku_table tb, int x, int y, int v)
{
    if (!sudoku_check_value(tb, x, y, v))
        return 0;

    sudoku_table_data * tbd = (sudoku_table_data *) tb;
    int index = sudoku_get_index(x, y);

    if (tbd->data[index] == 0)
        ++tbd->count;

    tbd->data[index] = v;
    return 1;
}

int sudoku_delete_value(sudoku_table tb, int x, int y)
{
    check_valid(x, 0);
    check_valid(y, 0);
    sudoku_table_data * tbd = (sudoku_table_data *) tb;
    int index = sudoku_get_index(x, y);

    if (tbd->data[index] != 0)
        --tbd->count;

    tbd->data[index] = 0;
    return 1;
}

int sudoku_check_complete(sudoku_table tb)
{
    sudoku_table_data * tbd = (sudoku_table_data *) tb;
    return tbd->count == sudoku_table_num_count;
}

sudoku_table sudoku_read_file(FILE * f)
{
    sudoku_table tb = sudoku_init();
    sudoku_table_data * tbd = (sudoku_table_data *) tb;
    int i, c;

    for (i = 0; i < sudoku_table_num_count; ++i) {
        c = fgetc(f);

        if (isdigit(c) == 0) {
            return NULL;
        }

        tbd->data[i] = c - 48;

        if (c != 48)
            ++tbd->count;
    }

    return tb;
}

int sudoku_write_file(sudoku_table tb, FILE * f)
{
    sudoku_table_data * tbd = (sudoku_table_data *) tb;
    int i;

    for (i = 0; i < sudoku_table_num_count; ++i) {
        if (fputc(tbd->data[i] + 48, f) == EOF)
            return 0;
    }

    fflush(f);
    return 1;
}

static int get_next_usable(int * l)
{
    int count = 0, i, selected;

    for (i = 0; i < sudoku_table_side; ++i) {
        if (l[i] == 1)
            ++count;
    }

    if (count == 0)
        return -1;

    selected = rand() % count;
    count = 0;

    for (i = 0; i < sudoku_table_side; ++i) {
        if (l[i] == 1) {
            if (count == selected)
                break;

            ++count;
        }
    }

    l[i] = 0;
    return i + 1;
}

static sudoku_table sudoku_generate_table()
{
    sudoku_table tb = sudoku_init();
    init_rand;
    int usable_tb[sudoku_table_num_count][sudoku_table_side];
    int i, j, n;

    for (i = 0; i < sudoku_table_num_count; ++i)
        for (j = 0; j < sudoku_table_side; ++j)
            usable_tb[i][j] = 1;

    i = 0;

    while (i < sudoku_table_num_count) {
        n = get_next_usable(usable_tb[i]);

        if (n == -1) {
            sudoku_delete_value(tb, get_x(i), get_y(i));

            for (j = 0; j < sudoku_table_side; ++j)
                usable_tb[i][j] = 1;

            --i;
        } else if (sudoku_check_value(tb, get_x(i), get_y(i), n)) {
            sudoku_set_value(tb, get_x(i), get_y(i), n);
            ++i;
        }
    }

    return tb;
}

sudoku_table sudoku_random_init(int count)
{
    if (count < 0 || count > 81)
        return NULL;

    sudoku_table tb = sudoku_generate_table();
    int i, j;
    init_rand;

    for (i = 0; i < count; ++i) {
        while (1) {
            j = rand() % sudoku_table_num_count;

            if (sudoku_get_value(tb, get_x(j), get_y(j)) != 0)
                break;
        }

        sudoku_delete_value(tb, get_x(j), get_y(j));
    }

    return tb;
}
