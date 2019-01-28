#ifndef TWEET_H
#define TWEET_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

class tweet {
  std::string tweetID;
  std::vector<std::string> content;
  double sentiment;
  std::unordered_set<std::string> cryptos;

  public:
    void setTweetID(std::string tweetID);
    void setContent(std::vector<std::string> content);
    void setSentiment(double sentiment);
    void setCryptos(std::unordered_set<std::string> cryptos);
    std::string getTweetID();
    std::vector<std::string> getContent();
    double getSentiment();
    std::unordered_set<std::string> getCryptos();
};

#endif