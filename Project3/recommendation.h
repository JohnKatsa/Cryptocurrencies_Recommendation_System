#ifndef RECOMMENDATION_H
#define RECOMMENDATION_H

#include "tweet.h"
#include "user.h"
#include "cluster.h"
#include "methods.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <unordered_set>
#include <set>
#include <map>
#include <math.h>
#include <algorithm>
#include <time.h>

struct valType {
    std::string uID;
    std::string cName;
    double sentiment;

    bool operator==(valType& v){
        return (v.uID == uID) && (v.cName == cName);
    }
};

typedef double type;
int cluster_main(char* inputFile, char* configurationFile, char* outputFile, char* metric, char* style);

#include "../Project2/clustering/cluster.h"
#include "../Project2/configuration/data.h"
#include "../Project2/configuration/configure.h"
#include "../Project2/init/Random_Selection_of_k_points/random_init.h"
#include "../Project2/init/kmeanspp/kmeanspp_init.h"
#include "../Project2/assign/cube/lsh/lsh.h"
#include "../Project2/assign/cube/cube.h"
#include "../Project2/assign/cube/nearest_neighbour/nearest_neighbour.h"
#include "../Project2/assign/lloyd_assign/lloyd_assign.h"
#include "../Project2/update/kmeans_update/kmeans_update.h"
#include "../Project2/update/PAM_a_la_Lloyd/pam_lloyd_update.h"
#include "../Project2/clustering/silhouette.h"

#endif