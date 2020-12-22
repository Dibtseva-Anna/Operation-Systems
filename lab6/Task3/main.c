#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h> 



void* cancelThreadFunc(void* argument){

	int delay = 2 * (*(int*)argument);
	printf("%d\n", delay);
	if (!delay)
	{
		fprintf(stderr, "Error initialization argument Thread\n" );
	}
	if (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL) != 0) {
			fprintf(stderr, "Cancel error\n");
	}
	int i = 0;
	for (int i = 0; i != delay; ++i)
	{
		printf("Iteration: %d\n", i);
		sleep(1);
	}


	return (void*) EXIT_SUCCESS;

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
	pthread_t cancelThread;

	
	if (pthread_create(&cancelThread, NULL, &cancelThreadFunc, &delay)!= 0) {
			fprintf(stderr, "Error pthread_create()\n");
			exit(EXIT_FAILURE);
	}
	
	sleep(delay);
	
	if (pthread_cancel(cancelThread) != 0) {
			fprintf(stderr, "Cancel error\n");
	}
	void* result;
	int codeExit = 10;
	if (pthread_join(cancelThread, &result) != 0) {
			fprintf(stderr, "Join error\n");
		}

	codeExit = (int)result;

	printf("Code EXIT = %d\n",result);
	if (result == PTHREAD_CANCELED)
		printf("Thread was canceled\n");
	else if(codeExit == 0)
		printf("Normal Thread exit \n");
	else
		printf("Undefined Thread exit\n");

	return EXIT_SUCCESS;
}
