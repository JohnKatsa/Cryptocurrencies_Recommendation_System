/*Main function*/

#include "cluster.h"

using namespace std;

// mapping used to see if center is assigned to a point (range-search assignment)
bool* mapping;

// make distances map db (if you can)
unordered_map<id_pairs,double,hash_fn> dist_saved;
bool useMap;

//unordered_map<string,unordered_set<string>> closest;	// uses closest_ids for project 3

int cluster_main(char* inputFile, char* configurationFile, char* outputFile, char* metric, char* style)
{
  // get command line arguments
  //char *inputFile, *configurationFile, *outputFile, *metric, *style;
  //read_arguments(args,argv,&inputFile,&configurationFile,&outputFile,&metric,&style,&complete);
  complete = 1;

  // read input and make database
  dataset* data = read_inputFile(inputFile,&n);

  // configure system
  int clusters, hash_functions, hash_tables;
  configurate(configurationFile,&clusters,&hash_functions,&hash_tables);

  // write initial output statistics
  fstream fs;
  fs.open(outputFile, fstream::in | fstream::out | fstream::app);
  fs << "Algorithm: " << style[0] << "x" << style[1] << "x" << style[2] << endl;
  fs << "Metric: " << metric << endl;
  fs.close();

  const clock_t begin_time = clock();

  // Can you use map?
  if(style[2] == '2'){
    useMap = 1;
    iskmeans = 0; // For safety :P
  }
  else{
    useMap = 0;   // For safety :P
    iskmeans = 1;
  }

  // INITIALIZE
  dataset** centers;
  if(style[0] == '1')
    centers = random_selection_of_k_points(data,clusters,n);
  else if(style[0] == '2')
    centers = k_means_plus_plus(data,clusters,n,metric);
  else{
    cout << "No initialization provided!" << endl;
    sleep(1);
    return -1;
  }

  for(int i = 0; i < clusters; i++)
    centers[i]->set_center(centers[i]);

  //for(int i = 0; i < clusters; i++)
    //cout << centers[i]->get_id() << " ";
  //cout << endl;
  //////////////////////////////////////////////////////////////////

  // ASSIGN (FOR LSH/CUBE, MORE INITIALIZATION NEEDED)
  int k = hash_tables, L = hash_functions, R = 100;
  hashtable** table;
  dataset** newcenters;

  if(style[1] == '1'){
    table = lsh_main(data,hash_tables,hash_functions,metric);
    mapping = new bool[n];  // allocate memory for assignment mapping
  }
  else if(style[1] == '2'){
    table = cube_main(data,hash_tables,metric);
    mapping = new bool[n];  // allocate memory for assignment mapping
    L = 1;
  }

  int iterations = 5;  // max iterations number
  for(int j = 0; j < iterations; j++){
    //cout << "EPANALHPSH " << j <<endl;

    if(style[1] == '1' || style[1] == '2'){
      // initialize to 0
      for(int i = 0; i < n; i++)
        mapping[i] = 0;
      for(int i = 0; i < clusters; i++){
        data = nearest_neighbours(data,centers[i],k,VECTORSIZE,L,table,metric,R,0,outputFile);   // LSH-neighnours
      }
      // assign the unassigned
      for(int i = 0; i < n; i++)
        if(mapping[i] == 0)// unassigned
          data[i].set_center(find_closer(centers,data[i],clusters,metric));
    }
    else if(style[1] == '3')
      data = lloyd(data,centers,n,clusters,metric);
    else{
      cout << "No assignment provided!" << endl;
      sleep(1);
      return -1;
    }
    //////////////////////////////////////////////////////////////////

    // if last iteration, no need to calculate new centers
    if(j == iterations-1)
      break;

    // UPDATE
    if(style[2] == '1')
      newcenters = kmeans_update(data,centers,clusters,n);
    else if(style[2] == '2')
      newcenters = pam_lloyd_update(data,centers,clusters,n,metric);
    else{
      cout << "No update provided!" << endl;
      sleep(1);
      return -1;
    }

    // breaking case
    bool flag = 0;
    for(int kl = 0; kl < clusters; kl++)
      if(centers[kl] != newcenters[kl]){
        flag = 1;
        break;
      }
    if(!flag)
      break;
    ////////////////

	if(j != iterations-1){
    	delete[] centers;
    	centers = newcenters;
	}
  }

  silhuette_whole(data,centers,clusters,metric,outputFile);

  const clock_t end_time = clock();
  //cout << ((end_time - begin_time)/CLOCKS_PER_SEC)/60 << " min(s) and "
   //    << ((end_time - begin_time)/CLOCKS_PER_SEC)%60 << " sec(s)" << endl;

  // update closest (used for project 3)
  /*unordered_map<string,unordered_set<string>>::iterator it;
  string cntr;
  string currid;
  for(int i = 0; i < n; i++){
	cntr = data[i].get_center()->get_id();
	currid = data[i].get_id();

	it = closest.find(cntr);
	if(it != closest.end())	// find if already in map this cluster so to ADD and not MAKE
	  it->second.insert(currid);
	else{	// MAKE
	  unordered_set<string> temp;
	  temp.insert(currid);
	  closest.insert({cntr,temp});
	}
  }*/
  ///////////////////////////////////////////

  // clean up
  clean_data(data,n);                     // delete dataset
  if(style[1] == '1' || style[1] == '2')
    delete_hashtables(table,L);           // delete hashtables if lsh or cube used

  return 0;
}

// find closer center
dataset* find_closer(dataset** centers, dataset data, int clusters, string metric)
{
  double f, min = insert_distance_in_map(&data,centers[0],metric);
  dataset* minpoint = centers[0];

  for(int i = 0; i < clusters; i++){
    f = insert_distance_in_map(&data,centers[i],metric);
    if(f < min){
      min = f;
      minpoint = centers[i];
    }
  }

  return minpoint;
}

// call this function when you want to add a new known distance pair.
double insert_distance_in_map(dataset* d1, dataset* d2, string metric)
{
  if(d1 == NULL || d2 == NULL)
    return 10000.0;
  struct id_pairs ip;
  unordered_map<id_pairs,double,hash_fn>::iterator it;
  double dist;
  ip.id1 = d1->get_id();
  ip.id2 = d2->get_id();

  if(!useMap)
    return distance(d1->get_xij(),d2->get_xij(),metric);

  it = dist_saved.find(ip);
  // not found
  if(it == dist_saved.end()){
    dist = distance(d1->get_xij(),d2->get_xij(),metric);
    dist_saved[ip] = dist;

    // insert it also with the opposite order
    string temp = ip.id1;
    ip.id1 = ip.id2;
    ip.id2 = temp;
    dist_saved[ip] = dist;
  }
  // found
  else{
    dist = it->second;
  }

  return dist;
}

// Function to return a map of close pairs of centers
unordered_map<string,int> closer_centers(dataset* data, dataset** centers, int clusters, string metric)
{
  double f;
  double min;
  int index_min;
  unordered_map<string,int> center_pairs;

  for(int i = 0; i < n; i++){
    min = 100000.0;
    for(int j = 0; j < clusters; j++){
      if(data[i].get_center() != centers[j]){ // dont return current closest
        f = insert_distance_in_map(&data[i],centers[j],metric);
        if(f < min){
          min = f;
          index_min = j;
        }
      }
    }
    // insert new pair of close centers
    center_pairs[data[i].get_id()] = index_min;
  }

  return center_pairs;
}
