#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h> 

void destructor(void* data){
    fprintf(stderr,"Thread canceled. Max value is %d\n",*((int*)data));
}

void* asyncThreadFunc(void* argument){

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	int countForDestructor = 0;
	while(1){
		pthread_cleanup_push(&destructor,&countForDestructor);
		printf("Iteration: %d\n", countForDestructor);
		countForDestructor++;
		sleep(1);
		pthread_cleanup_pop(0);
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
	pthread_t asyncThread;
	pthread_attr_t attr;
    if(pthread_attr_init(&attr)!=0){
        fprintf(stdout,"Can't initialize thread attributes!\n");
        return 0;
    }
    if(pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE)!=0){
        fprintf(stdout,"Can't set joinable attributes!\n");
        return 0;   
    }


	// pthread_create(&asyncThread, NULL, &asyncThreadFunc, NULL);
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
		printf("Undifed Thread exit\n");
	
	return EXIT_SUCCESS;
}
