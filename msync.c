#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#define MEM_SIZE 64

int main(int argc,char *argv[]){
	int fd;
	char *memPtr=NULL;
	struct stat sb;
	int flag=PROT_WRITE | PROT_READ;

	if(argc!=2){//인자를 2개를 받아야함 
		fprintf(stderr,"Usage:%s memPtr\n",argv[0]);
		exit(1);
	}

	if((fd=open(argv[1],O_RDWR | O_CREAT))<0){
		perror("File Open Error\n");
		exit(1);
	}

	if(fstat(fd,&sb)<0){
		perror("fstat error");
		exit(1);
	
	}

	memPtr=(char *)mmap(0,MEM_SIZE,flag,MAP_SHARED,fd,0);
	
	if(memPtr==(void *)-1){//char 배열  
		perror("mmap() error");	
		close(fd);
		exit(1);
	}

	printf("mem(%p),value(%s)",memPtr,memPtr);
	//mem <-->file (synchronization)
	while(1){
		scanf("%s",memPtr);
		if(!strncmp(memPtr,"exit",4)) break;
		if(msync(memPtr,MEM_SIZE,MS_SYNC)==-1){
			printf("mync() error(%s)\n",strerror(errno));
			break;
		}

	}

	if(munmap(memPtr,MEM_SIZE)==-1){
		printf("munmap() error(%s)\n",strerror(errno));
	}

	close(fd);

}
