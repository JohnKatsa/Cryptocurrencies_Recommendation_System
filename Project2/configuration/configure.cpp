/*File manipulation functions*/

#include "configure.h"

using namespace std;

// function to read arguments from command line
void read_arguments(int argc, char** argv,
  char** inputFile, char** configurationFile, char** outputFile, char** metric, char** style, bool* complete){

  int opt;

  while ((opt = getopt (argc, argv, "i:c:o:d:a:C:")) != -1)
  {
    switch (opt){
      case 'i':
        if(optarg){
          *inputFile = optarg;
          cout << "Input file: " << *inputFile << endl;
          break;
        }
        else
          cout << "No Input file given" << endl;
      case 'c':
        if(optarg){
          *configurationFile = optarg;
          cout << "Configuration file: " << *configurationFile << endl;
          break;
        }
        else
          cout << "No Configuration file given" << endl;
      case 'o':
        if(optarg){
          *outputFile = optarg;
          cout << "Output file: " << *outputFile << endl;
          break;
        }
        else
          cout << "No Output file given " << endl;
      case 'd':
        if(optarg){
          *metric = optarg;
          cout << "Metric: " << *metric << endl;
          break;
        }
        else
          cout << "No metric given " << endl;
      case 'a':
        if(optarg){
          *style = optarg;
          cout << "Style: " << *style << endl;
          break;
        }
      case 'C':
        if(optarg){
          *complete = optarg;
          cout << "Complete: " << *complete << endl;
          break;
        }
    }
  }
}

dataset* read_inputFile(char* inputFile, int* ret){

  FILE* fptr = fopen(inputFile,"r");

  size_t len = 0;
  char* buffer = NULL;
  char delimeters[5] = " ,\t\n";

  // calculate number of inputs
  n = 0;
  char ch = fgetc(fptr);
  while(ch != EOF){
    if(ch == '\n')
      n++;
    ch = fgetc(fptr);
  }
  *ret = n;
  // set fptr back to start
  fseek(fptr, 0, SEEK_SET);

  // make dataset array
  dataset* input = new dataset[n];

  int i = 0;  // used for dataset array
  int j = 0;  // used for data point array
  type* tmp;
  cout.precision(30);
  while(getline(&buffer,&len,fptr) != -1){
    char* token = strtok(buffer,delimeters);

    // set id and data points
    input[i].set_id(token);
    tmp = new type[VECTORSIZE];
    input[i].set_xij(tmp);

    token = strtok(NULL, delimeters);
    while(token != NULL){
      tmp[j] = atof(token);
      token = strtok(NULL, delimeters);
      j++;
    }

    // new loop initialization
    delete[] buffer;
    buffer = NULL;
    len = 0;
    j = 0;
    i++;
  }

  fclose(fptr);
  return input;

}

void configurate(char* configurationFile, int* clusters, int* hash_functions, int* hash_tables){

  FILE* fptr = fopen(configurationFile,"r");

  size_t len = 0;
  char* buffer;
  char delimeters[3] = ":\n";

  while (getline(&buffer,&len,fptr) != -1){
    char* token = strtok(buffer,delimeters);

    while(token != NULL){

      if(!strcmp(token,"number_of_clusters")){
        token = strtok(NULL, delimeters);
        *clusters = atoi(token);
      }
      else if(!strcmp(token,"number_of_hash_functions")){
        token = strtok(NULL, delimeters);
        *hash_functions = atoi(token);
      }
      else if(!strcmp(token,"number_of_hash_tables")){
        token = strtok(NULL, delimeters);
        *hash_tables = atoi(token);
      }

      token = strtok(NULL, delimeters);
    }
  }

  fclose(fptr);
}

void clean_data(dataset* data, int n)
{
  for(int i = 0; i < n; i++)
    delete[] data[i].get_xij();
  delete[] data;
}
