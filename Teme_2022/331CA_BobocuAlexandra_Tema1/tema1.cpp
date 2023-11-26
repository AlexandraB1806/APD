// Bobocu Alexandra-Florentina, 331CA
#include "helper.hpp"

int main(int argc, char *argv[]) {
	if (argc < 4) {
		cerr << "Not enough number of parameters" << "\n";
		exit(1);
	}

	int r;

	// Declararea si popularea campurilor structurii aferente unui thread
	thread_args arg;

	arg.num_mappers = stoi(argv[1]);
	arg.num_reducers = stoi(argv[2]);

	// Crearea mutexului
	r = pthread_mutex_init(&arg.mutex, NULL);
	if (r) {
		cerr << "Create mutex error";
		exit(1);
	}

	// Calculare numar total de threaduri
	int num_threads = arg.num_mappers + arg.num_reducers;

	// Crearea barierei
	r = pthread_barrier_init(&arg.barrier, NULL, num_threads);
	if (r) {
		cerr << "Create barrier error";
		exit(1);
	}

	string file = argv[3];
	// Popularea cozii de cai de fisiere
	arg.files_queue = read_file(file);

	vector<map<int, vector<int>>> map_list;
	// Popularea listei de map-uri rezultate in urma operatiei de Map
	arg.partial_maps_list = map_list;

	// Declararea threadurilor de tip Mapper si Reducer
	pthread_t mappers[arg.num_mappers];
	pthread_t reducers[arg.num_reducers];

	// Creare threaduri
	for (int i = 0; i < max (arg.num_mappers, arg.num_reducers); i++) {
		if (i < arg.num_mappers) {
			r = pthread_create(&mappers[i], NULL, mapper_func, &arg);

			if (r) {
				cerr << "Create mapper thread error";
				exit(1);
			}
		}

		if (i < arg.num_reducers) {
			r = pthread_create(&reducers[i], NULL, reducer_func, &arg);

			if (r) {
				cerr << "Create reducer thread error";
				exit(1);
			}
		}
	}

	// Join threaduri
	for (int i = 0; i < max (arg.num_mappers, arg.num_reducers); i++) {
		if (i < arg.num_mappers) {
			r = pthread_join(mappers[i], NULL);

			if (r) {
				cerr << "Join mapper thread error";
				exit(1);
			}
		}

		if (i < arg.num_reducers) {
			r = pthread_join(reducers[i], NULL);

			if (r) {
				cerr << "Join reducer thread error";
				exit(1);
			}
		}
	}

	// Distrugerea mutexului si a barierei
	pthread_mutex_destroy(&arg.mutex);
	pthread_barrier_destroy(&arg.barrier);
	
    return 0;    
}
