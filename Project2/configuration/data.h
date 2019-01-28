/*Definition of database manipulation header functions*/

#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <cstring>
#include "../clustering/cluster.h"
#include "configure.h"

class dataset {
  std::string id;
  type* xij;
  dataset* center;

public:
  std::string get_id();
  void set_id(std::string id);
  type* get_xij();
  void set_xij(type* xij);
  dataset* get_center();
  void set_center(dataset* center);
};

#endif
