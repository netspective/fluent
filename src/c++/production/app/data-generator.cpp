#include <sys/types.h>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <assert.h>
#include "ecgsyn.cpp"
using namespace std;
void* handle_client(void *arg) 
{
	int client, requestlen, len;
	char request[1024],send_data[1024],temp[1024];
	int i=1;
	int sys, dias, pulse;
	int spo2, pulseox;
	int temp_monitor;

	printf("\n inside the function ");
	client = *((int*)arg);
	bzero(request, 1024);
	/* Read the request */
	requestlen = 0;


	while (!requestlen) 
	{
		len = recv(client, request, 1024, 0);	
		request[len]='\0';
		printf("Request length %d %s\n",len,request);
		requestlen=len;	
		
		sleep(2);
	}

	fflush(stdout);
	while(1)
	{
		if (strcmp(request, "BP") == 0 ) 
		{
			while (1) 
			{
				sprintf(temp, "%d", time(0));
				strcat(send_data,temp);
				sys = rand()%30+110;
				sprintf(temp, "%d", sys);
				strcat(send_data,":");
				strcat(send_data,temp);

				dias = rand()%40+55;
				sprintf(temp, "%d", dias);
				strcat(send_data,":");
				strcat(send_data,temp);

				pulse = rand()%40+55;
				sprintf(temp, "%d", pulse);
				strcat(send_data,":");
				strcat(send_data,temp);

				send(client, send_data,strlen(send_data), MSG_NOSIGNAL);
				sleep(1);
				fflush(stdout);
				i++;
				strcpy(send_data,"");
			}
		}
		else if(strcmp(request, "PULSEOX") == 0 )
		{
			while (1) 
			{
				sprintf(temp, "%d", time(0));
				strcat(send_data,temp);

				spo2 = rand()%30+70;
				sprintf(temp, "%d", spo2);
				strcat(send_data,":");
				strcat(send_data,temp);


				pulseox = rand()%40+55;
				sprintf(temp, "%d", pulseox);
				strcat(send_data,":");
				strcat(send_data,temp);

				send(client, send_data,strlen(send_data), MSG_NOSIGNAL);
				sleep(1);
				fflush(stdout);
				i++;
				strcpy(send_data,"");
			}
		}

		else if(strcmp(request , "TEMPERATURE") == 0 )
		{
			while (1) 
			{
				fflush(stdout);	
				sprintf(temp, "%d", time(0));
                                strcat(send_data,temp);
                                sys = rand()%30+95;
                                sprintf(temp, "%d", sys);
                                strcat(send_data,":");
                	        strcat(send_data,temp);

                                send(client, send_data,strlen(send_data), MSG_NOSIGNAL);
                                sleep(1);
                                fflush(stdout);
                                i++;
                                strcpy(send_data,"");
                                strcpy(temp,"");
			}

		}
		else if(strcmp(strncpy(temp,request,3), "ECG") == 0 )
                {
			int m_count=0;
			std::stringstream ecgfile,rrfile,rrpcfile;
			string dsp[12];
			dsp[0] = strtok (request,":");
			for(int arn=0;arn<11;arn++)
			dsp[arn] = strtok (NULL,":");
			while(1)
			{
			time_t rawtime;
			time ( &rawtime );
			ecgfile<<rawtime;
			rrfile<<rawtime<<"rr";
			rrpcfile<<rawtime<<"rrpc";
			//dorun(ecgfile.str().c_str(),rrfile.str().c_str(),rrpcfile.str().c_str());	
			(float)atof(dsp[0].c_str());
			dorun(ecgfile.str().c_str(),rrfile.str().c_str(),rrpcfile.str().c_str(),(int)atoi(dsp[0].c_str()),(int)atoi(dsp[1].c_str()),(int)atoi(dsp[2].c_str()),(float)atof(dsp[3].c_str()),(float)atof(dsp[4].c_str()),(float)atof(dsp[5].c_str()),(float)atof(dsp[6].c_str()),(float)atof(dsp[7].c_str()),(float)atof(dsp[8].c_str()),(float)atof(dsp[9].c_str()),(float)atof(dsp[10].c_str()));	
			string ecg,rr,rrpc;
  			
			ifstream ifs(ecgfile.str().c_str());
			ifstream ifrr(rrfile.str().c_str());
			ifstream ifrrpc(rrpcfile.str().c_str());
			
				while(!ifrr.eof())
				{
				if(!ifs.eof())
				{
				if(!ifrrpc.eof())
				{
					fflush(stdout);
					stringstream ecgtemp;
					ecgtemp<<time(0)<<":";
					getline( ifs, ecg );
					for(int i = 0; i < ecg.length(); i++)
    					{
					           if( isspace(ecg[i]) )
					           ecg[i] = ':';
					}	
					ecgtemp <<ecg<<":";
					getline( ifrr, rr );
					ecgtemp <<rr <<":";
					getline( ifrrpc, rrpc );
					ecgtemp<<rrpc;
					if((strcmp(ecg.c_str(),"")!=0)&&(strcmp(rr.c_str(),"")!=0)&&(strcmp(rrpc.c_str(),"")!=0))
					send(client, ecgtemp.str().c_str(),1024, MSG_NOSIGNAL);
					ecgtemp.str("");			
					int newflag=-1;
					while(newflag <= 0&&!ifrr.eof())
					{
						newflag=recv(client, temp,1024, MSG_NOSIGNAL);
						cout<<"\nloop";
						
				
					}
					cout <<"\n"<< m_count++ <<" data Recv : "<<temp;
				}
				}
				}

			
			ifs.close();
			ifrr.close();
			ifrrpc.close();
			remove(ecgfile.str().c_str());
			remove(rrfile.str().c_str());
			remove(rrpcfile.str().c_str());
			ecgfile.str("");
			rrfile.str("");
			rrpcfile.str("");

			cout <<"\n#########################################################################";
			cout <<"\n NEW LOOP ";
			cout <<"\n#########################################################################";

		}
			
		
		}
		else
		{
			printf("\n Invalid command ");
			exit(1);
		}
		/* Close the connection */
		sleep(2);
	}
	close(client);
}


int create_server(char ipstring[20], char portstring[10]) 
{

	in_addr_t ip;
	int port, server, ret, on;
	struct sockaddr_in server_addr;

	ip = inet_addr(ipstring);
	port = atoi(portstring);

	server = socket(AF_INET, SOCK_STREAM, 0);
	assert(server > 0);
	on = 1;
	ret = setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)); 
	assert(ret == 0);

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = ip;

	ret = bind(server, (struct sockaddr*) &server_addr, sizeof(struct sockaddr_in));
	if (ret == -1) 
	{
		fprintf(stderr, "Error: unable to bind to %s:%s\n", ipstring, portstring);
		exit(-1);
	}
	ret = listen(server, 10);
	assert(ret == 0);

	return server;
}

int main() 
{
	int server, client, ret;
	struct sockaddr_in client_addr; 
	socklen_t addrlen;
	pthread_attr_t attr;
	pthread_t id;
	char ip_s[20]="127.0.0.1";
	char port_s[10]="5001";
	server = create_server(ip_s,port_s);
	while (1) 
	{	
		printf("\n Data Generator Waiting for Command\n");
		addrlen = sizeof(struct sockaddr_in);
		client = accept(server, (struct sockaddr *) &client_addr, &addrlen);
		if (client == -1) 
		{
			continue;
		}
		
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
		ret = pthread_create(&id, &attr, handle_client, (void*)&client);
		assert(ret == 0);
		sleep(1);
	}
	close(server);
	return 0;
}
