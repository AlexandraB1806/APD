// Task1
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2

int a = 0;
// mutex ca variabila globala
pthread_mutex_t mutex;

// Problema de sincronizare se rezolva in functia f
// Folosim mutex
void *f(void *arg)
{
	pthread_mutex_lock(&mutex);
	a += 2;
	pthread_mutex_unlock(&mutex);

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int i, r;
	void *status;
	pthread_t threads[NUM_THREADS];
	int arguments[NUM_THREADS];

	// Creare & initializare mutex
	r = pthread_mutex_init(&mutex, NULL);
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

	printf("a = %d\n", a);

	return 0;
}
