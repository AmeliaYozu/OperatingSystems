#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

time_t set_time(time_t,char*);
int main(int argc, char * const argv[]){
  time_t start_time = 0;
  time_t end_time = 0;
  char start_time_str[100];
  char end_time_str[100];
    
  pid_t pid;
  pid_t cpid;
 
  char homedir[100];
  sprintf(homedir,"%s/.launchx.log",getenv("HOME"));

  pid =fork();

  //handling fork error
  if(pid<0){
    perror("fork error");
    exit(1);
  }
  //get start time
  start_time = set_time(start_time,start_time_str);
   
  if(pid==0){
    if(argc>=2){
      if(execvp(argv[1],&*argv+1)<0){
    	perror("Commond Not Found");
      exit(1);
      }
    }
    else{
      if(execlp("startx","startx",(char*)0)<0){
	perror("Commond Not Found");
	exit(1);
      }
    }
    exit(0);
  }
  
  else{
    while((cpid=waitpid(pid,NULL,WNOHANG))<=0){
      //  printf("wait for child process...\n");
      sleep(1);
    }
    FILE *fp;
    if((fp=fopen(homedir,"at+"))==NULL){
      perror("File open failed");
      exit(1);
    }
    //  sleep(1);
    end_time = set_time(end_time,end_time_str);

    //calculate hr, mins, secs
    int diff_secs = difftime(end_time,start_time);
    int hr = diff_secs/3600;
    int hr_left = diff_secs%3600;
    int mins = hr_left/60;
    int secs = hr_left%60;

    //prepare info for logging
   
    if(argc>=2){
         fprintf(fp,"%s: Start %s session...\n",start_time_str,argv[1]);
      fprintf(fp,"%s: Your %s session lasted %02dhours %02dmins %02dsecs\n",end_time_str,argv[1],hr,mins,secs);
    }
    else{
      fprintf(fp,"%s: Start startx session...\n",start_time_str,argv[1]);
      fprintf(fp,"%s: Your startx session lasted %02dhours %02dmins %02dsecs\n",end_time_str,hr,mins,secs); 
    }
    rewind(fp);
    fclose(fp);
    exit(0);
  }
  exit(0);
}

time_t set_time(time_t target_time, char *target_time_str){
  target_time=time(NULL);
  struct tm sys_time={0};
  localtime_r(&target_time,&sys_time);
  sprintf(target_time_str,"[%04d.%02d.%02d %02d:%02d:%02d]",sys_time.tm_year+1900,sys_time.tm_mon+1,sys_time.tm_mday,sys_time.tm_hour,sys_time.tm_min,sys_time.tm_sec);
  return target_time;
}
