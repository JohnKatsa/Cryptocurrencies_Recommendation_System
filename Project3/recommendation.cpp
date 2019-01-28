#include "recommendation.h"

using namespace std;

unordered_map<string,unordered_set<string>> closest;	// uses closest_ids for project 3
unordered_map<string,string> tweetToUser;   // use to go from tweet to user
unordered_map<string,string> clusterToUser;   // use to go from cluster to user

vector<valType> train;  // used for validation
vector<valType> test;   // used for validation
int validation;         // validation flag
double MAE;
int sum = 0;

// functions for validation
vector<valType> makePairSet(unordered_map<string,user> userMap){

  vector<valType> ret;

  for(auto usr : userMap){

    for(auto cr : usr.second.getCryptoHash()){

      valType v;
      v.uID = usr.first;
      v.cName = cr.first;
      v.sentiment = cr.second;
      ret.push_back(v);

    }

  }

  return ret;
}



int main(int args, char** argv)
{
  bool comp = 0;
  string inputFile;
  string outputFile;
  myread(args, argv, &inputFile, &outputFile, &comp);

  vector<string> methods = {"1a","1b","2a","2b"};  //1a,2a,1b,2b

  validation = comp;

  fstream fs;
  if(!validation)
    fs.open(outputFile, fstream::in | fstream::out | fstream::app);
  
  double totalMAE = 0.0;
  int sumMAE = 0;

  float time1 = 0.0;

  // set map: userID --> user object
  unordered_map<string,user> userMap = makeUserHash(inputFile);

  // set map: word --> sentiment
  unordered_map<string,double> lexiconMap = makeLexiconHash("../dataset/vader_lexicon.csv");

  // set map: other crypto reference names - crypto identifier
  unordered_map<string,string> cryptoMap = makeCryptoHash("../dataset/coins_queries.csv");

  // set sentiment and cryptos referred of every tweet. Also make uj vector (Bonus: make tweet map)
  unordered_map<string,tweet> tweetMap = giveSentimentToTweet(userMap,lexiconMap,cryptoMap);

  // read project 2 clusters and make cluster hashes. Also make cj vector
  unordered_map<string,cluster> clusterMap = makeClusterHash("../Project2/out",tweetMap);

  // make vector of cryptos (used for the row)
  set<string> row = makeRow(cryptoMap);

  // make vector of uj (unordered_map)
  unordered_map<string,vector<double>> uVector;
  for(auto usr : userMap){
    uVector[usr.first] = makeUVector(usr.second,row);
  }

  // make vector of cj (unordered_map)
  unordered_map<string,vector<double>> cVector;
  for(auto cluster : clusterMap){
    cVector[cluster.first] = makeCVector(cluster.second,row);
  }

  // export vectors to files (comma separated)
  export_vector("uj_input",uVector);
  export_vector("cj_input",cVector);

  int k = 4, L = 5, R = 100000, n;

  int folds;
  if(validation)
    folds = 10;
  else
    folds = 1;

  for(string method : methods){

    if(!validation)
      fs << method << endl;
    
    cout << "Method : " << method << endl;

    for(int l = 0; l < folds; l++){
      /* <validation> */
      // make set of pairs (uid,crypto)
      train = makePairSet(userMap);
      
      // take 1/10 (shuffle/take first part)
      random_shuffle(train.begin(),train.end());
      vector<valType> testlocal(train.begin(),train.begin()+train.size()/10);
      test = testlocal;

      // for every item in test take it off Map
      for(auto t : test){
        auto usr = userMap.find(t.uID);
        if(usr != userMap.end()){
          usr->second.rmCryptoHash(t.cName);
        }
      }
      /* </validation> */

      // (0,0) : (uj,LSH)
      if(method == "1a"){
        const clock_t begin_time = clock();
        // make hashtables
        hashtable** table;
        dataset* data = read_inputFile((char*)"uj_input",&n);
        table = lsh_main(data,k,L,"cosine");

        // NN 
        for(int i = 0; i < n; i++)
          data = nearest_neighbours(data,&data[i],k,VECTORSIZE,L,table,"cosine",R,0,(char*)"out");
        
        // export 5 best cryptos
        vector<pair<string,vector<pair<string,double>>>> top5 = topUser(row,userMap,uVector,5);
        
        if(!validation){
          for(auto i : top5){
            fs << i.first << " ";
            for(auto j : i.second)
              fs << j.first << " ";
            fs << endl;
          }
        }
        else{
          //cout << "MAE : " << (1.0/sum)*MAE << endl;
          totalMAE += (1.0/sum)*MAE;
          sumMAE++;
        }
        cout << "TIME : " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " secs" << endl;
        time1 += float( clock () - begin_time ) /  CLOCKS_PER_SEC;

        remove("outuj");
      }
      ////////////////////////

      // (1,0) : (cj,LSH)
      else if(method == "1b"){
        closest.clear();
        const clock_t begin_time = clock();
        hashtable** table2;
        dataset* data2 = read_inputFile((char*)"cj_input",&n);
        table2 = lsh_main(data2,k,L,"cosine");

        for(int i = 0; i < n; i++)
          data2 = nearest_neighbours(data2,&data2[i],k,VECTORSIZE,L,table2,"cosine",R,0,(char*)"out");

        // export 2 best cryptos
        unordered_map<string,vector<pair<string,double>>> top2 = topCluster(row,clusterMap,cVector,2);
        
        // transform tweets/clusters to users
        unordered_map<string,vector<pair<string,double>>> user_top2;
        for(auto i : top2){
          vector<tweet> a = clusterMap.find(i.first)->second.getTweets();
          vector<pair<string,double>> v = i.second;
          for(auto j : a){
            string u = tweetToUser.find(j.getTweetID())->second;
            user_top2.insert({u,v});
          }
        }

        if(!validation){
          for(auto i : user_top2){
            fs << i.first << " ";
            for(auto j : i.second)
              fs << j.first << " ";
            fs << endl;
          }
        }
        else{
          //cout << "MAE : " << (1.0/sum)*MAE << endl;
          totalMAE += (1.0/sum)*MAE;
          sumMAE++;
        }

        cout << "TIME : " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " secs" << endl;
        time1 += float( clock () - begin_time ) /  CLOCKS_PER_SEC;
      }
      ////////////////////////


      // (0,1) : (uj,CLUSTERING)
      else if(method == "2a"){
        const clock_t begin_time = clock();
        cluster_main((char*)"uj_input",(char*)"../Project2/cluster.conf",(char*)"outuj",(char*)"euclidean",(char*)"112");
        unordered_map<string,cluster> clusterMapUj = makeClusterHash("outuj",userMap);
        // make vector of uj (unordered_map)
        cVector.clear();
        for(auto usr : clusterMapUj){
          cVector[usr.first] = makeCVector(usr.second,row);
        }

        unordered_map<string,vector<pair<string,double>>> top52 = topClusterUDEMO(row,clusterMapUj,userMap,cVector,uVector,5);

        if(!validation){
          // export 5 best cryptos
          for(auto i : clusterToUser){
            auto x = top52.find(i.second);
            if(x != top52.end()){
              fs << i.first << ": ";
              for(auto j : x->second)
                fs << j.first << ", ";
              fs << endl;
            }
          }
        }
        else{
          //cout << "MAE : " << (1.0/sum)*MAE << endl;
          totalMAE += (1.0/sum)*MAE;
          sumMAE++;
        }

        cout << "TIME : " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " secs" << endl;
        time1 += float( clock () - begin_time ) /  CLOCKS_PER_SEC;

        remove("outuj");
      }
      ////////////////////////


      // (1,1) : (cj,CLUSTERING)
      else if(method == "2b"){
        const clock_t begin_time = clock();
        cluster_main((char*)"cj_input",(char*)"../Project2/cluster.conf",(char*)"outcj",(char*)"euclidean",(char*)"112");

        unordered_map<string,cluster> clusterMapCj1 = makeClusterHash("outcj",userMap);

        for(auto usr : userMap){
          cluster cl;
          string id = usr.second.getUserID();
          cl.setClusterID(id);
          cl.setCryptoHash(userMap.find(id)->second.getCryptoHash());
          clusterMapCj1.insert({id,cl});

          // make vector of cj (unordered_map)
          cVector.clear();
          for(auto cluster : clusterMapCj1){
            cVector[cluster.first] = makeCVector(cluster.second,row);
          }

          remove("cj_input2");
          export_vector("cj_input2",cVector);

          remove("outcj2");
          cluster_main((char*)"cj_input2",(char*)"cluster.conf",(char*)"outcj2",(char*)"euclidean",(char*)"112");

          unordered_map<string,cluster> clusterMapCj2 = makeClusterHash("outcj2",userMap);

          //cVector.clear();
          unordered_map<string,vector<double>> cVector2;
          for(auto usr : clusterMapCj2){
            cVector2[usr.first] = makeCVector(usr.second,row);
          }

          unordered_map<string,vector<pair<string,double>>> top25 = topClusterC(row,clusterMapCj2,userMap,cVector,uVector,2,userMap.find(id)->second);

          if(!validation){
            // export 2 best cryptos
            auto x = top25.find(id);
            if(x != top25.end()){
              fs << id << ": ";
              for(auto j : x->second)
                fs << j.first << ", ";
              fs << endl;
            }
          }
        }
        if(validation){
          //cout << "MAE : " << (1.0/sum)*MAE << endl;
          totalMAE += (1.0/sum)*MAE;
          sumMAE++;
        }
        cout << "TIME : " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << " secs" << endl;
        time1 += float( clock () - begin_time ) /  CLOCKS_PER_SEC;

        remove("cj_input2");
        remove("outcj");
        remove("outcj2");

      }

      // restore cryptos
      for(auto t : test){
        auto re = userMap.find(t.uID);
        re->second.addCryptoHash({t.cName,t.sentiment});
      }
      ////////////////////////

    }

    if(validation){
      cout << method << " : MAE = " << totalMAE/sumMAE << endl;
    }
    else{
      fs << "Execution Time: " << time1 << " secs" << endl << endl;
      time1 = 0.0;
    }
  }


  remove("uj_input");
  remove("cj_input");

  if(!validation)
    fs.close();

  return 0;
}