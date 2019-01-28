/*Definition of hashtable functions*/

#ifndef LSH_HASHTABLE_H
#define LSH_HASHTABLE_H

#include "lsh_bucket.h"

class bucket;
class item;

class hashtable{
  bucket* buck;
public:
  hashtable();
  bucket* get_bucket();
  void set_bucket(bucket* p);
  void insert_point(item* p);
};

#endif
