#include "p1_process.h"
#include "p1_threads.h"
//using namespace std;
//typedef std::cout cout;
//typedef std::endl endl;

double sum = 0.0;
double sum_d = 0.0;
pthread_mutex_t mutex;
std::vector<student>* sVector = new std::vector<student>();


void get_statistics(std::string class_name[], int num_processes, int num_threads) 
{

	// You need to create multiple processes here (each process
	// handles at least one file.)
	
	
	// Each process should use the sort function which you have defined  		
	// in the p1_threads.cpp for multithread sorting of the data. 
	
	// Your code should be able to get the statistics and generate
	// the required outputs for any number of child processes and threads.
	std::string filename;
	std::string ofilename1;
	std::string ofilename2;
	std::ofstream ofile;
	double median;
	double dev;
	double ave;
	int id;
	int assign = 5/num_processes;
	int left = 5%num_processes;
	struct parameter **arg = new parameter*[num_threads];
	struct dparameter **param = new dparameter*[num_threads];
	int n = 0;
	pid_t pid, wpid;
	pthread_t *tid = new pthread_t[num_threads];
	int ret;

	for(id  = 0; id< num_processes; id++)
	{	
		pid = fork();
		if(pid<0)
		{
			std::cout<<"Fork Failed"<<std::endl;
			exit(1);
		}
		if(pid == 0)
		{
			printf("Child process is created. (pid: %d)\n", getpid());
			break;
		}
	}

	//std::cout<<"my id = "<<id<<std::endl;
	if(pid==0)
	{
		for(int filecounter = 0; filecounter<= assign;filecounter++)
		{
			if(filecounter==assign)
			{
				if(id>=left)
				{
					for(int j = 0; j<num_threads;j++)
					{
						delete arg[j];
						delete param[j];
					}
					printf("child process is terminated. (pid: %d)\n", pid);
					exit(0);
				}
				else{
					filename = "./input/"+class_name[assign*num_processes+id] + ".csv";
					ofilename1 = "./output/"+class_name[assign*num_processes+id] + "_stat.csv";
					ofilename2 = "./output/"+class_name[assign*num_processes+id] + "_sorted.csv";
				}
			}
			else{
				filename = "./input/"+class_name[assign*id+filecounter] + ".csv";
				ofilename1 = "./output/"+class_name[id*assign+filecounter] + "_stat.csv";
				ofilename2 = "./output/"+class_name[id*assign+filecounter] + "_sorted.csv";
			}
			
			//filename = "/Users/itisnthend/code/p1_students/input/111.csv";///data/users/qianw14/os/hw1/proj1/p1_students
			//filename = "/data/users/qianw14/os/hw1/proj1/p1_students/input/111.csv";
			//std::cout<<"111"<<std::endl;
			for(int j = 0; j<num_threads; j++)
			{
				arg[j] = new parameter();
				//std::cout<<filename.c_str()<<std::endl;
				arg[j]->ifile.open(filename.c_str());
				//std::cout<<"hahha"<<std::endl;
				if(!arg[j]->ifile.is_open())						//ensure open file correctly
				{
					//std::cout<<"hahha1"<<std::endl;
					std::cout<<filename.c_str()<<" open error"<<std::endl;
					return;
				}
				//std::cout<<"hahha2"<<std::endl;
				arg[j]->pnum_threads = num_threads;
				arg[j]->j = j;
			}
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_mutex_init(&mutex,NULL);
			for(int j = 0; j<num_threads; j++)
			{
				//std::cout<<"arg[j]->pnum_threads = "<<arg[j]->pnum_threads<<std::endl;
				//std::cout<<"arg[j]->j = "<<arg[j]->j<<std::endl;
				ret = pthread_create(tid+j,&attr,sort_stat,(parameter*)(arg[j]));
				if(ret != 0){
					printf("Create pthread error.\n");
					return ;
				}
			}
			//std::cout<<"111"<<std::endl;
			for(int j = 0; j<num_threads; j++)
			{
				pthread_join(tid[j],NULL);
			}
			//std::cout<<"before sVector->size() = "<<sVector->size()<<std::endl;
			//sleep(200);
			//std::cout<<"psum = "<<sum<<std::endl;
			// for(int j = 0; j<20; j++)
			// {
			//  	std::cout<<"sVector = "<<sVector->at(j).ss<<std::endl;
			// }
			// sleep(100);
			merge_sort((*sVector), sVector->size());
			
			//std::cout<<"sum1 : "<<sum<<std::endl;
			//sleep(200);
			ave = sum/(double)(sVector->size());
			//std::cout<<"ave : "<<ave<<std::endl;

			//std::cout<<"444"<<std::endl;
			for(int j = 0; j<num_threads; j++)
			{
				param[j] = new dparameter();
				param[j]->pnum_threads = num_threads;
				param[j]->j = j;
				param[j]->ave = ave;
			}
			for(int j = 0; j<num_threads; j++)
			{
				pthread_create((tid+j),&attr,dsum,(dparameter*)param[j]);
			}
			//std::cout<<"555"<<std::endl;
			for(int j = 0; j<num_threads; j++)
			{
				pthread_join(tid[j],NULL);
			}
			//std::cout<<"666"<<std::endl;
			//std::cout<<"after sVector->size() = "<<sVector->size()<<std::endl;
			//sleep(200);

			dev = sqrt(sum_d/(double)(sVector->size()));

			median = (sVector->at(sVector->size()/2).ss+sVector->at(sVector->size()/2+1).ss)/2;

			
			ofile.open(ofilename1.c_str(), std::ios::out);
			//std::cout<<"777"<<std::endl;
			if(!ofile.is_open())								//ensure open file correctly
			{
				std::cout<<ofilename1.c_str()<<"open error"<<std::endl;
				return;
			}
			//std::cout<<"888"<<std::endl;
			ofile<<"Average"<<','<<"median"<<','<<"Std.Dev"<<std::endl;
			ofile<<ave<<','<<median<<','<<dev<<std::endl;
			ofile.close();
			//std::cout<<"999"<<std::endl;

			
			ofile.open(ofilename2.c_str(),std::ios::out);
			//std::cout<<"112"<<std::endl;
			if(!ofile.is_open()) 								//ensure open file correctly
			{
				std::cout<<ofilename2.c_str() <<"open error"<<std::endl;  
				return;
			}
			//std::cout<<"vectorsize"<<sVector->size()<<std::endl;

			ofile<<"Student ID"<<','<<"Grade"<<std::endl;
			//std::cout<<"final sVector->size() = "<<sVector->size()<<std::endl;
			for(int j = 0; j<(int)(sVector->size()); j++)
			{
				ofile<<sVector->at(j).sid;
				ofile<<',';
				ofile<<sVector->at(j).ss<<std::endl;
			}
			//std::cout<<"114"<<std::endl;
			sVector->clear();
			ofile.close();
			//std::cout<<"115"<<std::endl;
		}

		for(int j = 0; j<num_threads;j++)
		{
			delete arg[j];
			delete param[j];
		}
		printf("child process is terminated. (pid: %d)\n", pid);
		exit(0);
	}
	else{
		while ((wpid = wait(NULL)) > 0);
	}
	pthread_mutex_destroy(&mutex);

	//sleep(1000);
	delete []tid;
	delete []arg;
	delete []param;
}
