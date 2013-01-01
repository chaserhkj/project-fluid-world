#ifndef _BACKEND_H_
#define _BACKEND_H_

#include <stdio.h>

#define sudoku_table_side 9

#define sudoku_table_minor_side 3

#define sudoku_table_num_count 81

#define sudoku_easy_count 20

#define sudoku_normal_count 40

#define sudoku_hard_count 60

typedef void * sudoku_table;

sudoku_table sudoku_init();

void sudoku_clear(sudoku_table tb);

int sudoku_get_value(sudoku_table tb, int x, int y);

int sudoku_check_value(sudoku_table tb, int x, int y, int v);

int sudoku_set_value(sudoku_table tb, int x, int y, int v);

int sudoku_delete_value(sudoku_table tb, int x, int y);

int sudoku_check_complete(sudoku_table tb);

sudoku_table sudoku_read_file(FILE * f);

int sudoku_write_file(sudoku_table tb, FILE * f);

sudoku_table sudoku_random_init(int count);

#endif /* _BACKEND_H_ */
