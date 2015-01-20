/*
Process - model  + pipe
Process - model: fork-exec-wait-exit
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc,char* argv[])
{
    int fd[2],cld_pid,status;
    char buf[200],len;
    if(pipe(fd)==-1){
	printf("Create pipe error.\n");
	exit(1);
    }
    if((cld_pid=fork())==0){
	close(fd[1]);
	len=read(fd[0],buf,sizeof(buf));
	printf("This is the child process.\n");
	execlp("/bin/ls","ls","-l",(char*)0);
    }else{
	close(fd[0]);
	if(argc==1){
		getcwd(buf,200);
	}
	else if(argc==2){
		strcpy(buf,argv[1]);
	}
	else{
		printf("Input Error.\n");
		exit(0);
	}
	len=write(fd[1],buf,200);
	waitpid(-1,&status,0);
        printf("This is the parent process.\n");
    }
    return 0;
}
