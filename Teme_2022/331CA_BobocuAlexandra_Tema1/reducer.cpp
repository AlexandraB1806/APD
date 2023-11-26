// Bobocu Alexandra-Florentina, 331CA
#include "helper.hpp"
#include <algorithm>
#include <set>

// Functia pentru threadul Reducer
void *reducer_func(void *arg) {
    thread_args *thread_arg = (thread_args *) arg;

    // Mapa partiala, rezultata in urma operatiei de tip Map
	// ce va urma sa fie extrasa din lista de mape
    map<int, vector<int>> selected_partial_map;

    // Mapa rezultat, obtinuta in urma operatiei de Reduce
	map<int, vector<int>> reducers_result;

    // Incep operatiile de Reduce cand toti Mapperii au terminat sarcinile
    pthread_barrier_wait(&thread_arg->barrier);

    for (unsigned long int i = 0; i < thread_arg->partial_maps_list.size(); i++) {
        // Selectarea mapei partiale
        selected_partial_map = thread_arg->partial_maps_list[i];

        // Creez separat o lista a cheilor
        vector<int> keys;

        // Parcurgere mapa partiala, obtinuta in urma operatiei de Map
        for (auto itr = selected_partial_map.begin();
            itr != selected_partial_map.end(); ++itr) {
            // Popularea vectorului de chei
            keys.push_back(itr->first);
            // Eliminarea cheilor duplicate
            set<int> set_keys(keys.begin(), keys.end());
            keys.assign(set_keys.begin(), set_keys.end());
        }

        // Pentru fiecare cheie in parte
        for (auto key : keys) {
            // Pentru valoarea de la fiecare cheie
            for (auto value : selected_partial_map[key]) {
                // Populez mapa rezultat
                reducers_result[key].push_back(value);
            }

            // Eliminarea valorilor duplicate
            set<int> set_values(reducers_result[key].begin(), reducers_result[key].end());
            reducers_result[key].assign(set_values.begin(), set_values.end());
        }
    }

    // Afisarea numarului de valori unice in fisierul de iesire
    for (auto itr = reducers_result.begin(); itr != reducers_result.end(); ++itr) {
        string out_file = "out" + to_string(itr->first) + ".txt";
        ofstream output_file(out_file);
        output_file << itr->second.size();
    }

    pthread_exit(NULL);
}
