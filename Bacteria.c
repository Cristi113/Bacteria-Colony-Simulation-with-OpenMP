#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "Bacteria.h"

int ROWS, COLS, generations; // Dimensiunile grilei si numarul de generatii
int *grid;                   // Grila pentru generatia curenta
int *new_grid;               // Grila pentru generatia urmatoare

// Functie pentru initializarea grilei cu valori 0
void initialize_matrix(int *matrix){
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            matrix[i * COLS + j] = 0;
        }
    }
}

// Functie pentru a printa grila curenta
void print_grid(void){
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            // Afiseaza 'X' daca exista bacterie, altfel afiseaza '.'
            printf("%c ", grid[i * COLS + j] ? 'X' : '.');
        }
        printf("\n");
    }
}

// Functie pentru a citi configuratia initiala dintr-un fisier
void init_grid_from_file(const char *filename){
    FILE *file = fopen(filename, "r");
    if (!file){
        fprintf(stderr, "Error opening file %s\n", filename); // Daca fisierul nu poate fi deschis
        exit(1); // Iesire din program cu eroare
    }

    // Citim dimensiunile grilei si numarul de generatii
    fscanf(file, "%d %d", &ROWS, &COLS);

    // Alocam memorie pentru grile
    grid = (int *)malloc(ROWS * COLS * sizeof(int));
    new_grid = (int *)malloc(ROWS * COLS * sizeof(int));
    if (!grid || !new_grid){
        fprintf(stderr, "Memory allocation error\n"); // Daca nu se poate aloca memorie
        exit(1); // Iesire din program cu eroare
    }
    // Citim grila din fisier
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            char c;
            fscanf(file, " %c", &c); // Citim un caracter(X sau .)
            grid[i * COLS + j] = (c == 'X') ? 1 : 0; // '1' pentru bacterie, 0 pentru celula goala
        }
    }

    fclose(file); // Inchidem fisierul
}

// Functie pentru a schimba pointerii intre generatii
void swap_ptr(int **p1, int **p2){
    int *tmp = *p1;
    *p1 = *p2;
    *p2 = tmp;
}

// Functie pentru a numara vecinii unei celule
int count_neighbors(int x, int y)
{
    int count = 0;
    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx != 0 && dy != 0)
            {
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < ROWS && ny >= 0 && ny < COLS)
                {
                    if (grid[nx * COLS + ny] == 1)
                        count++;
                }
            }
        }

    return count;
}

// Functie pentru simularea bacteriilor serial
void simulate_bacteria_serial(void){
    for (int gen = 0; gen < generations; gen++){
        for (int i = 0; i < ROWS; i++){
            for (int j = 0; j < COLS; j++){
                int neighbors = count_neighbors(i, j); // Numaram vecinii fiecarei celule
                new_grid[i * COLS + j] = 0; // Initializam celula ca fiind goala

                if (grid[i * COLS + j] == 1){ // Daca exista bacterie in celula curenta
                    if (neighbors == 2 || neighbors == 3){
                        new_grid[i * COLS + j] = 1; // Bacteria supravietuieste
                    }
                }
                else{ // Daca celula este goala
                    if (neighbors == 3){
                        new_grid[i * COLS + j] = 1; // Apare o noua bacterie
                    }
                }
            }
        }
        swap_ptr(&grid, &new_grid); // Schimbam grilele pentru urmatoarea generatie

        #if DEBUG == 1
            printf("Generation %d:\n", gen + 1); // Print generation number (starting from 1)
            print_grid();  // Assuming print_grid() is a function that prints the grid
        #endif
    }
}

// Functie pentru simularea bacteriilor paralel cu politica dinamica
void simulate_bacteria_parallel_v1(){
    for (int gen = 0; gen < generations; gen++){
#pragma omp parallel for schedule(dynamic, 1) num_threads(NUM_THREADS)// cu dimensiune de chunk 1
        for (int i = 0; i < ROWS; i++){
            for (int j = 0; j < COLS; j++){
                int neighbors = count_neighbors(i, j);
                if (grid[i * COLS + j] == 1){
                    if (neighbors == 2 || neighbors == 3){
                        new_grid[i * COLS + j] = 1;
                    }else{
                        new_grid[i * COLS + j] = 0;
                    }
                }else{
                    if(neighbors == 3){
                        new_grid[i * COLS + j] = 1;
                    }else{
                        new_grid[i * COLS + j] = 0;
                    }
                }
            }
        }
        swap_ptr(&grid, &new_grid); // Schimba grilele pentru urmatoarea generatie
    }
}

// Functie pentru simularea bacteriilor cu fire persistente
void simulate_bacteria_parallel_v2(){
#pragma omp parallel num_threads(NUM_THREADS)
    {
        for (int gen = 0; gen < generations; gen++){
#pragma omp for
            for (int i = 0; i < ROWS; i++){
                for (int j = 0; j < COLS; j++){
                    int neighbors = count_neighbors(i, j);
                    if (grid[i * COLS + j] == 1){
                        if (neighbors == 2 || neighbors == 3){
                            new_grid[i * COLS + j] = 1;
                        }else{
                            new_grid[i * COLS + j] = 0;
                        }
                    }else{
                        if (neighbors == 3){
                            new_grid[i * COLS + j] = 1;
                        }else{
                            new_grid[i * COLS + j] = 0;
                        }
                    }
                }
            }
            #pragma omp single
            // Operatie ce necesita sa fie executata o data
            swap_ptr(&grid, &new_grid); // Schimba grilele pentru urmatoarea generatie
        }
    }
}

void simulate_bacteria_parallel_v3(){
#pragma omp parallel num_threads(NUM_THREADS)
    {
    int local_n, rest;
    int my_rank = omp_get_thread_num();

    local_n = ROWS / NUM_THREADS;
    rest = (my_rank == NUM_THREADS - 1) ? ROWS % NUM_THREADS : 0;

    int start = my_rank * local_n;
    int end = start + local_n + rest;
    for (int gen = 0; gen < generations; gen++){
        for (int i = start; i < end; i++){ // Iteram pe randurile atribuite fiecarui thread
            for (int j = 0; j < COLS; j++){
                int neighbors = count_neighbors(i, j);
                if (grid[i * COLS + j] == 1){
                    if (neighbors == 2 || neighbors == 3){
                        new_grid[i * COLS + j] = 1;
                    }else{
                        new_grid[i * COLS + j] = 0;
                    }
                }else{
                    if (neighbors == 3){
                        new_grid[i * COLS + j] = 1;
                    }else{
                        new_grid[i * COLS + j] = 0;
                    }
                }
            } 
        }
            #pragma omp barrier

            #pragma omp single
            {
                swap_ptr(&grid, &new_grid);
            }

            #pragma omp barrier
    }
    
    }

    //swap_ptr(&grid, &new_grid); // Schimba grilele pentru urmatoarea generatie
}

// Functie pentru simularea bacteriilor folosind task-uri
void simulate_bacteria_parallel_v4() {
    for (int gen = 0; gen < generations; gen++){
#pragma omp parallel num_threads(NUM_THREADS)
        {
#pragma omp single
            {
                for (int i = 0; i < ROWS; i++){
                    #pragma omp task firstprivate(i)
                    {
                        for (int j = 0; j < COLS; j++){

                            int neighbors = count_neighbors(i, j);
                            if (grid[i * COLS + j] == 1){
                                if (neighbors == 2 || neighbors == 3){
                                    new_grid[i * COLS + j] = 1;
                                }else{
                                    new_grid[i * COLS + j] = 0;
                                }
                            }else{
                                if (neighbors == 3){
                                    new_grid[i * COLS + j] = 1;
                                }else{
                                    new_grid[i * COLS + j] = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        swap_ptr(&grid, &new_grid); // Schimba grilele pentru urmatoarea generatie
    }
}

// Functie pentru validarea rezultatelor intre simularea seriala si paralela
int validate_result(int *serial_grid, int *parallel_grid){
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            int idx = i * COLS + j;
            if (serial_grid[idx] != parallel_grid[idx]){ // Verificam daca exista diferente intre serial si paralel
                printf("Mismatch found at (%d, %d): Serial=%d, Parallel=%d\n", i, j, serial_grid[idx], parallel_grid[idx]);
                return 0;  // Validarea a esuat
            }
        }
    }
    return 1;  // Validarea a avut succes
}

// Functie pentru a salva grid-ul in fisier
void save_grid_to_file(const char *filename, const char *version) {

    char _filename[256];
    strncpy(_filename, filename, strlen(filename) - 4);
    _filename[strlen(filename) - 4] = '\0';  // Scoate extensia .txt

    char output_filename[512];
    if (version) {
        sprintf(output_filename, "%s_parallel_omp_%s.txt", _filename, version);
    } else {
        sprintf(output_filename, "%s_serial_out.txt", _filename);
    }

    FILE *file = fopen(output_filename, "w");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", output_filename);
        exit(1);
    }

    fprintf(file, "%d %d\n", ROWS, COLS);

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            fprintf(file, "%c ", grid[i * COLS + j] ? 'X' : '.');
        }
        fprintf(file, "\n");
    }

    fclose(file);
}