/*Implementation of silhouette functions*/

#include <iostream>
#include "silhouette.h"

bool iskmeans;
bool complete;

using namespace std;

// Function to calculate silhuette of every item
double silhuette_item(dataset* item, vector<dataset*> cluster1, vector<dataset*> cluster2, string metric)
{
  double sum1 = 0.0;
  double sum2 = 0.0;
  int counter1 = 0;
  int counter2 = 0;

  for(int i = 0; i < cluster1.size(); i++){
    sum1 += insert_distance_in_map(item,cluster1[i],metric);
    counter1++;
  }
  for(int i = 0; i < cluster2.size(); i++){
    sum2 += insert_distance_in_map(item,cluster2[i],metric);
    counter2++;
  }

  if(sum1 == 0) // to prevent fp exception
    sum1 = 1;
  if(sum2 == 0) // to prevent fp exception
    sum2 = 1;
  double mean1 = sum1/counter1;
  double mean2 = sum2/counter2;
  if(mean1 > mean2)
    return ((mean2-mean1)/(mean1));
  else
    return ((mean2-mean1)/(mean2));
}

double silhuette_cluster(unordered_map<string,int> center_pairs, vector<dataset*>* clusters, string metric, int index)
{

  double sum = 0.0;
  int counter = 0;
  vector<dataset*> cluster1 = clusters[index];
  vector<dataset*> cluster2;

  for(int i = 0; i < cluster1.size(); i++){
    cluster2 = clusters[center_pairs.find(cluster1[i]->get_id())->second];
    double tmp = silhuette_item(cluster1[i],cluster1,cluster2,metric);
    if(!isnan(tmp))
      sum += tmp;
    counter++;
  }

  return (sum/counter);
}

void silhuette_whole(dataset* data, dataset** centers, int clusters, string metric, string outputFile)
{
  int i, j;
  unordered_map<string,int>::iterator it;

  // make array of vector for every cluster of datapoints
  vector<dataset*>* cluster_members = new vector<dataset*>[clusters];
  for(i = 0; i < n; i++){
    for(j = 0; j < clusters; j++){
      if(data[i].get_center() == centers[j]){
        cluster_members[j].push_back(&data[i]);
        break;
      }
    }
  }

  // calculate pairs of centers
  unordered_map<string,int> center_pairs = closer_centers(data,centers,clusters,metric);

  // open file stream to write output
  fstream fs;
  fs.open(outputFile, fstream::in | fstream::out | fstream::app);

  for(i = 0; i < clusters; i++){
    // print xij (kmeans)
    if(iskmeans){
      fs << "CLUSTER - " << i+1 << " {size " << cluster_members[i].size() << ", centroid: ";
      for(int k = 0; k < VECTORSIZE; k++)
        fs << centers[i]->get_xij()[k] << " ";
      fs << "}" << endl;
    }
      // print id (PAM)
    else
      fs << "CLUSTER - " << i+1 << " {size " << cluster_members[i].size()
      << ", centroid: " << centers[i]->get_id() << "}" << endl;
  }

  // for every cluster calculate its silhuette
  double sum = 0.0;
  int counter = 0;
  fs << "Silhouette: [";
  for(i = 0; i < clusters; i++){
    double tmp = silhuette_cluster(center_pairs,cluster_members,metric,i);
    if(!isnan(tmp)){
      sum += tmp;
      fs << "s" << i+1 << ": " << tmp << ", ";
    }
    else
      fs << "s" << i+1 << ": " << 0 << ", ";
    counter++;
  }

  fs << "stotal: " << sum/counter << "]" << endl;

  // if asked for complete stats
  if(complete){
    for(i = 0; i < clusters; i++){
      fs <<  "CLUSTER - " << i+1 << " {";
      for(j = 0; j < cluster_members[i].size(); j++){
        fs << cluster_members[i][j]->get_id() << ", ";
      }
      fs << "}" << endl;
    }
  }

  fs.close();

  delete[] cluster_members;
}
