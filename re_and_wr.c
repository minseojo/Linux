#include <stdio.h>
#include <assert.h>
#include <pthread.h>

int buffer;
int count=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void put(int value) {
	assert(count == 0);
	count=1;
	buffer=value;
}

int get() {
	assert(count==1);
	count=0;
	return buffer;
}

void *producer(void *arg) {
	int i;
	int loops = (int)arg;
	for(i=0; i<loops; i++) {
		printf("put \n");
		put(i);
	}
}

void *consumer(void *arg) {
	int i;
	int loops = (int) arg;
	while(1) {
		int tmp=get();
		printf("%d\n", tmp);
	}
}

int main(int argc, char *argv[]) {
	pthread_t p, c;
	printf("[main begin] \n");
	pthread_create(&p, NULL, producer, 10);
	pthread_create(&c, NULL, consumer, 10);

	pthread_join(p, NULL);
	pthread_join(c, NULL);

	printf("[main end] \n");

	return 0;
}
