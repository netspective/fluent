#include <stdio.h>      
#include <pthread.h>    
#include <time.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 100
#define LINE_ARRAY_SIZE (MAX_LINE+1)
using namespace std;
int thr_id,thr_id1; 
pthread_t  p_thread;   

void* do_loop(void* data) { 
    	
	int x = *((int*)data); 
	if(x==1)
	{
		system("./bp-pub &");
	}
    	else if(x==2)
	{
		exit(1);
	}
    	else if(x==3)
	{
		exit(1);
	}
	else
	{
		exit(1);
	}
	
	pthread_exit(NULL); 
} 




int main(int argc, char* argv[]) 
{ 
	int x;
	while (1) 
	{
		
		cout<<"1----BLOOD_PRESSURE\n";
  		cout<<"2----PULSE_OX\n";
		cout<<"2----TEMPERATURE\n";
  		cout<<"Enter the choice : ";
  		cin>>x;
			

    		if(x==1)
		{
			thr_id = pthread_create(&p_thread, NULL, do_loop,(void*)&x); 		
		}
    		else if(x==2)
		{
			thr_id = pthread_create(&p_thread, NULL, do_loop,(void*)&x); 		
		}
    		else if(x==3)
		{
			thr_id = pthread_create(&p_thread, NULL, do_loop,(void*)&x); 		
		}
		else
		{
			exit(1);
		}

  	}
 
   
    return 0; 
} 
