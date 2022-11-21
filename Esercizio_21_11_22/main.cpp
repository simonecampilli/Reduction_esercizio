#include <stdio.h>

#include <pthread.h>


const int N_ELEM = 16;
int arr[N_ELEM];
int array[4];
int N=16;
void * pthreads_fn(void * args)
{
    int myid = (int) args; // This is my uniqe identifier, as assigned by programmer in main()

    printf("Hello PThreads world! My id is %u\n", myid);
    int istart, iend;
    int sum_local=0;
    istart = N_ELEM /4 * myid ;
    iend = istart + 4  ;

    for (int i=istart; i< iend ; i++) {
        sum_local = sum_local + arr[i] ;
    }
    array[myid]=sum_local;




    return NULL; // We work on a global object. No need to return anything, here
}

int main()
{
    // We spawn as many threads as the elements of the array:
    // this means that our data partitioning strategy assigns one array element to each thread
     int NUM_THREADS;
    NUM_THREADS = 4;
    pthread_t mythreads[NUM_THREADS];
    pthread_attr_t myattr;
    void *returnvalue;

    // Init arr
    for(int i=0; i<N_ELEM; i++)
        arr[i]= i;

    for(int i=1; i<NUM_THREADS; i++) // ==> FORK
    {
        pthread_attr_init(&myattr);
        int err = pthread_create (&mythreads[i], &myattr, pthreads_fn, (void *) i); // Pass 'i' as identifier for thread
        pthread_attr_destroy(&myattr);
    }

    // We want the master thread to do the very same work of other threads
    pthreads_fn((void *) 0);
    int sum=0;
    for(int i=1; i<NUM_THREADS; i++) // <== JOIN
        pthread_join(mythreads[i], &returnvalue);
    for( int i=0; i<NUM_THREADS; i++) {
        sum += array[i]  ;
        printf("%i\n",array[i]);
    }
    // Check (after join!!)
    printf("[CHECK] average value is %f\n",( (float) sum)/N_ELEM);


    return 0;
}