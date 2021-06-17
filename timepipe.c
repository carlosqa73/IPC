#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>

#define MSGSIZE 16

int main(int argc, char* argv[]){

	pid_t pid;
	char inbuf[MSGSIZE];
	int p[2], nbytes;

	if(pipe(p) < 0)
		exit(1);

	pid = fork();

	if(pid == 0){ //Proceso hijo

		struct timeval start;

		char** comando = malloc(12 * sizeof(char*));
		for(int i = 1; i < argc; i++){
			comando[i-1] = argv[i];
		}

		gettimeofday(&start, NULL);

		//Cast de long int a char*
		long int a = start.tv_usec;
		char b[100];
		sprintf(b," %ld", a);

		//se escribe en el pipe
		write(p[1], b, MSGSIZE);
		close(p[1]);

		//Se ejecuta el execvp
		execvp(comando[0], comando);
		perror("Error en el exec. \n");

		exit(0);

	}

	else{ //proceso padre

		wait(NULL);

		struct timeval end;
		gettimeofday(&end, NULL);

		//Leo el valor en el pipe
		close(p[1]);
		while((nbytes = read(p[0], inbuf, MSGSIZE)) > 0)
			printf("%s\n", inbuf);

		if(nbytes != 0)
			exit(2);

		char* b;
		long int res;
		res = strtol(inbuf, &b, 10);

		long int result = end.tv_usec - res;
		float rf = (float) result;
		float results = rf/1000000;

		printf("Tiempo de inicio: %ld\n", res);
		printf("Tiempo de fin: %ld\n", end.tv_usec);
		printf("Tiempo total de ejecucion: %ld microsegundos\n", result);
		printf("Tiempo total de ejecucion: %.6f segundos\n", results);

		exit(0);

	}

}
