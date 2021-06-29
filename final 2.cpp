#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#define MAX 5

int buffer[MAX];
int count=0;
int in=-1, out=-1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill = PTHREAD_COND_INITIALIZER;

void put(int value) {
	assert(count!=MAX);
	in++;
	in%=MAX;
	buffer[in]=value;
    count++;
}

int get() {
	assert(count!=0);
    out++;
	out%=MAX;
	int tmp = buffer[out];
	count--;
    return tmp;
}

void *producer(void *arg) {
	int i;
	int loops = (int)arg;
	for(i=0; i<loops; i++) {
		pthread_mutex_lock(&mutex);
        while(count == MAX) {
			pthread_cond_wait(&empty, &mutex);
		}
		printf("put\n");
        put(i);
		pthread_cond_signal(&fill);
        pthread_mutex_unlock(&mutex);
	}
}

void *consumer(void *arg) {
	int i;
	int loops = (int) arg;
	for(i=0; i<loops; i++) {
		pthread_mutex_lock(&mutex);
		while(count==0) {
			pthread_cond_wait(&fill, &mutex);
		}
		int tmp=get();
		pthread_cond_signal(&empty);
		pthread_mutex_unlock(&mutex);
		printf("%d\n", tmp);
	}
}

int main(int argc, char *argv[]) {
	pthread_t p, c1, c2;
	printf("[main begin] \n");
	pthread_create(&p, NULL, producer, 10);
	pthread_create(&c1, NULL, consumer, 5);
	pthread_create(&c2, NULL, consumer, 5);
	
	pthread_join(p, NULL);
	pthread_join(c1, NULL);
	pthread_join(c2, NULL);
	
	printf("[main end] \n");

	return 0;
}
