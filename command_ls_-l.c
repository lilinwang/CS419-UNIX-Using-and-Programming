/*
Realization of command 'ls -l'
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

int get_mode(int mode)
{
    int flag=-1;
    char str[]="----------";
    if (S_ISDIR(mode)) {str[0] = 'd';flag=0;};
    if (S_ISCHR(mode)) {str[0] = 'c';flag=3;};
    if (S_ISBLK(mode)) {str[0] = 'b';flag=3;};
    if (S_ISLNK(mode)) {str[0] = 'l';flag=4;};

    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';

    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';

    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) {str[9] = 'x';if (flag<0)flag=1;}
    printf("%s",str);
    return flag;
}

void get_time(char *file)
{
    struct stat buf;
    struct tm *ts; 

    stat(file, &buf);
    ts=localtime(&buf.st_mtime);  
    printf("  %4d-%c%d-%c%d  %c%d:%c%d",(int)ts->tm_year+1900,(char)((ts->tm_mon+1)/10+48),(ts->tm_mon+1)%10,(char)(ts->tm_mday/10+48),ts->tm_mday%10,(char)(ts->tm_hour/10+48),ts->tm_hour%10,(char)(ts->tm_min/10+48),ts->tm_min%10); 
}

void strcut(char *s,char *ss,int n,int m){
  int i;
  for (i=0;i<n;i++)
    ss[i]=ss[m+i-1];
  ss[i]='\0';
}

void show(char *filename, struct stat *p)
{
    int ff,i,tem;
    char tt[5]="----";
    ff=get_mode(p->st_mode);
    printf("%3d ",(int)p->st_nlink);
    printf("%-11s ",getpwuid(p->st_uid)->pw_name);
    printf("%-11s ",getgrgid(p->st_gid)->gr_name);
    printf("%8ld ",(long)p->st_size);
    get_time(filename);
    tem=strlen(filename);
    if ((ff<0)&&(filename[tem-1]=='p')&&(filename[tem-4]=='.')&&(filename[tem-3]=='z')&&(filename[tem-2]=='i')) ff=2;
    if ((ff<0)&&(filename[tem-1]=='r')&&(filename[tem-4]=='.')&&(filename[tem-3]=='r')&&(filename[tem-2]=='a')) ff=2;
    if ((ff<0)&&(filename[tem-1]=='b')&&(filename[tem-4]=='.')&&(filename[tem-3]=='c')&&(filename[tem-2]=='a')) ff=2;
    switch (ff) {               
	case -1:
	  printf( " %s\n", filename);
	  break;
	case 0:
	  printf( " \033[01;34m%s \033[0m \n", filename ); 
	  break;
        case 1:  
          printf( " \033[01;32m%s \033[0m \n", filename ); 
	  break;
        case 2:  
          printf( " \033[01;31m%s \033[0m \n", filename ); 
	  break;
	case 3:  
          printf( " \033[01;93m%s \033[0m \n", filename ); 
	  break;
	case 4:  
          printf( " \033[01;36m%s \033[0m \n", filename ); 
	  break;
	default:
	  printf( "\n" );
    }
}

int main(int argc,char * argv[])
{
    DIR *dir;
    struct dirent *dp;
    struct stat p;
    char * dname;
    long total=0;
    if (argc == 1){
       dname=".";
       dir = opendir(dname);
       while ((dp = readdir(dir)) != NULL){
           if (dp->d_name[0]=='.') continue;
           stat(dp->d_name,&p);
           total+=(&p)->st_size;
           show(dp->d_name,&p);
       }
       closedir(dir);
       printf("总用量:%ld\n",total);
    }
    while (--argc)
    {
       total=0;
       printf("%s:/n",*++argv);
       dname=*argv;
       dir = opendir(dname);
       while ((dp = readdir(dir)) != NULL){
           if (dp->d_name[0]=='.') continue;
           stat(dp->d_name,&p);
           total+=(&p)->st_size;
           show(dp->d_name,&p);
       }
       closedir(dir);
       printf("总用量:%ld\n",total);
    }
    return 0;
}

