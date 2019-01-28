/*Define k-means++ Inititalization functions*/

#include "kmeanspp_init.h"

using namespace std;

double distance(type* point1, type* point2, string metric){

  if(!metric.compare("cosine")){
    int i;
    type xy = 0, normx = 0, normy = 0;
    for(i = 0; i < VECTORSIZE; i++){
      xy += point1[i]*point2[i];
      normx += point1[i]*point1[i];
      normy += point2[i]*point2[i];
    }
	if(normx != 0 && normy != 0)
    	return xy/(sqrt(normx)*sqrt(normy));
	else
		return 0.0;
  }
  else{
    int i;
    double distance2 = 0.0;
    for(i = 0; i < VECTORSIZE; i++){
      distance2 += (point1[i]-point2[i])*(point1[i]-point2[i]);
    }
     return (double)sqrt(distance2);
  }

}

dataset** k_means_plus_plus(dataset* data, int k, int n, string metric)
{
  time_t t;
  srand((unsigned) time(&t));
  dataset** centers = new dataset*[k];

  int st = rand()%n;

  // initialize sets
  set < int, greater<int> > data_set;
  set < int, greater<int> > centers_set;
  set < int, greater <int> > :: iterator itr_data;
  set < int, greater <int> > :: iterator itr_centers;
  for(int i = 0; i < n; i++){
    if(i == st)
      centers_set.insert(st);
    else
      data_set.insert(i);
  }

  // used to store data points probabilities and distances
  double probs[n];
  double distances[n];

  int i = 1;
  int min;
  double mindistance;
  double dist;
  while(i < k){

    // For every data-centers
    for(itr_data = data_set.begin(); itr_data != data_set.end(); ++itr_data){
      mindistance = 1000000;
      //For every center
      for(itr_centers = centers_set.begin(); itr_centers != centers_set.end(); itr_centers++){
        dist = insert_distance_in_map(&data[*itr_centers],&data[*itr_data],metric);
        if(dist < mindistance){
          mindistance = dist;
          min = *itr_centers;
        }
      }
      // set min distance of this data point
      distances[*itr_data] = mindistance;
    }

    // set all probabilities
    double sum = 0;
    for(itr_data = data_set.begin(); itr_data != data_set.end(); ++itr_data)
      sum += pow(distances[*itr_data],2);
    for(itr_data = data_set.begin(); itr_data != data_set.end(); ++itr_data)
      probs[*itr_data] = pow(distances[*itr_data],2)/sum;

    // calculate weighted-randomly next center
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    double p = dis(gen);

    double b = 0; // limits to result possibility
    for(itr_data = data_set.begin(); itr_data != data_set.end(); ++itr_data){
      b += probs[*itr_data];
      // found new center
      if(p <= b){
        centers_set.insert(*itr_data);
        data_set.erase(*itr_data);
        break;
      }
    }
    i++;
  }

  int l = 0;
  for(itr_centers = centers_set.begin(); itr_centers != centers_set.end(); ++itr_centers){
    centers[l] = &data[*itr_centers];
    l++;
  }
  return centers;
}
