/*Definition of functions used for PAM a la Lloyd's Update Algorithm*/

#ifndef PAM_LLOYD_UPDATE_H
#define PAM_LLOYD_UPDATE_H

#include <iostream>
#include <vector>
#include "../../clustering/cluster.h"

dataset** pam_lloyd_update(dataset* data, dataset** centers, int clusters, int n, string metric);
dataset* calculate_cluster_centroid(vector<dataset*> cluster, string metric);
double objective_function(dataset* datapoint, vector<dataset*> cluster, string metric);

#endif
