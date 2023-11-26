#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Task1
/*
Apar n mesaje (de la 0 la n - 1), unde n = nr de threaduri
#define NUM_THREADS 2
*/

// Task2
/*
#define NUM_THREADS sysconf(_SC_NPROCESSORS_CONF)

void *f(void *arg) {
  	long id = *(long*)arg;

	// Task3
	for (int i = 0; i < 100; i++) {
		printf("Hello World din thread-ul %ld de la indicele %d!\n", id, i);
	}
  	
  	pthread_exit(NULL);
}
*/

// Task4
#define NUM_THREADS 2

void *f1(void *arg) {
  	long id = *(long*)arg;

	printf("Prima functie din thread-ul %ld!\n", id);
  	
  	pthread_exit(NULL);
}

void *f2(void *arg) {
  	long id = *(long*)arg;

	printf("A doua functie din thread-ul %ld!\n", id);
  	
  	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	pthread_t threads[NUM_THREADS];
  	int r;
  	long id;
  	void *status;
	long ids[NUM_THREADS];

	// Popularea vectorului de id-uri
	for (id = 0; id < NUM_THREADS; id++) {
		ids[id] = id;
  	}

	// Crearea separata a celor 2 threaduri + verificari de creare corecta
	r = pthread_create(&threads[0], NULL, f1, &ids[0]);

	if (r) {
		printf("Eroare la crearea thread-ului 0\n");
		exit(-1);
	}

	r = pthread_create(&threads[1], NULL, f2, &ids[1]);

	if (r) {
		printf("Eroare la crearea thread-ului 1\n");
		exit(-1);
	}

	// Unirea celor 2 threaduri la final
	for (id = 0; id < 2; id++) {
		r = pthread_join(threads[id], NULL);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
	}

	// Tasks 2 & 3
	/*
  	for (id = 0; id < NUM_THREADS; id++) {
		ids[id] = id; 
		r = pthread_create(&threads[id], NULL, f, &ids[id]);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

  	for (id = 0; id < NUM_THREADS; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}
	*/

  	pthread_exit(NULL);
}
