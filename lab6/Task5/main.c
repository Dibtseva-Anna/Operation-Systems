#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

static pthread_key_t key;

pthread_once_t onceVar = PTHREAD_ONCE_INIT;


void destructor(void* str)
{
	free(str);
}

void pthreadOnce(void)
{
	pthread_key_create(&key,destructor);
}



void* threadFunc(void* argument) {

	if (pthread_once(&onceVar, &pthreadOnce) != 0)
	{
		fprintf(stderr, "Error pthread_once()\n" );
	}

	int countForLoop = rand() % 10 +1;
	char* getSpec = (char*)pthread_getspecific(key);
	if (pthread_getspecific(key) == NULL)
	{	//add special formating string.She looks like this:
		//This string is i for Thread:pid, some text, random number:number
		char* str = ", some text, random number:";
		pthread_t TID = pthread_self();

		char* specStr = (char*)malloc(sizeof str + sizeof TID + sizeof "Thread:%d"+100);//перевыделение памяти производиться специально.
		sprintf(specStr, "Thread:%ld", pthread_self());	
		strcat(specStr,str);
		if (pthread_setspecific(key,specStr) != 0)
		{
			fprintf(stderr, "Error in Thread, pthread_setspecific()\n" );
		}

		getSpec = (char*)pthread_getspecific(key);	
		printf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	}
	
	for (int i = 0; i < countForLoop; ++i)
	{
		printf("This string is %d for %s%d\n",i+1, getSpec, rand() % 10 + 1);
	}
	return (void*) EXIT_SUCCESS;

}



int main(int argc, char* argv[]) {
	int countThreads = 1;

	if (argc == 2)
	{
		countThreads = atoi(argv[1]);
	}
	else {
		fprintf(stderr, "Error\n");
		return EXIT_FAILURE;
	}



	setbuf(stdout, NULL);
	pthread_t thread[countThreads];

	for (int i = 0; i < countThreads; ++i)
	{
		if (pthread_create(&thread[i], NULL, &threadFunc, NULL) != 0) {
			fprintf(stderr, "Error pthread_create()\n");
			exit(EXIT_FAILURE);
		}
	}

	void* result = NULL;
	//wait threads
	for (int i = 0; i < countThreads; ++i)
	{
		if (!pthread_equal(pthread_self(), thread[i]))
		{
			if (pthread_join(thread[i], &result) != 0) {
				fprintf(stderr, "Join error\n");
			}
		}
	}
	//free key
	if(pthread_key_delete(key) != 0){
		fprintf(stderr, "Error pthread_key_delete()\n" );
	}

	printf("Code EXIT = %p\n", result);
	if (result == PTHREAD_CANCELED)
		printf("Threads was canceled\n");
	else
		printf("Threads completed with code %p\n", result);

	return EXIT_SUCCESS;
}
