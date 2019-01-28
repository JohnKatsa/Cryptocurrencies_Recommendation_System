#include "methods.h"

using namespace std;

// returns map of <userID - user object>
unordered_map<string,user> makeUserHash(string tweetsFile){

  // returned map & its iterator
  unordered_map<string,user> userMap;
  unordered_map<string,user>::iterator itr;
      
  // open tweets file
  fstream fs;
  fs.open(tweetsFile, fstream::in | fstream::out | fstream::app); 
      
  string line;  // used to store current line
  string token; // used to store current token
      
  // Identify the tweet
  while(getline(fs, line, '\n')){
    // make line a stream
    stringstream ss(line);

    // Vector of string to save tokens 
    vector<string> tokens; 

    // get user and tweet id
    string userID;
    string tweetID;
    getline(ss, userID, '\t');
    getline(ss, tweetID, '\t');

    //update my storage : tweet ---> user
    tweetToUser.insert({tweetID,userID});

    // Identify tokens (content of tweet)
    while(getline(ss, token, '\t')){
      tokens.push_back(token);
    }
    tokens[tokens.size()-1] = tokens[tokens.size()-1].substr(0,tokens[tokens.size()-1].size()-1);

    // make tweet object
    tweet twt;
    twt.setTweetID(tweetID);
    twt.setContent(tokens);

    // find if already exists this user 
    itr = userMap.find(userID);
    if(itr == userMap.end()){
      // make user object only if needed
      user usr;
      usr.setUserID(userID);
      usr.setTweet(twt);  // adds if it has more than one tweet
    
      // first tweet
      userMap.insert({userID,usr});
    }
    else
      // already exists user
      itr->second.setTweet(twt);
  }

  return userMap;
}

// returns map of <word - sentiment score>
unordered_map<string,double> makeLexiconHash(string lexiconFile){

  // returned map & its iterator
  unordered_map<string,double> lexiconMap;
  unordered_map<string,double>::iterator itr;

  // open lexicon file
  fstream fs;
  fs.open(lexiconFile, fstream::in | fstream::out | fstream::app); 
      
  string line;  // used to store current line
  string token; // used to store current token
      
  // Identify the tweet
  while(getline(fs, line, '\n')){
    // make line a stream
    stringstream ss(line); 

    // get word and sentiment value
    string word;
    string tmp;
    double sentiment;
    getline(ss, word, '\t');
    getline(ss, tmp, '\t');
    sentiment = stod(tmp);

    lexiconMap.insert({word,sentiment});
  }

  return lexiconMap;
}

// returns map of <other crypto reference names - crypto identifier>
unordered_map<string,string> makeCryptoHash(string cryptoFile){

  // returned map & its iterator
  unordered_map<string,string> cryptoMap;
  unordered_map<string,string>::iterator itr;
  vector<string> v;

  // open lexicon file
  ifstream fs;
  fs.open(cryptoFile); 
      
  string line;  // used to store current line
  string token; // used to store current token

  // Get crypto references and name line
  while(getline(fs, line)){
    // make line a stream
    stringstream ss(line);

    // used for Id Name and Reference Name
    string cryptoIdentifierName;
    string cryptoReferenceName;

    // get every reference name of cryprocurrency
    while(getline(ss, cryptoReferenceName, '\t')){
      v.push_back(cryptoReferenceName);
    }

    // set identifier name
    if(v.size() >= 5)
      cryptoIdentifierName = v[4];
    else
      cryptoIdentifierName = v[0];

    v[v.size()-1] = v[v.size()-1].substr(0,v[v.size()-1].size()-1);
    for(auto i : v)
      cryptoMap.insert({i,cryptoIdentifierName});

    v.clear();
  }

  fs.close();

  return cryptoMap;
}

// calculate total sentiment of every tweet
unordered_map<string,tweet> giveSentimentToTweet(unordered_map<string,user>& userMap,unordered_map<string,double> lexiconMap, unordered_map<string,string> cryptoMap){
  // make map of tweetid--tweet (used for project 2)
  unordered_map<string,tweet> tweetMap;
  
  // for every user
  for(unordered_map<string,user>::iterator usr = userMap.begin(); usr != userMap.end(); usr++){
    // for every of this user's tweets
    vector<tweet> tweets = usr->second.getTweets();                      // get tweets
    unordered_map<string,double> cryptos = usr->second.getCryptoHash();  // get user's cryptos

    for(vector<tweet>::iterator twt = tweets.begin(); twt != tweets.end(); twt++){
      // for every word of this tweet
      vector<string> content = twt->getContent();
      /*here make a temporary set */
      unordered_set<string> temporary_set = twt->getCryptos();
      double sentiment = 0.0;
      for (vector<string>::iterator word = content.begin(); word != content.end(); word++){
        // check if word has sentiment. if so, add it
        unordered_map<string,double>::iterator itr = lexiconMap.find(*word);
        unordered_map<string,string>::iterator itr2 = cryptoMap.find(*word);

        /*here put in temporary set every crypto found*/
        if(itr2 != cryptoMap.end())
          temporary_set.insert(itr2->second);

        if(itr != lexiconMap.end())
          sentiment += itr->second;
      }

      // store current tweet sentiment
      sentiment = sentiment/sqrt(sentiment*sentiment+15);
      twt->setSentiment(sentiment);
      twt->setCryptos(temporary_set);

      // update tweet map
      tweetMap.insert({twt->getTweetID(),*twt});

      // refresh crypto map of this user
      unordered_map<string,double>::iterator itr_tmp;
      for(auto cr : temporary_set){
        itr_tmp = cryptos.find(cr);  // find if word already exists
        if(itr_tmp == cryptos.end())
          cryptos.insert({cr,sentiment});  // first time
        else
          cryptos.insert({cr,itr_tmp->second + sentiment}); // add up to previous value
      }
    }

    usr->second.setCryptoHash(cryptos);
    usr->second.setTweets(tweets);
  }

  return tweetMap;
}

// calculate cj plus extra data for every cluster (With semantics as of p2)
unordered_map<string,cluster> makeClusterHash(string clusterFile, unordered_map<string,tweet> tweetMap){
  // returned map & its iterator
  unordered_map<string,cluster> clusterMap;
  unordered_map<string,tweet>::iterator itr;

  // open lexicon file
  fstream fs;
  fs.open(clusterFile, fstream::in | fstream::out | fstream::app); 
      
  string line;  // used to store current line
  string token; // used to store current token

  // skip to complete part of cluster file
  while(getline(fs, line, '\n')){
    stringstream ss(line);
    getline(ss, token, ' ');
    if(token == "Silhouette:") // first word of every line, to check if we are there
      break;
  }

  while(getline(fs, line, '\n')){

    string clusterID;
    string tweetID;
    cluster tmp_cluster;

    // make line a stream
    stringstream ss(line);

    //get cluster id
    getline(ss, token, ' ');
    getline(ss, token, ' ');
    getline(ss, token, ' ');
    clusterID = token;

    // clear "{"
    getline(ss, token, '{');
    if(token == "}") // empty 
      continue;

    // used for cluster crypto hash
    unordered_map<string,double> cryptos;
    unordered_map<string,double>::iterator itr_tmp;

    // get every tweet's data
    tmp_cluster.setClusterID(clusterID);
    while(getline(ss, tweetID, ',')){
      if(tweetID == " }")     // id = " }"
        continue;
      if(tweetID[0] == ' ')   // starting with " "
        tweetID.erase(0,1);
      itr = tweetMap.find(tweetID);
      if(itr != tweetMap.end()){
        tmp_cluster.setTweet(itr->second);
      }

      // refresh crypto map of this cluster
      if(itr != tweetMap.end()){
        for(auto cr : itr->second.getCryptos()){
          itr_tmp = cryptos.find(cr);  // find if word already exists
          if(itr_tmp == cryptos.end())
            cryptos.insert({cr,itr->second.getSentiment()});  // first time
          else
            cryptos.insert({cr,itr_tmp->second + itr->second.getSentiment()}); // add up to previous value
        }
      }
      else
        continue;
    }

    // set cluster crypto map
    tmp_cluster.setCryptoHash(cryptos);

    // add to clustermap
    clusterMap.insert({clusterID,tmp_cluster});
  }

  return clusterMap;
}

// calculate cj plus extra data for every cluster of users (Clustering)
unordered_map<string,cluster> makeClusterHash(string clusterFile, unordered_map<string,user> userMap){
  // returned map & its iterator
  unordered_map<string,cluster> clusterMap;
  unordered_map<string,user>::iterator itr;

  // open lexicon file
  fstream fs;
  fs.open(clusterFile, fstream::in | fstream::out | fstream::app); 
      
  string line;  // used to store current line
  string token; // used to store current token

  // skip to complete part of cluster file
  while(getline(fs, line, '\n')){
    stringstream ss(line);
    getline(ss, token, ' ');
    if(token == "Silhouette:") // first word of every line, to check if we are there
      break;
  }

  while(getline(fs, line, '\n')){

    string clusterID;
    string userID;
    cluster tmp_cluster;

    // make line a stream
    stringstream ss(line);

    //get cluster id
    getline(ss, token, ' ');
    getline(ss, token, ' ');
    getline(ss, token, ' ');

    // used for cluster crypto hash
    unordered_map<string,double> cryptos;
    unordered_map<string,double>::iterator itr_tmp;


    int counter = 0;
    while(getline(ss, userID, ',')){
      if(userID == " }")     // id = " {"
        continue;
      if(userID == "}")     // id = "}"
        continue;
      if(userID == "{}")     // id = "{}"
        continue;
      if(userID == "")     // id = ""
        continue;
      if(userID[0] == ' ' || userID[0] == '{')   // starting with " "
        userID.erase(0,1);
      itr = userMap.find(userID);

      // set id
      if(counter == 0){
        clusterID = userID;
        tmp_cluster.setClusterID(userID);
        counter++;
      }

      if(itr != userMap.end()){
        tmp_cluster.setUser(itr->second);
        
        // used for main 
        clusterToUser.insert({userID,clusterID});
      }

      // refresh crypto map of this cluster
      if(itr != userMap.end()){
        for(auto cr : itr->second.getCryptoHash()){
          itr_tmp = cryptos.find(cr.first);  // find if word already exists
          if(itr_tmp == cryptos.end())
            cryptos.insert({cr.first,cr.second});  // first time
          else
            cryptos.insert({cr.first,itr_tmp->second + cr.second}); // add up to previous value
        }
      }

      // add to closest
      unordered_map<string,unordered_set<string>>::iterator it = closest.find(clusterID);
      if(it != closest.end())
        it->second.insert(userID);
      else{
        unordered_set<string> tmpset;
        tmpset.insert(userID);
        closest.insert({clusterID,tmpset});
      }
    }

    // set cluster crypto map
    tmp_cluster.setCryptoHash(cryptos);

    // add to clustermap
    clusterMap.insert({clusterID,tmp_cluster});
  }

  return clusterMap;
}

// takes user and the row of cryptos and gives normalized vector
vector<double> makeUVector(user usr, set<string> row){
  vector<double> v;
  unordered_map<string,double> cryptoMap = usr.getCryptoHash();
  unordered_map<string,double>::iterator itr;

  // calculate average
  double avg = 0.0;
  int items = 0;
  for(auto cr : cryptoMap){
    avg += cr.second;
    items++;
  }
  if(items != 0)
    avg = avg/items;

  for(auto crypto : row){
    itr = cryptoMap.find(crypto);
    if(itr != cryptoMap.end())  // assigned
      v.push_back(itr->second /*- avg*/);
    else
      v.push_back(0.0); // avg - avg
  }

  return v;
}

// takes user and the row of cryptos and gives normalized vector
vector<double> makeCVector(cluster usr, set<string> row){
  vector<double> v;
  unordered_map<string,double> cryptoMap = usr.getCryptoHash();
  unordered_map<string,double>::iterator itr;

  // calculate average
  double avg = 0.0;
  int items = 0;
  for(auto cr : cryptoMap){
    avg += cr.second;
    items++;
  }
  if(items != 0)
    avg = avg/items;

  for(auto crypto : row){
    itr = cryptoMap.find(crypto);
    if(itr != cryptoMap.end())  // assigned
      v.push_back(itr->second /*- avg*/);
    else
      v.push_back(0.0); // avg - avg
  }

  return v;
}

// make a row for the cryptos in every vector (alphabetical)
set<string> makeRow(unordered_map<string,string> cryptoMap){
  set<string> s;

  for(auto cr : cryptoMap){
    s.insert(cr.second);
  }

  return s;
}

// make a set of cryptos in Row row that are not in user usr
set<string> makeNotCryptos(set<string> row, user usr){
  set<string> notCryptos = row;
    
  for(auto cr : usr.getCryptoHash()){
      notCryptos.erase(cr.first);
  }

  return notCryptos;
}

// make a set of cryptos in Row row that are not in cluster clstr
set<string> makeNotCryptos(set<string> row, cluster clstr){
  set<string> notCryptos = row;
    
  for(auto cr : clstr.getCryptoHash()){
      notCryptos.erase(cr.first);
  }

  return notCryptos;
}

double cosine_similarity(vector<double> v1, vector<double> v2){

  if(v1.size() != v2.size())
    perror("Wrong sizes");
  
  double uv = 0.0, normu = 0.0, normv = 0.0;
  for(int i; i < v1.size(); i++){
    uv += v1[i]*v2[i];
    normu += v1[i]*v2[i];
    normv += v1[i]*v2[i];
  }

  return uv/(sqrt(normu)*sqrt(normv));
}

// make R(u,i) to get top 5/2 cryptocurrencies - via userMap (0,0)
vector<pair<string,vector<pair<string,double>>>> topUser(set<string> row, unordered_map<string,user> userMap, unordered_map<string,vector<double>> uVector, int top){
  
  vector<pair<string,vector<pair<string,double>>>> ret;

  // for every user's P closest 
  for(auto usr : closest){
    double z = 0.0;     // normalizing factor

    set<string> notCryptos = makeNotCryptos(row,userMap.find(usr.first)->second);

    unordered_map<string,double> r_u_i;
    for(auto n : notCryptos){
      r_u_i.insert({n,0.0});
    }
    unordered_map<string,double>::iterator it;

    // for every user in P closest
    for(auto v : usr.second){

      vector<double> v1 = uVector.find(v)->second;         // v vector
      vector<double> v2 = uVector.find(usr.first)->second; // u vector
      double cs = cosine_similarity(v1,v2);

      for(auto cr : userMap.find(v)->second.getCryptoHash()){

        if(notCryptos.find(cr.first) != notCryptos.end()){

          it = r_u_i.find(cr.first);
          if(it != r_u_i.end())
            r_u_i.insert({cr.first,(cr.second+it->second)*cs});
          else
            r_u_i.insert({cr.first,cr.second*cs});
        }
      }
    }

    // used if validation enabled
    if(validation){
      for(auto t : test){
        if(t.uID == usr.first){
          auto p = r_u_i.find(t.cName);
          if(p != r_u_i.end()){
            if(p->second != p->second)
              MAE += abs(t.sentiment-0.5);
            else
              MAE += abs(p->second-t.sentiment);
            sum++;
          }
          break;
        }
      }
    }

    // get top 5/2
    vector<pair<string,double>> vec;
    for(int i = 0; i < top; i++){
      double max = -2;
      string maxv;
      for(auto r : r_u_i){
        if(r.second > max){
          maxv = r.first;
          max = r.second;
        }
      }
      if(max != -2){
        if(max <= 0){
          auto bn = notCryptos.begin();
          advance(bn, rand()%notCryptos.size());
          maxv = bn->data();
          vec.push_back(make_pair(maxv,max));
          r_u_i.erase(maxv);
        }
        else{
          vec.push_back(make_pair(maxv,max));
          r_u_i.erase(maxv);
        }
      }
    }

    if(vec.size() != 0)
      ret.push_back(make_pair(usr.first,vec));
  }

  return ret;
}

// make R(u,i) to get top 5/2 cryptocurrencies - via clusterMap of tweets (0,1)
unordered_map<string,vector<pair<string,double>>> topCluster(set<string> row, unordered_map<string,cluster> clusterMap, unordered_map<string,vector<double>> cVector, int top){
  
  unordered_map<string,vector<pair<string,double>>> ret;

  // for every user's P closest 
  for(auto usr : closest){
    double z = 0.0;     // normalizing factor

    set<string> notCryptos = makeNotCryptos(row,clusterMap.find(usr.first)->second);

    unordered_map<string,double> r_u_i;
    for(auto n : notCryptos){
      r_u_i.insert({n,0.0});
    }
    unordered_map<string,double>::iterator it;

    // for every user in P closest
    for(auto v : usr.second){

      vector<double> v1 = cVector.find(v)->second;         // v vector
      vector<double> v2 = cVector.find(usr.first)->second; // u vector
      double cs = cosine_similarity(v1,v2);

      for(auto cr : clusterMap.find(v)->second.getCryptoHash()){

        if(notCryptos.find(cr.first) != notCryptos.end()){

          it = r_u_i.find(cr.first);
          if(it != r_u_i.end())
            r_u_i.insert({cr.first,(cr.second+it->second)*cs});
          else
            r_u_i.insert({cr.first,cr.second*cs});
        }
      }
    }

    // used if validation enabled
    if(validation){
      for(auto t : test){
        if(t.uID == usr.first){
          auto p = r_u_i.find(t.cName);
          if(p != r_u_i.end()){
            if(p->second != p->second)
              MAE += abs(t.sentiment-0.5);
            else
              MAE += abs(p->second+t.sentiment);
          }
          break;
        }
        else{
          MAE += abs(t.sentiment);
        }
          sum++;
      }
    }

    // get top 5/2
    vector<pair<string,double>> vec;
    for(int i = 0; i < top; i++){
      double max = -2;
      string maxv;
      for(auto r : r_u_i){
        if(r.second > max){
          maxv = r.first;
          max = r.second;
        }
      }

      if(max != -2){
        if(max == 0){
          auto bn = notCryptos.begin();
          advance(bn, rand()%notCryptos.size());
          maxv = bn->data();
          vec.push_back(make_pair(maxv,max));
          r_u_i.erase(maxv);
        }
        else{
          vec.push_back(make_pair(maxv,max));
          r_u_i.erase(maxv);
        }
      }
    }

    if(vec.size() != 0)
      ret.insert(make_pair(usr.first,vec));
  }

  return ret;
}

// make R(u,i) to get top 5/2 cryptocurrencies - DEMO (1,0)
unordered_map<string,vector<pair<string,double>>> topClusterUDEMO(set<string> row, unordered_map<string,cluster> clusterMap, unordered_map<string,user> userMap, unordered_map<string,vector<double>> cVector, unordered_map<string,vector<double>> uVector, int top){
  
  unordered_map<string,vector<pair<string,double>>> ret;

  // for every user's P closest 
  for(auto usr : userMap){
    double z = 0.0;     // normalizing factor

    //cout << usr.first << endl;
    string clusterCenter;   // used for second iteration, to know on whitch cluster we are
    set<string> notCryptos;
    if(clusterMap.find(usr.first) != clusterMap.end()){    // a center
      clusterCenter = usr.first;
      notCryptos = makeNotCryptos(row,userMap.find(usr.first)->second);
    }
    else{  // not a center
      for(auto i : clusterMap){
        for(auto j : i.second.getUsers()){
          if(j.getUserID() == usr.first){ // found cluster
            clusterCenter = i.first;
            notCryptos = makeNotCryptos(row,usr.second);
          }
        }
      }
    }

    unordered_map<string,double> r_u_i;
    for(auto n : notCryptos){
      r_u_i.insert({n,0.0});
    }
    unordered_map<string,double>::iterator it;

    // for every user in P closest
    for(auto v : clusterMap.find(clusterCenter)->second.getUsers()){

      vector<double> v1 ,v2;
      if(uVector.find(v.getUserID()) != uVector.end())
        v1 = uVector.find(v.getUserID())->second;         // v vector
      if(uVector.find(usr.first) != uVector.end())
        v2 = uVector.find(usr.first)->second; // u vector
      
      double cs;
      if(v1.size() != 0 && v2.size() != 0){
        cs = cosine_similarity(v1,v2);
        z = 1/abs(cs);
      }
      else
        continue;

      if(userMap.find(v.getUserID()) != userMap.end()){
        for(auto cr : userMap.find(v.getUserID())->second.getCryptoHash()){

          if(notCryptos.find(cr.first) != notCryptos.end()){

            it = r_u_i.find(cr.first);
            if(it != r_u_i.end())
              r_u_i.insert({cr.first,(cr.second+it->second)*cs*z});
            else
              r_u_i.insert({cr.first,cr.second*cs*z});
          }
        }
      }
      else
        continue;
    }

    // used if validation enabled
    if(validation){
      for(auto t : test){
        if(t.uID == usr.first){
          auto p = r_u_i.find(t.cName);
          if(p != r_u_i.end()){
            if(p->second != p->second)
              MAE += abs(t.sentiment-2);
            else
              MAE += abs(p->second-t.sentiment);
            sum++;
          }
          break;
        }
      }
    }

    // get top 5/2unordered_map<string,cluster> clusterMap
    vector<pair<string,double>> vec;
    for(int i = 0; i < top; i++){
      double max = -2;
      string maxv;
      for(auto r : r_u_i){
        if(r.second > max){
          maxv = r.first;
          max = r.second;
        }
      }

      if(max != -2){
        if(max == 0){
          auto bn = notCryptos.begin();
          advance(bn, rand()%notCryptos.size());
          maxv = bn->data();
          vec.push_back(make_pair(maxv,max));
          r_u_i.erase(maxv);
        }
        else{
          vec.push_back(make_pair(maxv,max));
          r_u_i.erase(maxv);
        }
      }
    }

    if(vec.size() != 0)
      ret.insert(make_pair(usr.first,vec));
  }

  return ret;
}

// 2b
unordered_map<string,vector<pair<string,double>>> topClusterC(set<string> row, unordered_map<string,cluster> clusterMap, unordered_map<string,user> userMap, unordered_map<string,vector<double>> cVector, unordered_map<string,vector<double>> uVector, int top, user usr){
  
  unordered_map<string,vector<pair<string,double>>> ret;

  double z = 0.0;     // normalizing factor

  //cout << usr.first << endl;
  string clusterCenter;   // used for second iteration, to know on whitch cluster we are
  set<string> notCryptos;
  // not a center
  for(auto i : clusterMap){
    for(auto j : i.second.getUsers()){
      if(j.getUserID() == usr.getUserID()){ // found cluster
        clusterCenter = i.first;
        notCryptos = makeNotCryptos(row,usr);
      }
    }
  }

  unordered_map<string,double> r_u_i;
  for(auto n : notCryptos){
    r_u_i.insert({n,0.0});
  }
  unordered_map<string,double>::iterator it;

  // for every user in P closest
  for(auto v : clusterMap.find(clusterCenter)->second.getUsers()){

    vector<double> v1 ,v2;
    if(cVector.find(v.getUserID()) != cVector.end())
      v1 = cVector.find(v.getUserID())->second;         // v vector
    if(cVector.find(usr.getUserID()) != cVector.end())
      v2 = cVector.find(usr.getUserID())->second; // u vector
      
    double cs;
    if(v1.size() != 0 && v2.size() != 0){
      cs = cosine_similarity(v1,v2);
      z = 1/abs(cs);
    }
    else
      continue;

    if(userMap.find(v.getUserID()) != userMap.end()){
      for(auto cr : userMap.find(v.getUserID())->second.getCryptoHash()){

        if(notCryptos.find(cr.first) != notCryptos.end()){

          it = r_u_i.find(cr.first);
          if(it != r_u_i.end())
            r_u_i.insert({cr.first,(cr.second+it->second)*cs*z});
          else
            r_u_i.insert({cr.first,cr.second*cs*z});
        }
      }
    }
    else
      continue;
  }

  // used if validation enabled
    if(validation){
      for(auto t : test){
        if(t.uID == usr.getUserID()){
          auto p = r_u_i.find(t.cName);
          if(p != r_u_i.end()){
            if(p->second != p->second)
              MAE += abs(t.sentiment-0.5);
            else
              MAE += abs(p->second-t.sentiment);
            sum++;
          }
          break;
        }
      }
    }

  // get top 5/2unordered_map<string,cluster> clusterMap
  vector<pair<string,double>> vec;
  for(int i = 0; i < top; i++){
    double max = -2;
    string maxv;
    for(auto r : r_u_i){
      if(r.second > max){
        maxv = r.first;
        max = r.second;
      }
    }

    if(max != -2){
      if(max == 0){
        auto bn = notCryptos.begin();
        advance(bn, rand()%notCryptos.size());
        maxv = bn->data();
        vec.push_back(make_pair(maxv,max));
        r_u_i.erase(maxv);
      }
      else{
        vec.push_back(make_pair(maxv,max));
        r_u_i.erase(maxv);
      }
    }
  }

  if(vec.size() != 0)
    ret.insert(make_pair(usr.getUserID(),vec));

  return ret;
}

void export_vector(string filename, unordered_map<string,vector<double>> uVector){

  fstream fs;
  fs.open(filename, fstream::in | fstream::out | fstream::app);

  for(auto usr : uVector){

    fs << usr.first;
    for(auto i : usr.second){
      fs << "," << i;
    }
    fs << endl;

  }

  fs.close();
}

void myread(int argc, char** argv, string* inputFile, string* outputFile, bool* comp){

  int opt;
  *comp = 0;

  while ((opt = getopt (argc, argv, "v:o:d:")) != -1)
  {
    switch (opt){
      case 'd':
        if(optarg){
          *inputFile = optarg;
          cout << "Input file: " << *inputFile << endl;
          break;
        }
        else
          cout << "No Input file given" << endl;
      case 'o':
        if(optarg){
          *outputFile = optarg;
          cout << "Output file: " << *outputFile << endl;
          break;
        }
        else
          cout << "No Output file given " << endl;
      case 'v':
        if(optarg){
          *comp = 1;
          cout << "Validation: " << *comp << endl;
          break;
        }
    }
  }
}