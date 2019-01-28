/*Implementation of function for item manipulation*/

#include "lsh_item.h"

using namespace std;

string item::get_id(){
  return this->id;
}

void item::set_id(string id){
  this->id = id;
}

type* item::get_p(){
  return this->p;
}

int* item::get_g(){
  return this->g;
}

int item::get_fi(){
  return this->fi;
}

int item::get_index(){
  return this->index;
}

void item::set_index(int index){
  this->index = index;
}

void item::set_p(type* p){
  this->p = p;
}

void item::set_g(int* g){
  this->g = g;
}

void item::set_fi(int fi){
  this->fi = fi;
}
