/*
Write a program:
-- calculate the values of PI endlessly;
-- print '.' for each second of CPU time used while calculating the values;
-- print the current value:
   one info-line per SIGINT in sig-handler
*/
#include <sys/select.h>
#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
int ans[1000000];
int i=0;
void myhandler(int sig)      
{
	char s='.';
        int errsave=errno;
	write(STDERR_FILENO,&s,1);
        errno=errsave;
}
void print(int x)
{
	printf("\n3.141");
	int j=0;
	for(j=1;j<i;j++){
		if (ans[j]<1000) printf("0%d",ans[j]); 
		else printf("%d",ans[j]);
	}
	printf("\n");
}
int main()
{
	struct itimerval value;
	struct sigaction act;
	act.sa_handler=myhandler;
	act.sa_flags=0;
	sigemptyset(&act.sa_mask);
	
	value.it_interval.tv_sec=1;    
	value.it_interval.tv_usec=0;  
        value.it_value=value.it_interval;  
  
	sigaction(SIGPROF,&act,NULL);
	setitimer(ITIMER_PROF, &value, NULL);
	
	signal(SIGINT,print);

	int a=10000,b=0,c=5000,d=0,e=2000,f[5001],g=0,tem;
	while(b!=(c+1)) f[b++]=2000;
	while(i!=1000000)
	{
	   g=c*2;
	   b=c;
	   while(b!=1)
	   {
	    d+=f[b]*a;
	    --g;
	    f[b]=d%g;
	    d/=g;
	    --g;
	    --b;
	    d*=b;
	   }
	   ans[i]=e+d/a;
	   e=d%a;
	   d=0;
	   ++i;
	}
	return 0;

	
}	
