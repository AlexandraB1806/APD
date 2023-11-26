// Task3
// DE PARALELIZAT PRIMA BUCLA
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// adaugate
pthread_mutex_t **mutex;

int min(int a, int b) {
	return a < b ? a : b;
}

int N;
int P;
int **a;
int **b;
int **c;

void *thread_function(void *arg)
{
	int thread_id = *(int *)arg;

	// Formulele pentru start & end
	int start = thread_id * ((double) N / P);
	int end = min((thread_id + 1) * ((double) N / P), N);

	for (int i = start; i < end; i++) {
		for (int j = 0; j < N; j++) {
			int tmp = 0;
			for (int k = 0; k < N; k++) {
				tmp += a[i][k] * b[k][j];
			}
			pthread_mutex_lock(&mutex[i][j]);
			c[i][j] += tmp;
			pthread_mutex_unlock(&mutex[i][j]);
		}
	}

	pthread_exit(NULL);
}

void get_args(int argc, char **argv)
{
	if(argc < 3) {
		printf("Numar insuficient de parametri: ./program N P\n");
		exit(1);
	}

	N = atoi(argv[1]);
	P = atoi(argv[2]);
}

void init()
{
	a = malloc(sizeof(int *) * N);
	b = malloc(sizeof(int *) * N);
	c = malloc(sizeof(int *) * N);

	// adaugat
	mutex = malloc(sizeof(*mutex) * N);

	if (a == NULL || b == NULL || c == NULL || mutex == NULL) {
		printf("Eroare la malloc!");
		exit(1);
	}

	int i, j;
	for (i = 0; i < N; i++) {
		a[i] = malloc(sizeof(int) * N);
		b[i] = malloc(sizeof(int) * N);
		c[i] = malloc(sizeof(int) * N);

		// adaugat
		mutex[i] = malloc(sizeof(**mutex) * N);

		if(a[i] == NULL || b[i] == NULL || c[i] == NULL || mutex[i] == NULL) {
			printf("Eroare la malloc!");
			exit(1);
		}
	}

	for (i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			c[i][j] = 0;

			if(i <= j) {
				a[i][j] = 1;
				b[i][j] = 1;
			} else {
				a[i][j] = 0;
				b[i][j] = 0;
			}
		}
	}
}

void print(int **mat)
{
	int i, j;

	for (i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			printf("%i\t", mat[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char *argv[])
{
	int i;
	int r;

	get_args(argc, argv);
	init();

	// adaugat
	for (i = 0 ; i < N ; ++i) {
		for (int j = 0 ; j < N ; ++j) {
			r = pthread_mutex_init(&mutex[i][j], NULL);
			if (r) {
				fprintf(stderr, "Failed to initialized a mutex!\n");
				return 1;
			}
		}
	}

	pthread_t tid[P];
	int thread_id[P];

	for (i = 0; i < P; i++) {
		thread_id[i] = i;
		pthread_create(&tid[i], NULL, thread_function, &thread_id[i]);
	}

	for (i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	print(c);

	return 0;
}
