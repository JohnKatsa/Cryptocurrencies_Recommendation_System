#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "tweet.h"
#include <unordered_set>

class user {
  std::string userID;
  std::vector<tweet> tweets;
  std::unordered_map<std::string,double> cryptoHash;  // crypto:sentiment

  public:
    void setUserID(std::string userID);
    void setTweet(tweet tweet);
    void setTweets(std::vector<tweet> tweets);
    void setCryptoHash(std::unordered_map<std::string,double> cryptoHash);
    void addCryptoHash(std::pair<std::string,double> crypto);
    std::string getUserID();
    std::vector<tweet> getTweets();
    std::unordered_map<std::string,double> getCryptoHash();

    void rmCryptoHash(std::string crID);
};

#endif