#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h> 



void* asyncThreadFunc(void* argument){

	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	int i = 0;
	while(1){
		printf("Iteration: %d\n", i);
		i++;
		sleep(1);
	}
}



int main(int argc, char* argv[]) {
	int delay = 1;

	if (argc == 2)
	{	
		delay = atoi(argv[1]);
	}
	else{
		fprintf(stderr, "Error\n");
		return EXIT_FAILURE;
	}



	setbuf(stdout, NULL);
	pthread_t thread, asyncThread;

	pthread_create(&asyncThread, NULL, &asyncThreadFunc, NULL);
	if (pthread_create(&asyncThread, NULL, &asyncThreadFunc, NULL)!= 0) {
			fprintf(stderr, "Error pthread_create()\n");
			exit(EXIT_FAILURE);
	}
	
	sleep(delay);
	
	if (pthread_cancel(asyncThread) != 0) {
			fprintf(stderr, "Cancel error\n");
		}
	void* result;
	int codeExit = 10;
	if (pthread_join(asyncThread, &result) != 0) {
			fprintf(stderr, "Join error\n");
			exit(EXIT_FAILURE);
		}
	if (result)
	{
		codeExit = (int)result;
	}

	printf("Code EXIT = %d\n",result);
	if (result == PTHREAD_CANCELED)
		printf("Thread was canceled\n");

	else if(codeExit == 0)
		printf("Normal Thread exit \n");
	else
		printf("Undefined Thread exit\n");
	
	return EXIT_SUCCESS;
}
