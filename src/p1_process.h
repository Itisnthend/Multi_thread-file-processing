#ifndef __P1_PROCESS
#define __P1_PROCESS

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <wait.h>

//#include "p1_threads.h"
//using namespace std;

void get_statistics(std::string class_name[], int num_processes, int num_threads);

struct parameter
{
	int j;
	std::ifstream ifile;
	int pnum_threads;
};

struct dparameter
{
	int j;
	int pnum_threads;
	int ave;
};


struct student
{
	std::string sid;
	double ss;
};

extern  double sum;
extern  double sum_d;
extern std::vector<student>* sVector;
extern pthread_mutex_t mutex;



#endif
