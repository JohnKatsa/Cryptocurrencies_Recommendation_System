/*Implementation of Lloyd's assignment algorithm*/

#include "lloyd_assign.h"

using namespace std;

// returns index of min value
int find_min(double* list, int k)
{
  int min = 0;
  double minval = list[0];
  for(int i = 1; i < k; i++)
    if(list[i] < minval){
      min = i;
      minval = list[i];
    }

  return min;
}

dataset* lloyd(dataset* data, dataset** centers, int n, int k, string metric)
{
  // stores all distances to calculate min
  double test[k] = {0};

  for(int i = 0; i < n; i++){
    // calculate distances from all centers
    for(int j = 0; j < k; j++){
      test[j] = insert_distance_in_map(&data[i],centers[j],metric);
    }
    data[i].set_center(centers[find_min(test,k)]);
  }

  return data;
}
