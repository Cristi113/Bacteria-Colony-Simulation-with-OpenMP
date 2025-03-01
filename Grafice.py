import matplotlib.pyplot as plt

data = {
    "10x10": {
        2: [1.12500, 1.50000, 1.29375, 0.45796],
        4: [1.30197, 1.75972, 1.27778, 0.23621],
        8: [1.12851, 1.54009, 1.01926, 0.13410]
    },
    "1000x1000": {
        2: [1.70978, 1.79472, 1.75548, 1.73451],
        4: [2.48138, 2.49180, 2.59198, 3.03365],
        8: [4.00698, 3.91574, 3.89243, 4.90808]
    },
    "2000x2000": {
        2: [1.70897, 1.69190, 1.65557, 1.65306],
        4: [2.44418, 2.60756, 2.52883, 2.86374],
        8: [4.32469, 4.15026, 4.15877, 5.28192]
    },
    "3000x3000": {
        2: [1.74006, 1.72519, 1.73478, 1.70471],
        4: [2.69936, 2.66071, 2.66936, 2.85286],
        8: [4.51105, 4.36712, 4.42650, 5.32033]
    },
    "5000x5000": {
        2: [1.69545, 1.72712, 1.82734, 1.80205],
        4: [2.88569, 2.83175, 2.86896, 3.01562],
        8: [4.57660, 4.53974, 4.57466, 5.01019]
    }
}

threads = [2, 4, 8]

for grid_size, results in data.items():
    plt.figure(figsize=(10, 6))
    
    for i, version in enumerate(["Parallel V1", "Parallel V2", "Parallel V3", "Parallel V4"]):
        speedups = [results[thread][i] for thread in threads]
        plt.plot(threads, speedups, marker='o', label=version)
    
    plt.title(f"Speedup vs Threads for Grid Size {grid_size}")
    plt.xlabel("Number of Threads")
    plt.ylabel("Speedup")
    plt.legend()
    plt.grid(True)
    plt.show()
