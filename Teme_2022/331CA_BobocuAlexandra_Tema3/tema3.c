#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_COORDINATORS    4

#define SIZE_TAG            0
#define DATA_TAG            1
#define COORD_TAG           2
#define SIZE_TAG_WORKER     3
#define DATA_TAG_WORKER     4

/**
 * Functie pentru afisarea de mesaje.
 * @param sursa
 * @param destinatia
 */
void display_message(int source, int destination) {
    printf("M(%d,%d)\n", source, destination);
}


/**
 * Functie pentru citirea din fisierele de tip "cluster.txt".
 * @param topologia
 * @param vectorul cu numarul de workeri
 * @param rank-ul coordonatorului
 * @param index pentru vectorul cu numarul de workeri
 */
void read_from_file(int **topology, int *num_workers_array, int rank, int index) {
    // Numele fisierului
    char cluster_filename[100];
    sprintf(cluster_filename, "cluster%d.txt", rank);

    FILE *file = fopen(cluster_filename, "r");

    // Fiecare linie din fisier
    char line_from_file[10];

    // Nu s-a putut deschide fisierul
    if (file == NULL) {
        printf("Error: Cannot open file! \n");
        exit(1);
    }

    // Citirea numarului de workeri din fisier
    int num_workers = atoi(fgets(line_from_file, sizeof(line_from_file), file));

    // Popularea vectorului cu numarul de workeri pentru fiecare coordonator
    num_workers_array[index] = num_workers;

    // Coordonatorul cu un anumit rank detine un vector cu ID-urile workerilor care ii se subordoneaza
    topology[rank] = (int *) calloc(num_workers, sizeof(int));

    int id_worker;

    // Initializarea vector auxiliar care contine ID-urile workerilor
    int id_worker_array[num_workers];
    for (int i = 0; i < num_workers; i++) {
        id_worker_array[i] = 0;
    }

    int cnt = 0;
    while(fgets(line_from_file, sizeof(line_from_file), file) != NULL) {
        // Citirea ID-ului workerului din fisier
        id_worker = atoi(line_from_file);
        id_worker_array[cnt] = id_worker;
        cnt++;
    }

    // Completarea topologiei pe coloane cu ID-urile workerilor
    for (int i = 0; i < num_workers; i++) {
        topology[rank][i] = id_worker_array[i];
    }

    fclose(file);
}


/**
 * Functie pentru trimiterea informatiilor intre coordonatori.
 * @param topologia
 * @param rank-ul coordonatorului
 * @param sursa
 * @param destinatia
 * @param vectorul cu numarul de workeri
 */
void send_data_to_coord(int** topology, int rank, int source, int destination, int *num_workers_array) {
    // Se trimite mai intai numarul de workeri
    MPI_Send(&num_workers_array[rank], 1, MPI_INT, destination, SIZE_TAG, MPI_COMM_WORLD);
    display_message(source, destination);

    // Se trimit dupa ID-urile workerilor
    MPI_Send(topology[rank], num_workers_array[rank], MPI_INT, destination, DATA_TAG, MPI_COMM_WORLD);
    display_message(source, destination);
}


/**
 * Functie pentru primirea informatiilor de la coordonatori.
 * Aceste informatii pot fi primite de coordonatori.
 * @param topologia
 * @param rank-ul coordonatorului
 * @param sursa
 * @param destinatia
 * @param vectorul cu numarul de workeri
 */
void recv_data_from_coord(int** topology, int rank, int source, int destination, int *num_workers_array) {
    // Se primeste mai intai numarul de workeri de la coordonator
    MPI_Recv(&num_workers_array[rank], 1, MPI_INT, source, SIZE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Aloc linia specifica coordonatorului din topologie cu spatiu pentru
    // numarul de workeri pe care ii are, urmand sa populez acea linie cu ID-urile
    // workerilor
    topology[rank] = calloc(num_workers_array[rank], sizeof(int));
    MPI_Recv(topology[rank], num_workers_array[rank], MPI_INT, source, DATA_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}


/**
 * Functie pentru trimiterea informatiilor de la coordonatori catre workeri.
 * @param topologia
 * @param rank-ul coordonatorului
 * @param vectorul cu numarul de workeri
 */
void send_data_from_coord_to_worker(int** topology, int rank, int *num_workers_array) {
    // Mai intai, trimit catre workeri rank-ul coordonatorului, deci ma asigur
    // ca trimit informatiile catre toti workerii din cadrul coordonatorului
    // cu un anumit rank
    for (int i = 0; i < num_workers_array[rank]; i++) {
        MPI_Send(&rank, 1, MPI_INT, topology[rank][i], COORD_TAG, MPI_COMM_WORLD);
        display_message(rank, topology[rank][i]);

        // Parcurg coordonatorii si trimit mai intai dimensiunea (numarul de workeri)
        for (int j = 0; j < NUM_COORDINATORS; j++) {
            MPI_Send(&num_workers_array[j], 1, MPI_INT, topology[rank][i], SIZE_TAG_WORKER, MPI_COMM_WORLD);
            display_message(rank, topology[rank][i]);

            // Apoi, trimit topologia
            MPI_Send(topology[j], num_workers_array[j], MPI_INT, topology[rank][i], DATA_TAG_WORKER, MPI_COMM_WORLD);
            display_message(rank, topology[rank][i]);
        }
    }
}


/**
 * Functie pentru primirea informatiilor de workeri de la coordonatori.
 * @param topologia
 * @param rank-ul workerului
 * @param vectorul cu numarul de workeri
 * @param coordonatorului unui anumit worker
 */
void recv_data_from_coord_to_worker(int** topology, int rank, int *num_workers_array, int *parent) {
    // Mai intai, workerul primesc rank-ul coordonatorului
    MPI_Recv(parent, 1, MPI_INT, MPI_ANY_SOURCE, COORD_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Iau pe rand fiecare coordonator, iar workerul primeste numarul de workeri pe care
    // ii are in subordine coordonatorul. Voi aloca spatiul corespunzator cu numarul de
    //  workeri pe care ii are coordonatorul si voi popula acel spatiu cu topologia
    for (int i = 0; i < NUM_COORDINATORS; i++) {
        MPI_Recv(&num_workers_array[i], 1, MPI_INT, *parent, SIZE_TAG_WORKER, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        topology[i] = calloc(num_workers_array[i], sizeof(int));
        MPI_Recv(topology[i], num_workers_array[i], MPI_INT, *parent, DATA_TAG_WORKER, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}


int main (int argc, char *argv[]) {
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Aloc spatiu pentru topologie
    int **topology = (int **)calloc(NUM_COORDINATORS, sizeof(int*));
    // Aloc spatiu pentru vectorul cu numarul de workeri pentru fiecare coordonator
    int *num_workers_array = (int *)calloc(NUM_COORDINATORS, sizeof(int));

    // Daca procesul este de tip coordonator
    if (rank < NUM_COORDINATORS) {
        // Se completeaza topologia cu vectorul ce contine ID-urile workerilor, fiecare
        // coordonator de cluster in parte avand propriul sau vector de workeri
        if (rank == 0) {
            read_from_file(topology, num_workers_array, rank, 0);
        }

        if (rank == 1) {
            read_from_file(topology, num_workers_array, rank, 1);
        }

        if (rank == 2) {
            read_from_file(topology, num_workers_array, rank, 2);
        }

        if (rank == 3) {
            read_from_file(topology, num_workers_array, rank, 3);
        }

        // Coordonatorii transmit vecinilor direct conectati topologia
        if (rank == 0) {
            // Coordonatorul 0 trimite informatiile sale coordonatorului 3
            send_data_to_coord(topology, 0, 0, 3, num_workers_array);
            // Coordonatorul 3 il informeaza inapoi pe coordonatorul 0 despre coordonatorii
            // 1, 2 si 3
            recv_data_from_coord(topology, 1, 3, 0, num_workers_array);
            recv_data_from_coord(topology, 2, 3, 0, num_workers_array);
            recv_data_from_coord(topology, 3, 3, 0, num_workers_array);
        } else if (rank == 3) {
            // Coordonatorul 3 primeste informatii de la coordonatorul 0 despre 0
            recv_data_from_coord(topology, 0, 0, 3, num_workers_array);
            // Coordonatorul 3 trimite informatii catre coordonatorul 2 mai intai despre 0,
            // apoi despre el insusi
            send_data_to_coord(topology, 0, 3, 2, num_workers_array);
            send_data_to_coord(topology, 3, 3, 2, num_workers_array);
            // Coordonatorul 3 primeste informatii de la coordonatorul 2 despre 1 (care este direct
            // conectat cu 2) si despre 2
            recv_data_from_coord(topology, 1, 2, 3, num_workers_array);
            recv_data_from_coord(topology, 2, 2, 3, num_workers_array);
            // Coordonatorul 3 trimite informatii catre coordonatorul 0 despre coordonatorii 1, 2 si 3,
            // abia dupa ce a aflat informatii despre 1 si 2
            send_data_to_coord(topology, 1, 3, 0, num_workers_array);
            send_data_to_coord(topology, 2, 3, 0, num_workers_array);
            send_data_to_coord(topology, 3, 3, 0, num_workers_array);
        } else if (rank == 2) {
            // Coordonatorul 2 primeste informatii de la coordonatorul 3
            // despre coordonatorii 0 si 3
            recv_data_from_coord(topology, 0, 3, 2, num_workers_array);
            recv_data_from_coord(topology, 3, 3, 2, num_workers_array);
            // Coordonatorul 2 va trimite catre coordonatorul 1 informatii
            // despre coordonatorii 0, 3 si 2
            send_data_to_coord(topology, 0, 2, 1, num_workers_array);
            send_data_to_coord(topology, 3, 2, 1, num_workers_array);
            send_data_to_coord(topology, 2, 2, 1, num_workers_array);
            // Coordonatorul 2 primeste informatii de la coordonatorul 1 despre el insusi
            recv_data_from_coord(topology, 1, 1, 2, num_workers_array);
            // Coordonatorul 2 va trimite catre coordonatorul 3 informatii despre 1 si 2
            send_data_to_coord(topology, 1, 2, 3, num_workers_array);
            send_data_to_coord(topology, 2, 2, 3, num_workers_array);
        } else if (rank == 1) {
            // Coordonatorul 1 primeste informatii de la coordonatorul 2 cel mai apropiat
            // despre coordonatorii ceilalti, 0, 3 si 2
            recv_data_from_coord(topology, 0, 2, 1, num_workers_array);
            recv_data_from_coord(topology, 3, 2, 1, num_workers_array);
            recv_data_from_coord(topology, 2, 2, 1, num_workers_array);
            // Coordonatorul 1 va trimite catre 2 informatiile sale (in final, coordonatorul
            // 1 va contine toata topologia, pe care o va trimite pe cale intoarsa)
            send_data_to_coord(topology, 1, 1, 2, num_workers_array);
        }

        // Afisare topologie din cadrul coordonatorilor
        printf("%d -> ", rank);
        for (int i = 0; i < NUM_COORDINATORS; i++) {
            printf("%d:", i);
            for (int j = 0; j < num_workers_array[i]; j++) {
                if(num_workers_array[i] == 1 || j == num_workers_array[i] - 1) {
                    printf("%d ", topology[i][j]);
                } else {
                    printf("%d,", topology[i][j]);
                }
            }
        }
        printf("\n");

        // Coordonatorii trimit informatii despre topologie workerilor
        send_data_from_coord_to_worker(topology, rank, num_workers_array);

    } else {
        // Procesul este de tip worker

        int parent = 0;
        // Workerii primesc informatii despre topologie de la coordonatori
        recv_data_from_coord_to_worker(topology, rank, num_workers_array, &parent);

        // Afisare topologie din cadrul workerilor
        printf("%d -> ", rank);
        for (int i = 0; i < NUM_COORDINATORS; i++) {
            printf("%d:", i);
            for (int j = 0; j < num_workers_array[i]; j++) {
                if(num_workers_array[i] == 1 || j == num_workers_array[i] - 1) {
                    printf("%d ", topology[i][j]);
                } else {
                    printf("%d,", topology[i][j]);
                }
            }
        }
        printf("\n");
    }

    // Dezalocare memorie
    free(num_workers_array);

    for (int i = 0; i < NUM_COORDINATORS; i++) {
        free(topology[i]);
    }
    free(topology);

    MPI_Finalize();
    return EXIT_SUCCESS;
}
