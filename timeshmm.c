#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){

	pid_t pid;
	pid = fork();

	if(pid == 0){   //Proceso hijo

		const int SIZE = 4096;
		const char* name = "SHM";
		struct timeval start;

		int shm_fd; //descriptor de la memoria compartida
		void* ptr; //puntero a la memoria compartida

		shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
		ftruncate(shm_fd, SIZE);
		ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

		char** comando = malloc(12 * sizeof(char*));
		for(int i = 1; i < argc; i++){
			comando[i-1] = argv[i];
			//printf(comando[i-1]);
			//printf("\n");
		}

		gettimeofday(&start, NULL);
		sprintf(ptr, "%d", start.tv_sec);

		execvp(comando[0], comando);
		perror("Error en el exec.\n");



		exit(0);

	}

	else{	//Proceso padre

		const int SIZE = 4096;
		const char* name = "SHM";
		struct timeval end;

		int shm_fd;
		void* ptr;

		shm_fd = shm_open(name, O_RDONLY, 0666);
		ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

		wait(NULL);

		gettimeofday(&end, NULL);

		printf("%d\n", ptr);
		printf("%d\n", end.tv_sec);

		exit(0);

	}

	return 0;
}
