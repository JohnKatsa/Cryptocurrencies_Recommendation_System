/*Header used for Main*/

#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <list>
#include <limits>
#include <unordered_map>
#include <iterator>
#include <cmath>

typedef std::numeric_limits< double > dbl;
typedef double type;

struct id_pairs{
  std::string id1;
  std::string id2;
  bool const operator<(const id_pairs &o) const{
    return id1 < o.id1 || (id1 == o.id1 && id2 < o.id2);
  }
  bool const operator==(const id_pairs &o) const{
    return id1 == o.id1 && id2 == o.id2;
  }
};

struct hash_fn{

  std::size_t operator() (const id_pairs &o) const{
    std::size_t h1 = std::hash<std::string>{}(o.id1);
    std::size_t h2 = std::hash<std::string>{}(o.id2);
    return h1 ^ (h2 << 1); // or use boost::hash_combine (see Discussion)
	}
};

// make distances map db (if you can)
extern std::unordered_map<id_pairs,double,hash_fn> dist_saved;
extern bool useMap;

#include "../configuration/data.h"
#include "../configuration/configure.h"
#include "../init/Random_Selection_of_k_points/random_init.h"
#include "../init/kmeanspp/kmeanspp_init.h"
#include "../assign/cube/lsh/lsh.h"
#include "../assign/cube/cube.h"
#include "../assign/lloyd_assign/lloyd_assign.h"
#include "../assign/cube/nearest_neighbour/nearest_neighbour.h"
#include "../update/kmeans_update/kmeans_update.h"
#include "../update/PAM_a_la_Lloyd/pam_lloyd_update.h"
#include "silhouette.h"

dataset* find_closer(dataset** centers, dataset data, int clusters, std::string metric);
double insert_distance_in_map(dataset* d1, dataset* d2, std::string metric);
std::unordered_map<string,int> closer_centers(dataset* data, dataset** centers, int clusters, std::string metric);
int cluster_main(char* inputFile, char* configurationFile, char* outputFile, char* metric, char* style);

#endif
