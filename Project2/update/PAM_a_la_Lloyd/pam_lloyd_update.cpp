/*Implementation of functions used for PAM a la Lloyd's Update Algorithm*/

#include "pam_lloyd_update.h"

using namespace std;

dataset** pam_lloyd_update(dataset* data, dataset** centers, int clusters, int n, string metric)
{
  int i, j;

  // make array of vector for every cluster of datapoints
  vector<dataset*>* cluster_members = new vector<dataset*>[clusters];
  for(i = 0; i < n; i++)
    for(j = 0; j < clusters; j++)
      if(data[i].get_center() == centers[j]){
        cluster_members[j].push_back(&data[i]);
        //break;
      }

  // make medoid pointers array
  dataset** medoids = new dataset*[clusters];
  for(i = 0; i < clusters; i++){ // loop to calculate medoids
    dataset* tmp;
    if((tmp = calculate_cluster_centroid(cluster_members[i],metric)) == NULL) // no new center
      medoids[i] = centers[i];
    else
      medoids[i] = tmp;
  }

  delete[] cluster_members;

  return medoids;

}

// returns new medoid of the given cluster
dataset* calculate_cluster_centroid(vector<dataset*> cluster, string metric)
{
  double f, min = 100000000.0;
  dataset* minpoint;

  if(cluster.size() == 0)
    return NULL;

  for(int i = 0; i < cluster.size(); i++){
    f = objective_function(cluster[i],cluster,metric);
    if(f < min){
      min = f;
      minpoint = cluster[i];
    }
  }

  return minpoint;
}

double objective_function(dataset* datapoint, vector<dataset*> cluster, string metric)
{
  double sum = 0.0;
  for(int i = 0; i < cluster.size(); i++){
    sum += insert_distance_in_map(datapoint,cluster[i],metric);
  }
  return sum;
}
