#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "tweet.h"
#include "user.h"

class cluster {
  std::string clusterID;
  std::vector<tweet> tweets;                          // cluster members --> tweets (LSH with clusters)
  std::vector<user> users;                            // cluster members --> users  (Clustering)
  std::unordered_map<std::string,double> cryptoHash;  // cryptos in cluster and their sentiments

  public:
    void setClusterID(std::string clusterID);
    void setTweet(tweet tweet);
    void setTweets(std::vector<tweet> tweets);
    void setCryptoHash(std::unordered_map<std::string,double> cryptoHash);
    void setUser(user user);
    void setUsers(std::vector<user> users);
    std::string getClusterID();
    std::vector<tweet> getTweets();
    std::unordered_map<std::string,double> getCryptoHash();
    std::vector<user> getUsers();
};

#endif