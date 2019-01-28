/*Header for initialization functions*/

#ifndef RANDOM_INIT_H
#define RANDOM_INIT_H

#include <iostream>
#include <cstring>
#include <time.h>
#include <set>
#include <iterator>
#include <random>
#include "../../clustering/cluster.h"

class dataset;

dataset** random_selection_of_k_points(dataset* data, int k, int n);

#endif
