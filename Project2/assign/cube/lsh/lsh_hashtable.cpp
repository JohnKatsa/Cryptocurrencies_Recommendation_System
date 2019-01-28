/*Implementation of hashtable functions*/

#include "lsh_hashtable.h"

using namespace std;

// CLASS: HASHTABLE, FUNCTIONS

hashtable::hashtable(){
}

bucket* hashtable::get_bucket(){
  return this->buck;
}

void hashtable::set_bucket(bucket* buck){
  this->buck = buck;
}

void hashtable::insert_point(item* p){
  bucket* tmp = this->get_bucket();

  while(tmp->get_bucket() != NULL)
    tmp = tmp->get_bucket();

  tmp->set_next(allocate_bucket(p));
}
