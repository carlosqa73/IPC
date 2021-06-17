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
		}

		gettimeofday(&start, NULL);
		//sprintf(ptr, "%ld", start.tv_sec);
		//ptr += strlen((char *)start.tv_sec);

		//Cast de long int a char*
		long int a = start.tv_usec;
		char b[100];
		sprintf(b, "%ld", a);

		//Se escribe en el buffer compartido el tiempo de inicio
		sprintf(ptr, "%s", b);
		ptr += strlen(b);

		//Se ejecuta el execvp
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

		//Se transforma el char* leido a long int para obtener el tiempo de inicio.
		char* a;
		long int res;
		res = strtol(ptr, &a, 10);

		long int result = end.tv_usec - res;
		float rf = (float) result;
		float results = rf / 1000000;

		printf("Tiempo de inicio: %ld\n", res);
		printf("Tiempo de fin: %ld\n", end.tv_usec);
		printf("Tiempo total de ejecucion: %ld microsegundos\n", result);
		printf("Tiempo total de ejecucion: %.6f segundos\n", results);

		exit(0);

	}

	return 0;
}
