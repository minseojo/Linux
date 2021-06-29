#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_PHILOSOPHER 6

typedef struct _chopstick_t {
	int value;
	pthread_mutex_t lock;
	pthread_cond_t cond;
} chopstick_t;

chopstick_t chopstick[NUM_PHILOSOPHER];

void chopstick_init(chopstick_t *c, int value) {
	c->value = value;
	pthread_mutex_init(&c->lock, NULL);
	pthread_cond_init(&c->cond, NULL);
}

void chopstick_wait(chopstick_t *c) {
	pthread_mutex_lock(&c->lock);
	while(c->value <= 0) {
		pthread_cond_wait(&c->cond, &c->lock);
	}
	c->value--;
	pthread_mutex_unlock(&c->lock);
}

void chopstick_post(chopstick_t *c) {
	pthread_mutex_lock(&c->lock);
	c->value++;
	pthread_cond_signal(&c->cond);
	pthread_mutex_unlock(&c->lock);
}

void get_chopstick(int philosopher) {
	printf("philosopher %d waits for the left chopstick. \n", philosopher);
	chopstick_wait(&chopstick[philosopher]);
	printf("philosopher %d waits for the right chopstick. \n", philosopher);
	chopstick_wait(&chopstick[(philosopher+1)%6]);
	printf("philosopher %d gets the chopsticks. \n", philosopher);
}

void put_chopstick(int philosopher) {
	chopstick_post(&chopstick[philosopher]);
	chopstick_post(&chopstick[(philosopher+1)%6]);
}

void *philosopher(int *number) {
	while(1) {
		printf("philosopher %d thinks \n", (int *) number);
		loop(1000000);
		get_chopstick((int *) number);
		printf("philosopher %d eats \n", (int *) number);
		loop(1000000);
		put_chopstick((int *) number);
		loop(1000000);
	}
}

void loop(int count) {
	for(int i=0; i<count; i++);
}

int main(int argc, char *argv[]) {
	for(int i=0; i<NUM_PHILOSOPHER; i++) {
		chopstick_init(&chopstick[i], 1);
	}
	
	pthread_t p[NUM_PHILOSOPHER];
	
	printf("[main begin] \n");
	for(int i=0; i<NUM_PHILOSOPHER-1; i++) {
		pthread_create(&p[i], NULL, philosopher, i);
	}
	
	for(int i=0; i<NUM_PHILOSOPHER-1; i++) {
		pthread_join(p[i], NULL);
	}
	printf("[main end] \n");
	
	return 0;
}
