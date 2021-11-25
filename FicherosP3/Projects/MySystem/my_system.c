#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define N 10


int my_system(char* cmd)
{
	pid_t child_pid;
	int stat=0;

	
	child_pid = fork();
	if (child_pid == 0)
	{
		// Este codigo lo ejecuta SOLO el hijo
		execl("/bin/bash","/bin/bash","-c",cmd,NULL);
			//execv("/bin/bash","/bin/bash","-c",cmd,NULL);
		// Por si exec falla...
		perror ( "error de exec" ) ;
		exit(-1);
	}
	else { /* proceso padre */
	while ( child_pid != wait (& stat ) ) ;
	}
	return 0;
	
}


int main(int argc, char* argv[])
{
	if (argc!=2){
		fprintf(stderr, "Usage: %s <command>\n", argv[0]);
		exit(1);
	}
	my_system(argv[1]);
	return 0;
}

