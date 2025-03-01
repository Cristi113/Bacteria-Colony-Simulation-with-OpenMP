#ifndef __BACTERIA_H
#define __BACTERIA_H

#define NUM_THREADS 8

#define DEBUG 1

extern int ROWS, COLS, generations;
extern int *grid, *new_grid;

void initialize_matrix(int *matrix);
void print_grid(void);
void init_grid_from_file(const char *filename);
void swap_ptr(int **p1, int **p2);
int count_neighbors(int x, int y);
void simulate_bacteria_serial(void);
void simulate_bacteria_parallel_v1(void);
void simulate_bacteria_parallel_v2(void);
void simulate_bacteria_parallel_v3(void);
void simulate_bacteria_parallel_v4(void);
int validate_result(int *serial_grid, int *parallel_grid);
void save_grid_to_file(const char *filename, const char *version);

#endif