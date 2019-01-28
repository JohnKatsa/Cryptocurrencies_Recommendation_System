/*Functions used for bucket manipulation*/

#include "lsh_bucket.h"

using namespace std;

// CLASS: BUCKET, FUNCTIONS

void bucket::set_point(item* point){
  this->point = point;
}

void bucket::set_next(bucket* next){
  this->next = next;
}

item* bucket::get_point(){
  return this->point;
}

bucket* bucket::get_bucket(){
  return this->next;
}

/////
bucket* allocate_bucket(item* p){
  bucket* bu = new bucket;

  bu->set_point(p);
  bu->set_next(NULL);

  return bu;
}
/////
