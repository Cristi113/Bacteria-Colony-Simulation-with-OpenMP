#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "Bacteria.h"

int main(int argc, char *argv[]){
    struct timespec start, end;

#if DEBUG == 0
    double elapsed_serial, elapsed_parallel_v1, elapsed_parallel_v2;
    double elapsed_parallel_v3, elapsed_parallel_v4;
#endif

    // Verificare in caz ca utilizatorul nu a furnizat fisierul de intrare si numarul de generatii
    if (argc < 3){
        fprintf(stderr, "Usage: %s <filename> <generations>\n", argv[0]);
        exit(1);
    }

    // Initializam grila folosind fisierul dat ca argument
    init_grid_from_file(argv[1]);
    generations = atoi(argv[2]);

    // Copiz grid-ul initial pentru a-l folosi ca un reset point
    int *initial_grid = (int *)malloc(ROWS * COLS * sizeof(int)); 
    memcpy(initial_grid, grid, ROWS * COLS * sizeof(int));


    // Rulam simularea seriala si stocam rezultatul in grila seriala
    clock_gettime(CLOCK_MONOTONIC, &start);
    simulate_bacteria_serial();
    clock_gettime(CLOCK_MONOTONIC, &end);

#if DEBUG == 0
    elapsed_serial = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
#endif

    // Folosim direct grila ca rezultat al simularii seriale pentru comparatie
    int *serial_result = (int *)malloc(ROWS * COLS * sizeof(int));
    initialize_matrix(serial_result); // Initializam grila de rezultat
    for (int i = 0; i < ROWS; i++){ // Copiem valorile din grila curenta (dupa simularea seriala)
        for (int j = 0; j < COLS; j++){
            serial_result[i * COLS + j] = grid[i * COLS + j];
        }
    }

    // Initializam grila de rezultat paralel pentru validare
    memcpy(grid, initial_grid, ROWS * COLS * sizeof(int));
    int *parallel_result = (int *)malloc(ROWS * COLS * sizeof(int));

    // Simularea paralela V1
    clock_gettime(CLOCK_MONOTONIC, &start);
    simulate_bacteria_parallel_v1(); // Rulam simularea paralela V1
    clock_gettime(CLOCK_MONOTONIC, &end);

#if DEBUG == 0
    elapsed_parallel_v1 = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
#endif

    // Copiem rezultatele in grila paralela pentru validare
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            parallel_result[i * COLS + j] = grid[i * COLS + j];
        }
    }
    // Validam rezultatele si afisam daca testul a trecut sau nu

#if DEBUG == 0
    printf("Validation for V1: %s\n", validate_result(serial_result, parallel_result) ? "Passed" : "Failed");
#endif

    save_grid_to_file(argv[1], NULL);
    save_grid_to_file(argv[1], "V1");

    memcpy(grid, initial_grid, ROWS * COLS * sizeof(int));

    // Simularea paralela V2
    clock_gettime(CLOCK_MONOTONIC, &start);
    simulate_bacteria_parallel_v2(); // Rulam simularea paralela V2
    clock_gettime(CLOCK_MONOTONIC, &end);

#if DEBUG == 0
    elapsed_parallel_v2 = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
#endif

    // Copiem rezultatele in grila paralela pentru validare
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            parallel_result[i * COLS + j] = grid[i * COLS + j];
        }
    }
    // Validam rezultatele si afisam daca testul a trecut sau nu

#if DEBUG == 0
    printf("Validation for V2: %s\n", validate_result(serial_result, parallel_result) ? "Passed" : "Failed");
#endif
    save_grid_to_file(argv[1], "V2"); 

    memcpy(grid, initial_grid, ROWS * COLS * sizeof(int));

    // Simularea paralela V3
    clock_gettime(CLOCK_MONOTONIC, &start);
    simulate_bacteria_parallel_v3(); // Rulam simularea paralela V3
    clock_gettime(CLOCK_MONOTONIC, &end);

#if DEBUG == 0
    elapsed_parallel_v3 = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
#endif

    // Copiem rezultatele in grila paralela pentru validare
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            parallel_result[i * COLS + j] = grid[i * COLS + j];
        }
    }
    // Validam rezultatele si afisam daca testul a trecut sau nu

#if DEBUG == 0
    printf("Validation for V3: %s\n", validate_result(serial_result, parallel_result) ? "Passed" : "Failed");
#endif

    save_grid_to_file(argv[1], "V3"); 

    memcpy(grid, initial_grid, ROWS * COLS * sizeof(int));

    // Simularea paralela V4

    clock_gettime(CLOCK_MONOTONIC, &start);
    simulate_bacteria_parallel_v4(); // Rulam simularea paralela V4
    clock_gettime(CLOCK_MONOTONIC, &end);

#if DEBUG == 0
    elapsed_parallel_v4 = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
#endif

    // Copiem rezultatele in grila paralela pentru validare
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++){
            parallel_result[i * COLS + j] = grid[i * COLS + j];
        }
    }
    // Validam rezultatele si afisam daca testul a trecut sau nu

#if DEBUG == 0
    printf("Validation for V4: %s\n", validate_result(serial_result, parallel_result) ? "Passed" : "Failed");
#endif

    save_grid_to_file(argv[1], "V4"); 

#if DEBUG == 0
    printf("Serial Simulation Time: %.5f seconds\n", elapsed_serial);
    printf("Parallel V1 Simulation Time: %.5f seconds\n", elapsed_parallel_v1);
    printf("Parallel V2 Simulation Time: %.5f seconds\n", elapsed_parallel_v2);
    printf("Parallel V3 Simulation Time: %.5f seconds\n", elapsed_parallel_v3);
    printf("Parallel V4 Simulation Time: %.5f seconds\n", elapsed_parallel_v4);

    double speedup_v1 = elapsed_serial / elapsed_parallel_v1;
    double speedup_v2 = elapsed_serial / elapsed_parallel_v2;
    double speedup_v3 = elapsed_serial / elapsed_parallel_v3;
    double speedup_v4 = elapsed_serial / elapsed_parallel_v4;

    printf("Speedup for Parallel V1: %.5f\n", speedup_v1);
    printf("Speedup for Parallel V2: %.5f\n", speedup_v2);
    printf("Speedup for Parallel V3: %.5f\n", speedup_v3);
    printf("Speedup for Parallel V4: %.5f\n", speedup_v4);
#endif

    // Eliberam memoria alocata pentru grile
    free(initial_grid);
    free(grid);
    free(new_grid);
    free(serial_result);
    free(parallel_result);

    return 0;
}