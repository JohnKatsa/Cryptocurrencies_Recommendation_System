/*File manipulation function headers*/

#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include "../clustering/cluster.h"
#include "data.h"

#define VECTORSIZE 100

class dataset;

// get command line arguments
void read_arguments(int argc, char** argv, char** inputFile, char** configurationFile, char** outputFile, char** metric, char** style, bool* complete);
// read input file
dataset* read_inputFile(char* inputFile, int* ret);
// read configurations
void configurate(char* configurationFile, int* clusters, int* hash_functions, int* hash_tables);
// clean dataset
void clean_data(dataset* data, int n);

#endif
