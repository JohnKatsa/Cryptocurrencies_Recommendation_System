/*Definition of bucket of hashtable functions*/

#ifndef LSH_BUCKET_H
#define LSH_BUCKET_H

#include "lsh_item.h"

class item;

// bucket of hashtable representation
class bucket{
  item* point;
  bucket* next;
public:
  void set_point(item* point);
  void set_next(bucket* next);
  item* get_point();
  bucket* get_bucket();
  bucket* allocate_bucket();
};

bucket* allocate_bucket(item* p);

#endif
