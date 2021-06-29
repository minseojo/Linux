#include <stdio.h>
#include <assert.h>
#include <pthread.h>

int done=0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void thr_exit() {
	pthread_mutex_lock(&m);
	done=1;
	pthread_cond_signal(&c);
	pthread_mutex_unlock(&m);
}

int *child(void *arg) {
	int num_of_carnations=0;
	printf("Child: guess what? A present for you, mom and dad! \n");

	thr_exit();
	
	for(int i=0; i<10000; i++) {
		num_of_carnations++;
	}
	
	return num_of_carnations;
}

void thr_join() {
	pthread_mutex_lock(&m);
	while(done==0) {
		pthread_cond_wait(&c, &m);
	}
	pthread_mutex_unlock(&m);
}

int main(int argc, char *argv[]) {
	pthread_t c;
	int num_of_carnations=0;
	printf("[main begin] \n");
	
	pthread_create(&c, NULL, child, NULL);
	thr_join();
	printf("Parents: thank you, sweetheart. \n");
	
	pthread_join(c, &num_of_carnations);
	printf("num of carnations: %d \n", num_of_carnations);
	
	printf("[main end] \n");
	
	return 0;
}
