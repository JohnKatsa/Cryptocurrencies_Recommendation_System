/*Definitions of functions used for lsh*/

#ifndef LSH_H
#define LSH_H

#include <iostream>
#include <bitset>
#include "../../../clustering/cluster.h"
#include "lsh_item.h"
#include "lsh_bucket.h"
#include "lsh_hashtable.h"

extern int* r;
extern float* t;
extern float** v;
extern int tablesize;
extern int n;

#define W 400

class hashtable;

using namespace std;

// helpful for euclidean function
typedef struct euclidean_return{
  int* g;
  int fi;
} euclidean_return;

// general function
hashtable** lsh_main(dataset* data, int k, int L, string family);
// initialize hashtable buckets to empty
hashtable* initialize_buckets(int tablesize, hashtable* table);
// convert number to bitset
bitset<1>* convert_uniformly(int* h, int k);
// function to make k normal normal_distribution numbers
euclidean_return euclidean(int k, int d, type* p, int flag, int thesis);
// make bit string out of number
char* make_bit_string(bitset<1> * h, int k);
// cosine similarity function
int cosine_similarity(int k, int d, type* p);
// fill hashtable with the dataset
hashtable** fill_hashtables(int d, int k, int L, dataset* data, string family, int flag);
// compare g functions (used in euclidean)
int compare_g(int* g, int* g_bucket, int k);
// delete hashtables
void delete_hashtables(hashtable** table, int L);

#endif
