#ifndef METHODS_H
#define METHODS_H

#include "tweet.h"
#include "user.h"
#include "cluster.h"
#include "recommendation.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <unordered_set>
#include <set>
#include <map>
#include <math.h>
#include <algorithm>
#include <time.h>

using namespace std;

struct valType;

extern unordered_map<string,string> tweetToUser;     // use to go from tweet to user
extern unordered_map<string,string> clusterToUser;   // use to go from cluster to user

extern int validation;
extern double MAE;
extern int sum;
//extern vector<valType> train;  // used for validation
extern vector<valType> test;   // used for validation


unordered_map<string,user> makeUserHash(string tweetsFile);
unordered_map<string,double> makeLexiconHash(string lexiconFile);
unordered_map<string,string> makeCryptoHash(string cryptoFile);
unordered_map<string,tweet> giveSentimentToTweet(unordered_map<string,user>& userMap,unordered_map<string,double> lexiconMap, unordered_map<string,string> cryptoMap);
unordered_map<string,cluster> makeClusterHash(string clusterFile, unordered_map<string,tweet> tweetMap);
unordered_map<string,cluster> makeClusterHash(string clusterFile, unordered_map<string,user> userMap);
vector<double> makeUVector(user usr, set<string> row);
vector<double> makeCVector(cluster usr, set<string> row);
set<string> makeRow(unordered_map<string,string> cryptoMap);
set<string> makeNotCryptos(set<string> row, user usr);
set<string> makeNotCryptos(set<string> row, cluster clstr);
double cosine_similarity(vector<double> v1, vector<double> v2);
vector<pair<string,vector<pair<string,double>>>> topUser(set<string> row, unordered_map<string,user> userMap, unordered_map<string,vector<double>> uVector, int top);
unordered_map<string,vector<pair<string,double>>> topCluster(set<string> row, unordered_map<string,cluster> clusterMap, unordered_map<string,vector<double>> cVector, int top);
unordered_map<string,vector<pair<string,double>>> topClusterUDEMO(set<string> row, unordered_map<string,cluster> clusterMap, unordered_map<string,user> userMap, unordered_map<string,vector<double>> cVector, unordered_map<string,vector<double>> uVector, int top);
unordered_map<string,vector<pair<string,double>>> topClusterC(set<string> row, unordered_map<string,cluster> clusterMap, unordered_map<string,user> userMap, unordered_map<string,vector<double>> cVector, unordered_map<string,vector<double>> uVector, int top, user usr);
void export_vector(string filename, unordered_map<string,vector<double>> uVector);
void myread(int argc, char** argv, string* inputFile, string* outputFile, bool* comp);


#endif