/*Definition of Lloyd's assignment algorithm*/

#ifndef LLOYDS_ASSIGN_H
#define LLOYDS_ASSIGN_H

#include <iostream>
#include <cmath>
#include "../../clustering/cluster.h"

class dataset;

// returns index of min value
int find_min(double* list, int k);
dataset* lloyd(dataset* data, dataset** centers, int n, int k, std::string metric);

#endif
