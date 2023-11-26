// Task2
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2
// bariera ca variabila globala
pthread_barrier_t barrier;

void *f(void *arg) {
	int thread_id = *(int *)arg;

	// cod 1
	if (thread_id == 1) {
		printf("1\n");
	}

	// Toate threadurile trebuie sa fi executat primul if
	pthread_barrier_wait(&barrier);

	// cod 2
	if (thread_id == 0) {
		printf("2\n");
	}

	pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int i, r;
	void *status;
	pthread_t threads[NUM_THREADS];
	int arguments[NUM_THREADS];

	// Creare & initializare bariera
	r = pthread_barrier_init(&barrier, NULL, NUM_THREADS);
	if (r) {
		fprintf(stderr, "Eroare la crearea mutexului\n");
		return 1;
	}

	for (i = 0; i < NUM_THREADS; i++) {
		arguments[i] = i;
		r = pthread_create(&threads[i], NULL, f, &arguments[i]);

		if (r) {
			printf("Eroare la crearea thread-ului %d\n", i);
			exit(-1);
		}
	}

	for (i = 0; i < NUM_THREADS; i++) {
		r = pthread_join(threads[i], &status);

		if (r) {
			printf("Eroare la asteptarea thread-ului %d\n", i);
			exit(-1);
		}
	}

	return 0;
}
