/*Definition of hypercube functions*/

#ifndef CUBE_H
#define CUBE_H

#include "../../clustering/cluster.h"
#include "./lsh/lsh_item.h"
#include "./lsh/lsh_bucket.h"
#include "./lsh/lsh_hashtable.h"

class dataset;

hashtable** cube_main(dataset* data, int k, std::string family);

#endif
