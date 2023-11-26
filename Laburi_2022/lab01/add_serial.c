#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int* arr;
int array_size;

// Structura pentru ca fiecare thread sa stie limitele
// bucatii de cod pe care o executa
typedef struct limits {
    int start;
    int end;
} limits;

void *add_100(void *arg) {
    limits *lim = (limits *)arg;

    // lim e pointer si acceseaza start cu ->
    for (int i = lim->start; i < lim->end; i++) {
        arr[i] += 100;
    }

    // Aici se face dezalocarea structurii
    free(lim);
    pthread_exit(NULL);
}

int min(int a, int b) { 
    return a < b ? a : b;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    array_size = atoi(argv[1]);

    // Alocare spatiu + populare arr
    arr = malloc(array_size * sizeof(*arr));
    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

    // Afisare arr initial
    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }

    // -----------------------------------------------------------
    // TODO: aceasta operatie va fi paralelizata
  	// for (int i = 0; i < array_size; i++) {
    //     arr[i] += 100;
    // }

    // Paralelizarea:
    long cores = sysconf(_SC_NPROCESSORS_CONF);
    pthread_t threads[cores];
    int r;

    for (int id = 0; id < cores; id++) {
        // In main aloc structura creata (malloc de sizeof pointer)
        limits *lim = malloc(sizeof(*lim));

        // Populare start & end
        // ATENTIE LA CUM LE SCRIU
        lim->start = id * ((double) array_size / cores);
        lim->end = min((id + 1) * ((double) array_size / cores), array_size);

        // Crearea thread-urilor. Se trimite functia, iar ca argument, structura
        // (care inglobeaza 2 membrii!)
        r = pthread_create(&threads[id], NULL, add_100, (void *)lim);        
        // Verificare
        if (r) {
            printf("Eroare la asteptarea thread-ului %d\n", id);
            exit(-1);
        }
    }

    for (int id = 0; id < cores; id++) {
        r = pthread_join(threads[id], NULL);
        if (r) {
            printf("Eroare la terminarea thread-ului %d\n", id);
            exit(-1);
        }
    }

    // -----------------------------------------------------------

    // Afisare arr dupa ce fiecare element s-a incrementat cu 100
    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }

    free(arr);
  	pthread_exit(NULL);

    return 0;
}
