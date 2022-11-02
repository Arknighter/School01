#include<sys/types.h>
#include<sys/wait.h>
#include <sys/types.h>
#include <dirent.h>
#include<unistd.h>
#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
		pid_t pid,ppid;
		int pipe_fd[2];
		DIR *pdir=NULL;
		struct dirent *p_ent=NULL; 
		char *play_buff=NULL;
		pdir =opendir(argv[1]);
		assert(pdir!=NULL);
		pipe(pipe_fd);
		printf("read port = %d write port= %d\n",pipe_fd[0],pipe_fd[1]);
__FLAG:
		pid= fork();
		if(pid==0)// child process
		{
				printf("child process pid = %d\n",getpid());
				char path_buff[1024];
				int retur=0;
				close(pipe_fd[1]);
				retur=read(pipe_fd[0],path_buff,sizeof(path_buff));
				close(pipe_fd[0]);
				printf("child process receive string : %s\n",path_buff);
				execl("/usr/bin/mplayer","mplayer",path_buff,NULL);
		}
		else if(pid>0)// parent process
		{
			while((p_ent=readdir(pdir))!=NULL)
			{
					if(strstr(p_ent->d_name,".mp4")!=NULL|| \
							strstr(p_ent->d_name,".mp3")!=NULL|| \
							strstr(p_ent->d_name,".wmv")!=NULL)
					{
						play_buff=(char *)malloc(strlen(argv[1]+strlen(p_ent->d_name))+1);
						strcpy(play_buff,argv[1]);
						strcat(play_buff,p_ent->d_name);
						printf("parent process write string to pipe: \" %s \"\n",play_buff);
						write(pipe_fd[1],play_buff,strlen(play_buff)+1);
						free(play_buff); 
					    wait(NULL);	
					   goto __FLAG;
				   }
			}
			char pid_str[20];
			char buff[100];
			strcpy(buff,"kill -9 ");
			sprintf(pid_str,"%d",pid);
			strcat(buff,pid_str);
			system(buff);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			closedir(pdir);
		}	
		printf("pid = %d will exixt\n",getpid());
		return 1;
}
