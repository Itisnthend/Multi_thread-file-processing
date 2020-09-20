#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <pthread.h>

#include "p1_process.h"
#include "p1_threads.h"


using namespace std;

int main(int argc, char** argv)
{
	printf("Main process is created. (pid: %d)\n", getpid());
	int num_processes = 0;
	int num_threads = 0;

	std::string class_name[] = {"os", "architecture", "java", "algorithm", "digital-design"};

	// Check the argument and print error message if the argument is wrong
	if(argc == 3 && (atoi(argv[1]) > 0 && atoi(argv[2]) > 0))
	{
		num_processes = atoi(argv[1]);
		num_threads = atoi(argv[2]);
		// Call the process
		if(num_processes>5)
		{
			cout<<"Number of processes is to many, there will be some non-active processes"<<endl;
			return 1;
		}
		get_statistics(class_name, num_processes, num_threads);
	}
	else
	{
		printf("[ERROR] Expecting 2 arguments with integral value greater than zero.\n");
		printf("[USAGE] p1_exec <number of processes> <number of threads>\n");
	}
	printf("Main process is terminated. (pid: %d)\n", getpid());
	delete sVector;
	return 0;
	
}
