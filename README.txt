Acest program implementeaza simularea evolutiei unei colonii de bacterii.
Programul permite rularea atat intr-o versiune seriala, cat si in mai multe versiuni paralele optimizate.
Scopul principal este de a analiza performanta si eficienta executiei paralele pe grile de dimensiuni diferite.
Programul suporta doua moduri de rulare: normal (pentru masuratori de performanta) si interactiv (pentru vizualizarea evolutiei grilei).

Cele doua moduri de rulare pot fi schimbate in fisierul "Bacteria.h":
    #define DEBUG 0: Afiseaza pe ecran doar timpii, speed-upul si validarea(daca versiunea seriala afiseaza la fel ca si cele paralele)
    #define DEBUG 1: Afiseaza pe ecran fiecare generatie a coloniei de bacterii, in functie de numarul de generatii date

Compilare si rulare:
    Proiectul se va compila si va fi rulat cu urmatoarele comenzi:
    gcc -fopenmp -Wall -o p Bacteria.c BacteriaMain.c
    ./p bacteriaN.txt gen, unde "N" din "bacteriaN" reprezinta numarul de linii si de coloane a grid-ului, iar gen reprezinta numarul de generatii

    Exemplu: ./p bacteria10.txt 5

    La rularea programului se vor creea fisiere pentru versiunea seriala si pentru fiecare versiune paralela cu varianta finala a grid-ului
