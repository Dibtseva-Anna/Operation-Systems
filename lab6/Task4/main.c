#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>



void* syncThreadFunc(void* argument){
	double pi = 1;
	double part = 3;
	int oldCancelState;
	if (pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,NULL) != 0) {
			fprintf(stderr, "Can`t set cancel type\n");
	}
	int i = 0;
	for (int i = 0; i < 100000; ++i)
	{	
		if (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldCancelState) != 0) {
			fprintf(stderr, "Cancel error before eval\n");
		}	
		if (i%2 == 0)
			pi -= 1/part;
		else
			pi += 1/part;
		part += 2;
		if (pthread_setcancelstate(oldCancelState,NULL) != 0) {
			fprintf(stderr, "Cancel error after eval\n");
		}	
		pthread_testcancel();

		printf("curent pi = %f\n",4*pi);
		sleep(1);
	}
	printf("End detached Thread\n");

	return (void*) &pi;

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
	pthread_t syncThread;

	
	if (pthread_create(&syncThread, NULL, &syncThreadFunc, NULL)!= 0) {
			fprintf(stderr, "Error pthread_create()\n");
			exit(EXIT_FAILURE);
	}
	
	sleep(delay);
	
	if (pthread_cancel(syncThread) != 0) {
			fprintf(stderr, "Cancel error in main\n");
	}

	void* result;
	if (!pthread_equal(pthread_self(),syncThread))
	{
		if (pthread_join(syncThread, &result) != 0) {
			fprintf(stderr, "Join error\n");
		}
	}
	if (!result)
	{
		fprintf(stderr, "Can`t set result\n" );
		return EXIT_FAILURE;
	}
	


	printf("Code EXIT = %d\n",result);
	if (result == PTHREAD_CANCELED)
		printf("Thread was canceled\n");
	else
		printf("full pi = %f\n", *(double*)result * 4);

	return EXIT_SUCCESS;
}
