/*
test and compare the excution time of following function(user & kernel):
--fgets(),fgetc(),fread(),read()

*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
  FILE *fp;
  FILE *f;
  
//产生10MB的文件“77”（文件名） generate test file (size: 10M)
  if((fp=fopen("77","w"))==0) 
  { 
     printf("\nopen file error"); 
     exit(1); 
  }
  int j=0;
  for (j=0;j<=400000;j++){fputs("3423434234342343423434234\n",fp);}
  fclose(fp);

//用fgets实现copyfile，并计时
  if((fp=fopen("77","r"))==NULL) 
  { 
     printf("\nopen file error"); 
     getchar(); 
     exit(1); 
  }
  if((f=fopen("88","w"))==NULL) 
  { 
     printf("\nopen file error"); 
     exit(1); 
  }

  char s[100];
  clock_t sta=clock();
  while (fgets(s,sizeof(s),fp)!=NULL){fputs(s,f);};
  clock_t sto=clock();
  double time=sto-sta;
  printf("Time spent of \"fgets\" is :%f\n",time/CLOCKS_PER_SEC);

  fclose(f);
  fclose(fp);
//用fgetc实现copyfile，并计时
  if((fp=fopen("77","r"))==NULL) 
  { 
     printf("\nopen file error"); 
     exit(1); 
  }
  if((f=fopen("99","w"))==NULL) 
  { 
     printf("\nopen file error"); 
     exit(1); 
  }
  char ch;
  sta=clock();
  while(!feof(fp))
	{
		ch=fgetc(fp);
		fputc(ch,f);
	}

  sto=clock();
  time=sto-sta;
  printf("Time spent of \"fgetc\" is :%f\n",time/CLOCKS_PER_SEC);
  fclose(fp);
  fclose(f);

//用fread实现copyfile，并计时
  if((f=fopen("66","w"))==NULL) 
  { 
     printf("\nopen file error"); 
     exit(1); 
  }
  if((fp=fopen("77","r"))==NULL) 
  { 
     printf("\nopen file error");  
     exit(1); 
  }
  sta=clock();
  size_t bytes;
  
  while((bytes=fread(s,sizeof(char),100,fp))>0)
        fwrite(s,sizeof(char),bytes,f);
  sto=clock();
  time=sto-sta;
  printf("Time spent of \"fread\" is :%f\n",time/CLOCKS_PER_SEC);
  fclose(fp);
  fclose(f);

//用read实现copyfile，并计时
  int i;
  int fin,fout;
  if((fin = open("77",O_RDONLY)) < 0){
      perror("open error");
      exit(1);
  }
  if((fout = open( "55",O_RDWR | O_CREAT | O_TRUNC,00600)) < 0 ){
      perror( "write error" );
      exit(1);
  } 
  sta=clock();
  
  while ((i=read(fin,s,100))!=0) 
  write(fout,s,i);
  sto=clock();
  time=sto-sta;
  printf("Time spent of \"read\"  is :%f\n",time/CLOCKS_PER_SEC);
 
  return 0;
}
