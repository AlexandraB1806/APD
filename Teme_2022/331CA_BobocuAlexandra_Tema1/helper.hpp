// Bobocu Alexandra-Florentina, 331CA
#ifndef _HELPER_H
#define _HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <pthread.h>

using namespace std;

// Structura specifica unui thread
struct thread_args {
    pthread_mutex_t mutex;
    pthread_barrier_t barrier;
    queue<string> files_queue;
    vector<map<int, vector<int>>> partial_maps_list;
    int num_mappers;
    int num_reducers;
};

queue<string> read_file(string file);
long power(int n, int exponent);
long binary_search(long n, int left, int right, int exponent);
void *mapper_func(void *arg);
void *reducer_func(void *arg);

#endif
