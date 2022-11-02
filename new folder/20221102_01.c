#include<stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
		DIR *pdir=NULL;
		struct dirent * pdetail=NULL;
		int i=0;
		printf("argv[1]=   %s\n",argv[1]);
		
		pdir=opendir(argv[1]);
		
		if(pdir==NULL)
		{
			exit(0);
		}
		//struct dirent *readdir(DIR *dirp);
		while((pdetail=readdir(pdir))!=NULL)
		{
					printf(" %d  %s  type= %d\n",i+1, \
										pdetail->d_name,\
										pdetail->d_type);
					i++;
		}
		closedir(pdir);
		return 1;
}
