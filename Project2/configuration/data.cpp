/*Data type used to store database functions*/

#include "data.h"

using namespace std;

string dataset::get_id(){
  return this->id;
}
void dataset::set_id(string id){
  this->id = id;
}
type* dataset::get_xij(){
  return this->xij;
}
void dataset::set_xij(type* xij){
  this->xij = xij;
}
dataset* dataset::get_center(){
  return this->center;
}
void dataset::set_center(dataset* center){
  this->center = center;
}
