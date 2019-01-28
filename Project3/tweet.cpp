#include "tweet.h"

// getters and setters
void tweet::setTweetID(std::string tweetID){
  this->tweetID = tweetID;
}
void tweet::setContent(std::vector<std::string> content){
  this->content = content;
}
void tweet::setSentiment(double sentiment){
  this->sentiment = sentiment;
}
void tweet::setCryptos(std::unordered_set<std::string> cryptos){
  this->cryptos = cryptos;
}
std::string tweet::getTweetID(){
  return this->tweetID;
}
std::vector<std::string> tweet::getContent(){
  return this->content;
}
double tweet::getSentiment(){
  return this->sentiment;
}
std::unordered_set<std::string> tweet::getCryptos(){
  return this->cryptos;
}
