#include "lsh.h"

using namespace std;

int* r;
float* t;
float** v;
int tablesize;
int n;

hashtable** lsh_main(dataset* data, int k, int L, string family){

 srand(time(NULL));  // only once
 // initialize lsh constants
 if(!family.compare("euclidean")){
   // r
   r = new int[k];
   for(int i = 0; i < k; i++){
     r[i] = rand();
   }
   // Take k sample normal numbers
   std::random_device rd;
   std::mt19937 gen(rd());
   // t
   t = new float[L];
   for(int i = 0; i < L; i++){
     std::uniform_real_distribution<float> dist(0.0,W);
     t[i] = dist(gen);
   }
   // v
   v = new float*[L];
   for(int i = 0; i < L; i++){
     v[i] = new float[VECTORSIZE];
     for(int j = 0; j < VECTORSIZE; j++){
       std::normal_distribution<float> d(0,1);
       v[i][j] = d(gen);
     }
   }
 }

 // make hashtables
 hashtable** table = fill_hashtables(VECTORSIZE,k,L,data,family,0);
 return table;
}

hashtable* initialize_buckets(int tablesize, hashtable* table){
  for(int i = 0; i < tablesize; i++){
    table[i].set_bucket(new bucket);
    table[i].get_bucket()->set_next(NULL);
  }
  return table;
}

bitset<1>* convert_uniformly(int* h, int k){
  bitset<1>* ret = new bitset<1>[k];

  for(int i = 0; i < k; i++)
    ret[i] = h[i]%2;

  return ret;
}

// function to make k normal normal_distribution numbers
euclidean_return euclidean(int k, int d, type* p, int flag, int thesis){

  euclidean_return ret;
  int* h = new int[k];
  float* v_local;
  float t_local;

  int fi = 0;

  for(int j = 0; j < k; j++){

    // calculate normal v
    v_local = v[thesis];

    // calculate p*v
    float pv = 0;
    for(int i = 0; i < d; i++){
        pv += v_local[i]*p[i];
    }

    // calculate uniform t
    t_local = t[thesis];

    // calculate h(p)
    // calculate g(p) (= [h1,h2,...,hk] ---> fi(p))
    h[j] = floor((pv + t_local)/W);
    fi += r[j] * h[j];
  }

  // return the appropriate euclidean struct
  ret.g = h;

  if(flag){
    ret.fi = std::stoi(make_bit_string(convert_uniformly(h,k),k), nullptr, 2)%tablesize;
  }
  else{
    ret.fi = abs(fi%tablesize);
  }
  return ret;
}

char* make_bit_string(bitset<1> * h, int k){

  char* c = new char[k+1];
  for(int i = 0; i < k; i++){
    c[i] = h[i].to_string()[0];
  }
  c[k]='\0';
  return c;

}

int cosine_similarity(int k, int d, type* p){

  float* r = new float[d];
  bitset<1> * h = new bitset<1>[k];
  int g;

  // Take k sample normal numbers
  std::random_device rd;
  std::mt19937 gen(rd());

  for(int j = 0; j < k; j++){

    // calculate normal v
    for(int i = 0; i < d; i++){
      std::normal_distribution<float> d(0.0,1.0);
      r[i] = d(gen);
    }

    // calculate p*v
    float rp = 0;
    for(int i = 0; i < d; i++){
      rp += r[i]*p[i];
    }

    // calculate h(p)
    // calculate g(p) (= [h1,h2,...,hk] ---> fi(p))
    if(rp >= 0)
      h[j] = 1;
    else
      h[j] = 0;
  }

  char* c = make_bit_string(h,k);
  g = std::stoi(c, nullptr, 2);

  return g;
}

hashtable** make_hashtables(int L, int tablesize){

  hashtable** table = new hashtable*[L];
  for(int i = 0; i < L; i++){
    table[i] = new hashtable[tablesize];
  }

  for(int i = 0; i < L; i++){
    table[i] = initialize_buckets(tablesize,table[i]);
  }
  return table;
}

hashtable** fill_hashtables(int d, int k, int L, dataset* data, string family, int flag){

  //int tablesize;
  //n = 3521;
  //cout << "N = " << n << endl;
  if(!family.compare("cosine"))
    tablesize = pow(2,k);
  else
    tablesize = n;
  hashtable** table = make_hashtables(L,tablesize);

  int fi;
  euclidean_return e;
  for(int it = 0; it < n; it++){

    // insert item in L hashtables
    item* p = new item;
    p->set_p(data[it].get_xij());
    p->set_id(data[it].get_id());
    p->set_index(it);
    for(int i = 0; i < L; i++){
      if(!family.compare("cosine")){
        fi = cosine_similarity(k,d,p->get_p());
        p->set_fi(fi);
      }
      else{
        e = euclidean(k,d,p->get_p(),flag,i);
        p->set_fi(e.fi);
        p->set_g(e.g);
        fi = e.fi;
      }

		//cout << "fi = " << fi << endl;
      table[i][fi%tablesize].insert_point(p);
    }
  }

  return table;
}

int compare_g(int* g, int* g_bucket, int k){
  for(int i = 0; i < k; i++)
    if(g[i] != g_bucket[i])
      return 1;
  return 0;
}

void delete_hashtables(hashtable** table, int L){

  for(int i = 0; i < L; i++){
    bucket* b = table[i]->get_bucket();

    bucket* previous;
    while(b != NULL){
      previous = b;
      b = b->get_bucket();
      delete previous;
    }

  }
}
