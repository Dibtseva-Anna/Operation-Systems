#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h> 

int* globalArr;

void* threadFunc(void* argument)
{	
	int numberThread = *(int*) argument;
	globalArr[numberThread] = rand() % 10 + 1;
	sleep(globalArr[numberThread]);
}

void* createThreads(pthread_t pthreads[], int countThreads){

	for (int i = 0; i < countThreads+1; ++i)
	{	
		if (pthread_create(&pthreads[i], NULL, &threadFunc, &i)!= 0) {
			fprintf(stderr, "Error pthread_create()\n");
			exit(EXIT_FAILURE);
		}
		if (pthread_join(pthreads[i], NULL) != 0) {
			fprintf(stderr, "Join error\n");
			exit(EXIT_FAILURE);
		}
	}
	return NULL;
}

void* detachedThreadFunc(void* argument){

	bool state = false;
	int countArr = *(int*) argument;
	while(state != true)
	{
		for (int i = 0; i < countArr+1; ++i)
		{
			printf("GlobalArray[%d] = %d\n",i, globalArr[i]);

			if (globalArr[countArr] != -1)
			{
				state = true;
			}
		}
		printf("------------------------------\n");
		sleep(1);
	}
	printf("End detached Thread\n");
	return NULL;
}



int main(int argc, char* argv[]) {
	int countThreads = 1;

	if (argc == 2)
	{	
		countThreads = atoi(argv[1]);
	}
	else{
		fprintf(stderr, "Error\n");
		return EXIT_FAILURE;
	}



	setbuf(stdout, NULL);
	//create array thread
	pthread_t thread[countThreads];
	int* arr = (int*) malloc(sizeof(int)*countThreads);
	globalArr = (int*) malloc(sizeof(int) * countThreads);
	globalArr[countThreads] = -1;//для выхода из функции
	//create detached thread
	pthread_attr_t attr;
	pthread_t detachedThread;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&detachedThread, &attr, &detachedThreadFunc,&countThreads);
	pthread_attr_destroy (&attr);
	//create threads
	createThreads(thread, countThreads);

	
	return EXIT_SUCCESS;
}
