// Bobocu Alexandra-Florentina, 331CA
#include "helper.hpp"

// Functie pentru citirea continutului fisierelor care trebuie
// prelucrate, existente in fisierul "test.txt".
// Returnez coada populata cu caile catre fisierele de procesat.
queue<string> read_file(string file) {
	// Deschid corespunzator fisierul cu numele "test.txt"
	ifstream test_file;
	test_file.open(file.c_str());

	// Verificare deschidere corecta
	if (!test_file) {
        cerr << "Couldn't open test.txt file" << '\n';
        exit(1);
    }

	// Calea catre fisierele de forma "inX.txt"
	string file_path;

	// Retin fisierele de acea forma intr-o coada
	queue<string> all_file_paths;

	int num_files = 0;

	// Citire linie cu linie din fisierul "test.txt"
    if (test_file.is_open()) {
		// Citirea numarului de fisiere
		test_file >> num_files;

		// Citesc atat timp cat imi permite numarul de fisiere
        while (num_files) {
			// Citirea fiecarei cai in parte
			test_file >> file_path;

			// Popularea cozii de fisiere
			all_file_paths.push(file_path);

            num_files--;
        }
    }

	// Inchid corespunzator fisierul cu numele "test.txt"
	test_file.close();
	
	return all_file_paths;
}
