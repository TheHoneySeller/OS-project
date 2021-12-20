#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_OF_PHILS 5

typedef struct thread_arg {
	pthread_mutex_t *lock;
    int i;

} thread_arg;

enum {THINKING, HUNGRY, EATING} state[NUM_OF_PHILS];

pthread_mutex_t *locks[NUM_OF_PHILS];
pthread_t tids[NUM_OF_PHILS];
thread_arg *thread_args[NUM_OF_PHILS];

void test(int i, pthread_mutex_t *lock) { //taken from the solution in the textbook pg. 296 modified for mutexes
    if ((state[(i + 4) % 5] != EATING) &&
        (state[i] == HUNGRY) &&
        (state[(i + 1) % 5] != EATING))
    {
        state[i] = EATING;
        pthread_mutex_unlock(lock);
    }
}

static void *run(void *args_ptr) { //some parts taken from the solution in the textbook pg. 296 modified for mutexes
    thread_arg *args = (thread_arg *) args_ptr;
    int i = args->i;
    pthread_mutex_t *lock = args->lock;

    while (1){
        //pickup
        state[i] = HUNGRY;
        test(i, lock);
        if (state[i] != EATING)
        {
            pthread_mutex_lock(lock);
        }

        //eat
        printf("philosopher %d is eating.\n", i);

        //putdown
        state[i] = THINKING;
        test((i + 4) % 5, lock);
        test((i + 1) % 5, lock);
    }

    pthread_exit(NULL);
}

int main()
{
    for (size_t i = 0; i < NUM_OF_PHILS; i++)
    {
        state[i] = THINKING;
        
        if (pthread_mutex_init(locks[i], NULL) != 0) {
            printf("failed to init mutex\n");
            exit(1);
        }
    }
    
    for (int i = 0; i < NUM_OF_PHILS; i++) {

        thread_args[i]->i = i;
        thread_args[i]->lock = locks[i];

        int r = pthread_create(&(tids[i]), NULL, &run, (void *) &(thread_args[i]));

		if (r != 0) {
			printf("failed to create thread %d \n", i);
			exit(1);
		}
    }
 
    return 0;
}