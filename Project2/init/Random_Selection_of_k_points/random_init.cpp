/*Define Inititalization functions*/

#include "random_init.h"

using namespace std;

dataset** random_selection_of_k_points(dataset* data, int k, int n)
{
  time_t t;
  srand((unsigned) time(&t));

  // find k random points
  dataset** centers = new dataset*[k];
  string* check = new string[k];

  if(n < k){
    cout << "Cannot make enough cluster centers with this dataset (n < k)" << endl;
    return NULL;
  }

  int i = 0;
  int flag = 0;
  while(i < k){
    centers[i] = &data[rand()%n];
    // check that this number is not selected more than once
    for(int j = 0; j < i; j++){
      if(centers[i]->get_id() == check[j]){
        flag = 1;
        break;
      }
    }
    // only if flag is 0 go to next, else re calculate
    if(!flag){
      check[i] = centers[i]->get_id();
      i++;
    }
    flag = 0;
  }
  delete[] check;

  return centers;
}
