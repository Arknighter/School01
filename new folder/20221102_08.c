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
		//pid_t 定义linux进程pid ppid
		pid_t pid,ppid;
		//管道由pipe函数创建而成pipe(pipe_fd)经由参数pipe_fd返回两个文件描述符
		//pipe_fd[0]为读而打开,pipe_fd[1]为写而打开。pipe_fd[1]的输出是pipe_fd[0]的输入。
		//定义了2管道，一般pipe_fd[0]是接收管道描述，pipe_fd[1]是写管道描述用于进程间通讯的
		int pipe_fd[2];
		DIR *pdir=NULL;
		struct dirent *p_ent=NULL; 
		char *play_buff=NULL;
		pdir =opendir(argv[1]);//打开目录
		assert(pdir!=NULL);//判断打开的目录是否为null
		pipe(pipe_fd);//创建管道  主程序希望与子程序进行通信。
		//我们用pipe()函数为他们创建管道，这样他们就可以通过管道来通信了。
		printf("read port = %d write port= %d\n",pipe_fd[0],pipe_fd[1]);//输出提示信息
__FLAG:
		pid= fork();//创建进程
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
