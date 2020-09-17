#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SERVER_FIFO "server_fifo"
#define CLIENT_FIFO "client_fifo"
#define BUFF_SIZE 100

void* receiver(void *value){
	int sfd, cmd, pid;
	char buff[BUFF_SIZE];

	if((sfd = open(SERVER_FIFO,O_RDONLY)) == -1){
		printf("[SYSTEM] open error!!\n");
		return NULL;
	}

	while(1){
		memset(buff,0,BUFF_SIZE);

		if(read(sfd,buff,BUFF_SIZE) == -1){//sfd에서 buff로 데이터를 읽어들임 
			break;
		}

		printf("\n[SERVER] %s",buff);
		printf("[CLIENT] ");
		fflush(stdout);

		if (strncmp(buff, "quit", 4) == 0) {
			printf("Quit chatting\n");
			exit(0);
		}
	}

	close(sfd);
}

int main(){
	int cfd;
	int pid;
	char buff[BUFF_SIZE];
	char msg[BUFF_SIZE];
	pthread_t t_receiver;
	int quit;

	if((cfd = open(CLIENT_FIFO, O_RDWR)) == -1){
		printf("[SYSTEM] open error!!\n");
		exit(1);
	}

	pid = getpid();

	pthread_create(&t_receiver, NULL, receiver, NULL);

	quit = 0;
	while(quit == 0) {
		printf("[CLIENT] ");
		fgets(msg,BUFF_SIZE,stdin);
		write(cfd,msg,strlen(msg));
		if (strncmp(msg, "quit", 4) == 0) {
			printf("Quit chatting\n");
			pthread_cancel(t_receiver);
			quit = 1; 
		}
	}

	close(cfd);
}

