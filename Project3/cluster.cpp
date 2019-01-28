#include "cluster.h"

void cluster::setClusterID(std::string clusterID){
  this->clusterID = clusterID;
}
void cluster::setTweet(tweet tweet){
  this->tweets.push_back(tweet);
}
void cluster::setTweets(std::vector<tweet> tweets){
  this->tweets = tweets;
}
void cluster::setCryptoHash(std::unordered_map<std::string,double> cryptoHash){
  this->cryptoHash = cryptoHash;
}
void cluster::setUser(user user){
  this->users.push_back(user);
}
void cluster::setUsers(std::vector<user> users){
  this->users = users;
}
std::string cluster::getClusterID(){
  return this->clusterID;
}
std::vector<tweet> cluster::getTweets(){
  return this->tweets;
}
std::unordered_map<std::string,double> cluster::getCryptoHash(){
  return this->cryptoHash;
}
std::vector<user> cluster::getUsers(){
  return this->users;
}