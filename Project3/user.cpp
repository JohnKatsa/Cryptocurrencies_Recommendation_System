#include "user.h"

using namespace std;

// getters and setters
void user::setUserID(std::string userID){
  this->userID = userID;
}
void user::setTweet(tweet tweet){
  this->tweets.push_back(tweet);
}
void user::setTweets(std::vector<tweet> tweets){
  this->tweets = tweets;
}
void user::setCryptoHash(std::unordered_map<string,double> cryptoHash){
  this->cryptoHash = cryptoHash;
}
void user::addCryptoHash(pair<std::string,double> crypto){
  this->cryptoHash.insert(crypto);
}
std::string user::getUserID(){
  return this->userID;
}
std::vector<tweet> user::getTweets(){
  return this->tweets;
}
std::unordered_map<string,double> user::getCryptoHash(){
  return this->cryptoHash;
}
void user::rmCryptoHash(std::string crID){
  this->cryptoHash.erase(crID);
}