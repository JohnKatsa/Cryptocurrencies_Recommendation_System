/*Definition of nearest neighbours functions*/

#ifndef NEAREST_NEIGHBOUR_H
#define NEAREST_NEIGHBOUR_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "../../../clustering/cluster.h"

class dataset;

extern bool* mapping;  // map to know in lsh/hypercube Assignment

extern std::unordered_set<std::string> closest_ids; 			// used for nearest neighbours of every id (set of id neighbours)
extern std::unordered_map<std::string,std::unordered_set<std::string>> closest;	// uses closest_ids for project 3

using namespace std;

dataset* find_neighbours(dataset* data, hashtable t, dataset* query, int d, string family, float range, int* g, int fi, int k, int flag, char* outputFile);
dataset* nearest_neighbours(dataset* data, dataset* query, int k, int d, int L, hashtable** table, string family, float range, int flag, char* outputFile);

#endif
