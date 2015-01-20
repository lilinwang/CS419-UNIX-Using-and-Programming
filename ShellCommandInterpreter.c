/*
A simple shell command interpreter
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include <fcntl.h>
#define WORDNUM 100
char* redir[10][3];
char* param[10][10];
int psize=0,status;

void get_word(char tem[]){
    char* c=tem;
    char *res;
    char *word=(char*)malloc((WORDNUM+1)*sizeof(char));
    int csize,asize,flag;
    csize=0;asize=0;flag=0;
psize=0;
    bzero(word,WORDNUM+1);
    while(*c) {
	if(*c=='>' || *c=='<'){
    	    if(csize!=0){
		strcpy(param[psize][asize],word);
    		asize++;csize=0;
    	    }
            if(*c=='>'){
    		if(*(c+1)=='>'){
		   flag=3;c++;
    	        }
    	       	flag=2;
    	    }
    	    else if(*c=='<'){flag=1;}
        }
    	else if(*c==' '){
    	    if(csize!=0){
    		word[csize]=0;
		csize++;
    		if(flag==0){
		    strcpy(param[psize][asize],word);
    		    asize++;
    		}
    		else{
    		    redir[psize][flag-1]=word;
    		    flag=0;
    		}
    	    }
    	    csize=0;
    	}
    	else if(*c=='|'){
    	    if(csize!=0){
    		word[csize]=0;
		csize++;
    		strcpy(param[psize][asize],word);
    		param[psize][asize+1]=NULL;
    	    }
    	    asize=0;csize=0;
	    psize++;
    	}
    	else{word[csize++]=*c;}			
    	c++;
    }
    if(csize!=0){
	word[csize++]=0;
	if(flag==0){
            strcpy(param[psize][asize],word);
	    asize++;	
	}
	else{
	    redir[psize][flag-1]=word;
	    flag=0;
	}
    }
    param[psize][asize] = NULL;
}

void ioredirect(int pnum){
    int tem;
    if(redir[pnum][1][0]!=0){
        tem=open(redir[pnum][1],O_WRONLY|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
	dup2(tem,1);
	close(tem);
    }
    if(redir[pnum][0][0]!=0){
        tem=open(redir[pnum][1],O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
        dup2(tem,1);
        close(tem);
    }
    if(redir[pnum][2][0]!=0){
        tem=open(redir[pnum][2],O_WRONLY|O_APPEND);
	dup2(tem,1);
	close(tem);
    }
}

int word_exec(int pnum){
    char *tem=param[pnum][0];
    
    if(strcmp(tem,"cd")==0){
	chdir(param[pnum][1]);
	return 0;
    }
    else if(strcmp(tem,"exit")==0){
	exit(0);
	return 0;
    }
//调用已有函数执行
    else{
    	if(fork()!=0){
            waitpid(-1,&status,0);
	    return 0;
	}
	else{	

//printf("************\n");
            ioredirect(pnum);
            execvp(param[pnum][0],param[pnum]);
	    exit(0);
	}
    }
//printf("88\n");
}

int run(){
    int fd[psize+1][2];
    int pnum=0,i;
    pid_t pid;
//printf("^^^%d^^^\n",psize);
    for(i=0;i<=psize;i++){
	if(pipe(fd[pnum])==-1){
	    printf("Create pipe error.\n");
	    exit(1);
	}
    }

//printf("222222\n");
    if(psize==0){
        word_exec(0);
	return 0;
    }
//printf("222222\n");
    while(pnum<psize){
	pid=fork();
	if(pid==0){
            if(pnum+1<psize) dup2(fd[pnum][1],1);
	    if(pnum!=0) dup2(fd[pnum-1][0],0);
	    for(i=0;i<psize;i++){
		close(fd[i][0]);
		close(fd[i][1]);
	    }
	    word_exec(pnum);
	    exit(0);
	}
	pnum++;
    }
    for(i=0;i<psize;i++){
	close(fd[i][0]);
	close(fd[i][1]);
    }
    for(i=0;i<psize;i++){
	wait(&status);
    }

    return 0;
}

int main(int argc,char* argv[])
{
    while(1){	
	int i,j,lsize=0;
	char dir[100],c;
	getcwd(dir,sizeof(dir));
	printf("%s$ ",dir);
	char line[100];
	bzero(line,99);
	c=fgetc(stdin);
        while (c && c !='\n'){//input from std input
           // if (c=='\n') break;
       	    line[lsize++] = c;
       	    c=fgetc(stdin);
 	}
	
	for(i=0;i<10;i++){//initialize
	  for(j=0;j<10;j++){
	     param[i][j]=(char*)malloc((WORDNUM+1)*sizeof(char));
	     bzero(param[i][j],WORDNUM+1);
	  }
	}
	for(i=0;i<10;i++){
	  for(j=0;j<3;j++){
	     redir[i][j]=(char*)malloc((WORDNUM+1)*sizeof(char));
	     bzero(redir[i][j],WORDNUM+1);
	  }
	}

	get_word(line);//parse input command
	char* tempcp;
	for(i=0;i<=psize;i++){
	    tempcp=param[i][0];
	    if(strcmp(tempcp,"exit")==0) exit(0);
	
	}
        run();


	for(i=0;i<10;i++){//release memory
	  for(j=0;j<10;j++){
	 	free(param[i][j]);
		param[i][j]=NULL;
	  }
	}
	for(i=0;i<10;i++){
	  for(j=0;j<3;j++){
		free(redir[i][j]);
		redir[i][j]=NULL;
	  }
	}
//printf("222222\n");
    }
    return 0;
}
