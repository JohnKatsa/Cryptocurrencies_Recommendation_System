/*Implementation of functions used in kmeans update*/

#include "kmeans_update.h"

dataset** kmeans_update(dataset* data, dataset** centers, int clusters, int n)
{
  int i, j, k;

  // keeps sum of every coordinate
  float** sums = new float*[clusters];
  for(i = 0; i < clusters; i++){
    sums[i] = new float[VECTORSIZE];
    for(j = 0; j < VECTORSIZE; j++)
        sums[i][j] = 0.0;
  }

  // keeps number of items per cluster
  int* enumerate = new int[clusters];
  for(i = 0; i < clusters; i++)
    enumerate[i] = 1;   // to avoid fp exception

  // update data for every cluster to calculate new centers
  for(i = 0; i < n; i++){
    type* tmp = data[i].get_xij();
    for(j = 0; j < clusters; j++){
      if(data[i].get_center() == centers[j]){
        for(k = 0; k < VECTORSIZE; k++)
          sums[j][k] += tmp[k];
        enumerate[j] += 1;
      }
    }
  }

  // compute new cluster centers
  dataset** newcenters = new dataset*[clusters];
  type* tmp;
  for(i = 0; i < clusters; i++){
    // allocate memory for new point (out of dataset)
    newcenters[i] = new dataset;
    newcenters[i]->set_id("other" + to_string(i));
    tmp = new type[VECTORSIZE];
    for(j = 0; j < VECTORSIZE; j++)
      tmp[j] = sums[i][j]/(enumerate[i]-1);   // take back 1 added at the beginning
    newcenters[i]->set_xij(tmp);
  }

  return newcenters;
}
