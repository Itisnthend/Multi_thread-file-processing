#include "p1_process.h"
#include "p1_threads.h"

// This file includes functions that actually perform the
// computation. You can create a thread with these function
// or directly call this function

void* dsum(void* param)
{
	struct dparameter *para;
	para = (struct dparameter*)param;
	double tsum = 0.0;
	int length = (int)(1000000/para->pnum_threads)+1;
	int counter = 0;

	//std::cout<<"dsum1"<<std::endl;
	while(counter<length&&(para->j*length+counter)<sVector->size())
	{
		tsum += pow((sVector->at(para->j*length+counter).ss-para->ave),2.0);
		counter++;
	}
	
	//std::cout<<"dsum1 tsum = "<<tsum<<std::endl;
	pthread_mutex_lock(&mutex);
	sum_d += tsum;
	pthread_mutex_unlock(&mutex);
	//std::cout<<"dsum1 sum_d = "<<sum_d<<std::endl;
	
	pthread_exit(NULL);
}

std::string Trim(std::string& str)
{
	str.erase(0,str.find_first_not_of("\t\r\n"));
	str.erase(str.find_last_not_of("\t\r\n")+1);
	return str;
}

void *sort_stat(void* arg)
{
	struct parameter *argt;
	argt = (struct parameter*)arg;
	//std::cout<<"threads id = "<<argt->j<<std::endl;
	double tave;
	int length = (int)(1000000/argt->pnum_threads)+1;
	//std::cout<<"abaituo"<<std::endl;
	int counter = 0, counter1 = 0;
	std::vector<student> tstudents;
	struct student students;
	//std::cout<<"students.ssl : "<<students.ss<<std::endl;
	//sleep(100);
	std::string line;
	double tsum = 0.0;


	//std::cout<<"a111"<<std::endl;
	//std::cout<<"argt:"<<argt->j<<std::endl;
	getline(argt->ifile,line);
	//std::cout<<"getline:"<<line<<std::endl;
	//sleep(100);
	while(getline(argt->ifile,line)&&counter<(argt->j+1)*length)
	{
		//std::cout<<"line = "<<line<<std::endl;
		if(counter>=argt->j*length)
		{

			std::istringstream sin(line);
			std::vector<std::string> strs;
			std::string str;
			while(getline(sin, str, ','))
			{
				strs.push_back(str);
			}

			std::string stid = Trim(strs[0]);
			std::string sts = Trim(strs[1]);
			//std::cout<<"stid : "<<stid<<std::endl;
			//std::cout<<"sts : "<<sts<<std::endl;
			students.sid = stid;
			students.ss = stod(sts);
			//std::cout<<"students.sid : "<<students.sid<<std::endl;
			//std::cout<<"students.ss : "<<students.ss<<std::endl;
			tstudents.push_back(students);
			tsum += students.ss;
			//std::cout<<"students.ssl : "<<students.ss<<std::endl;
	        //sleep(100);
		}
		counter++;
	}
	//std::cout<<"inside counter = "<<counter<<std::endl;
	//sleep(200);
	//std::cout<<"tstudentssize() =  : "<<tstudents.size()<<std::endl;
	//std::cout<<"pppp sum= "<<sum<<std::endl;
	pthread_mutex_lock(&mutex);
	sum += tsum;                                                  
	pthread_mutex_unlock(&mutex);
	//std::cout<<"sum = "<<sum<<std::endl;

	//std::cout<<"inside tstudentssize = "<<tstudents.size()<<std::endl;
	//sleep(200);
	merge_sort(tstudents, tstudents.size());
	//std::cout<<"77777"<<std::endl;

	pthread_mutex_lock(&mutex);
	//std::cout<<"before counter1 = "<<counter1<<std::endl;
	while(counter1<tstudents.size())
	{
		sVector->push_back(tstudents[counter1++]);
		//std::cout<<"inside sVector = "<<sVector->at(counter1-1).sid<<std::endl;
	}
	//std::cout<<"after counter1 = "<<counter1<<std::endl;
	//std::cout<<"inside counter1 = "<<counter1<<std::endl;
	//sleep(200);

	pthread_mutex_unlock(&mutex);
	//std::cout<<"888888 sVectorsize() = "<<sVector->size()<<std::endl;
	//sleep(200);
		

	pthread_exit(NULL);
}


void merge(std::vector<student>& arr, int n)
{
	std::vector<student> ans(n);
	for(int hstep = 1; hstep<n; hstep+=hstep)
	{
		for(int idex = 0; idex<n; idex+=2*hstep)
		{
			int left = idex, mid = std::min(idex+hstep,n), right = std::min(idex+hstep+hstep,n);
			int i = left, j = mid, k = left;
			while(i<mid&&j<right)
			{
				if(arr[i].ss>arr[j].ss)
				{
					ans[k].sid = arr[j].sid;
					ans[k++].ss = arr[j++].ss;
				}
				else
				{
					ans[k].sid = arr[i].sid;
					ans[k++].ss = arr[i++].ss;
				}
			}
			while(i<mid)
			{
				ans[k].sid = arr[i].sid;
				ans[k++].ss = arr[i++].ss;
			}
			while(j<right)
			{
				ans[k].sid = arr[j].sid;
				ans[k++].ss = arr[j++].ss;
			}
			for(k = idex; k< right; k++)
			{
				arr[k].sid = ans[k].sid;
				arr[k].ss = ans[k].ss;
			}
		}
	}
}

void merge_sort(std::vector<student>& arr, int N) 
{
	//std::cout<<"444444"<<std::endl;
    if(arr.empty()) return;
    merge(arr,N);
}

unsigned int strtoul(std::string str)
{
	unsigned int result = 0;
	for(int i = str.size()-1; i>=0; i--)
	{
		unsigned int temp = 0, k = str.size()-i-1;
		if(isdigit(str[i]))
		{
			temp = str[i]-'0';
			while(k--)
				temp *= 10;
			result += temp;
		}
		else
		{
			break;
		}
	}
	return result;
}

