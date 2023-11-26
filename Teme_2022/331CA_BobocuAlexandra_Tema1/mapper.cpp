// Bobocu Alexandra-Florentina, 331CA
#include "helper.hpp"
#include "math.h"

// Functie pentru ridicarea la putere a unui numar
long power(int n, int exponent) {
	long aux = n;
	for (int i = 1; i < exponent; i++) {
		aux = aux * n;
	}
	return aux;
}

// Functie pentru gasirea bazei unui numar, ce are un anumit exponent,
// folosind algoritmul de cautare binara
int binary_search(int n, int left, int right, int exponent) {
	if (right >= left) {
		int mid = left + (right - left) / 2;

        long pow = power(mid, exponent);

		if (pow == n) {
			return n;
		} else if(pow > n) {
			return binary_search(n, left, mid - 1, exponent);
		} else if (pow < n) {
			return binary_search(n, mid + 1, right, exponent);
		}
	}
	return -1;
}

// Functia pentru threadul Mapper
void *mapper_func(void *arg) {
    thread_args *thread_arg = (thread_args *) arg;

	// Fisierul pe care un Mapper il alege din coada de fisiere
    string selected_file;

    int num_of_numbers = 0;
    int each_number;

	// Mapa partiala, rezultata in urma operatiei de tip Map
	// pe fiecare fisier de tip "inX.txt"
	map<int, vector<int>> mappers_nums;

	// Parcurg coada de fisiere
    while (!thread_arg->files_queue.empty()) {
		// Permit unui singur thread la un moment dat sa isi aleaga
		// fisierul pe care sa il proceseze
        pthread_mutex_lock(&thread_arg->mutex);
        selected_file = thread_arg->files_queue.front();
        thread_arg->files_queue.pop();
        pthread_mutex_unlock(&thread_arg->mutex);

		// Deschid corespunzator fisierul
        ifstream curr_file;
        curr_file.open(selected_file.c_str());

        // Verificare deschidere corecta
		if (!curr_file) {
			cerr << "Couldn't open file" << '\n';
			exit(1);
    	}

        if (curr_file.is_open()) {
			// Citirea numarului de numere
            curr_file >> num_of_numbers;

            // Citesc atat timp cat imi permite numarul de numere
			while (num_of_numbers) {
                // Citirea fiecarui numar
                curr_file >> each_number;

				// Populare mapa rezultata in urma operatiei de Map
				for (int e = 2; e <= thread_arg->num_reducers + 1; e++) {
					// Pentru fiecare numar in parte, rulez cautare binara pentru
					// a vedea daca s-a gasit baza pentru exponentul dat
					int res = binary_search(each_number, 1, sqrt(each_number), e);
					// Daca s-a gasit
					if (res != -1) {
						// Pentru exponentul respectiv, o sa am la dispozitie lista
						// numerelor care respecta conditia
						mappers_nums[e].push_back(res);
					}
				}

				num_of_numbers--;
			}
			// Inchid corespunzator fisierul
            curr_file.close();
        }
    }

	// Populez lista de map-uri partiale.
	// Permit adaugarea unui element doar o data
	pthread_mutex_lock(&thread_arg->mutex);
    thread_arg->partial_maps_list.push_back(mappers_nums);
	pthread_mutex_unlock(&thread_arg->mutex);

	// Astept ca toti Mapperii sa isi termine sarcina
    pthread_barrier_wait(&thread_arg->barrier);
	
	pthread_exit(NULL);
}
