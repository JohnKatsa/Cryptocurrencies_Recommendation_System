/*Implementation of hypercube functions*/

#include "cube.h"

hashtable** cube_main(dataset* data, int k, string family){
 int L = 1;

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
 hashtable** table = fill_hashtables(VECTORSIZE,k,L,data,family,1);
 return table;
}
