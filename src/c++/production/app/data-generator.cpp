#include <sys/types.h>
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
#include <unistd.h>
#include <assert.h>

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
				//printf("\nSend %d",i);
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

				send(client, send_data,strlen(send_data), 0);
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
				//printf("\nSend %d",i);
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

				send(client, send_data,strlen(send_data), 0);
				sleep(1);
				fflush(stdout);
				i++;
				strcpy(send_data,"");
			}
		}

		else if(strcmp(request , "TEMPMONITOR") == 0 )
		{
			while (1) 
			{
				//printf("\nSend %d",i);
				sprintf(temp, "%d", time(0));
				strcat(send_data,temp);

				temp_monitor = rand()%60+50;
				sprintf(temp, "%d", temp_monitor);
				strcat(send_data,":");
				strcat(send_data,temp);

				send(client, send_data,strlen(send_data), 0);
				sleep(1);
				fflush(stdout);
				i++;
				strcpy(send_data,"");
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
	char port_s[10]="5000";
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
		//ret = pthread_create(&id, NULL, handle_client,(void*)&client);
		assert(ret == 0);
	}
	close(server);
	return 0;
}

