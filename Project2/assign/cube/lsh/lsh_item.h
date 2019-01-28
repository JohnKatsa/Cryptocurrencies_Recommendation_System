/* Definition of hashtable items used in lsh and hypercube*/

#ifndef LSH_ITEM_H
#define LSH_ITEM_H

#include "../../../clustering/cluster.h"

using namespace std;

// item of hashtable
class item{
  string id;
  int index;  // index in data array
  type* p;  // data point
  int* g;   // g(p)
  int fi;   // fi(p)

public:
  string get_id();
  void set_id(string id);
  type* get_p();
  int* get_g();
  int get_fi();
  int get_index();
  void set_index(int index);
  void set_p(type* p);
  void set_g(int* g);
  void set_fi(int f);
};

#endif
