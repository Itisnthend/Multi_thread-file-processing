#ifndef __P1_THREADS
#define __P1_THREADS

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


void *sort_stat(void* arg);
void merge(std::vector<student>& arr, int n);
void merge_sort(std::vector<student>& arr, int N);
void* dsum(void* param);
unsigned int strtoul(std::string str);
//void pmergepass(mparameter* arg)
std::string Trim(std::string& str);

#endif
