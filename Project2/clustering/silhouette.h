/*Definition of silhouette functions*/

#ifndef SILHOUETTE_H
#define SILHOUETTE_H

#include "cluster.h"
#include <fstream>

extern bool iskmeans;
extern bool complete;

double silhuette_item(dataset* item, vector<dataset*> cluster1, vector<dataset*> cluster2, std::string metric);
double silhuette_cluster(std::unordered_map<string,int> center_pairs, vector<dataset*>* clusters, std::string metric, int index);
void silhuette_whole(dataset* data, dataset** centers, int clusters, std::string metric, std::string outputFile);

#endif
