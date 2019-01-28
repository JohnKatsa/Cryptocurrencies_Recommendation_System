/*Definition of functions used in kmeans update Implementation*/

#ifndef KMEANS_UPDATE_H
#define KMEANS_UPDATE_H

#include <iostream>
#include "../../clustering/cluster.h"

dataset** kmeans_update(dataset* data, dataset** centers, int clusters, int n);

#endif
