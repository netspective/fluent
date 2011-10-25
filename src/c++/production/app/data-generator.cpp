/* tcpserver.c */

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
//#include <iostream>
#include <unistd.h>
int main()
{
	int sock, connected, bytes_recieved , flag = 1;  
	char send_data [1024] , recv_data[1024], temp[1024];       

	struct sockaddr_in server_addr,client_addr;    
	socklen_t sin_size;
	int i=1;
	int sys, dias, pulse;
	int spo2, pulseox;
	int temp_monitor;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(int)) == -1) {
		perror("Setsockopt");
		exit(1);
	}

	server_addr.sin_family = AF_INET;         
	server_addr.sin_port = htons(5000);     
	server_addr.sin_addr.s_addr = INADDR_ANY; 
	bzero(&(server_addr.sin_zero),8); 

	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
		perror("Unable to bind");
		exit(1);
	}

	if (listen(sock, 5) == -1) {
		perror("Listen");
		exit(1);
	}

	printf("\nData Generator Waiting for command");
	fflush(stdout);

	while(1) {  
		sin_size = sizeof(struct sockaddr_in);

		connected = accept(sock, (struct sockaddr *)&client_addr,&sin_size);

		printf("\n I got a connection from (%s , %d)",
		inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

		bytes_recieved = recv(connected,recv_data,1024,0);
		recv_data[bytes_recieved] = '\0';

		if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0) {
			close(connected);
			break;
		} else {
			printf("\n RECIEVED DATA = %s " , recv_data);
		}
		fflush(stdout);
		while(1) {

			if (strcmp(recv_data , "BP") == 0 ) {
				while (i<=10) {

					printf("\nSend %d",i);
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

					send(connected, send_data,strlen(send_data), 0);
					sleep(1);
					fflush(stdout);
					i++;
					strcpy(send_data,"");
				}
			}
			else if(strcmp(recv_data , "PULSEOX") == 0 )
			{
				while (i<=10) {

                                        printf("\nSend %d",i);
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

                                        send(connected, send_data,strlen(send_data), 0);
                                        sleep(1);
                                        fflush(stdout);
                                        i++;
                                        strcpy(send_data,"");
                                }
			}
			else if(strcmp(recv_data , "TEMPMONITOR") == 0 ) 
			{
					while (i<=10) {

                                        printf("\nSend %d",i);
                                        sprintf(temp, "%d", time(0));
                                        strcat(send_data,temp);
                            
			                temp_monitor = rand()%60+50;
                                        sprintf(temp, "%d", temp_monitor);
                                        strcat(send_data,":");
                                        strcat(send_data,temp);

                                        send(connected, send_data,strlen(send_data), 0);
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
		

//			printf("\nTransfer Completed\n\n");
			i=1;
			fflush(stdout);
			sleep(2);
		}
	}       
	close(sock);
	return 0;
} 




