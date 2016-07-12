#include "cs537.h"
#include "request.h"
#include <stdlib.h>
#include <pthread.h>

// 
// server.c: A very, very simple web server
//
// To run:
//  server <portnum (above 2000)>
//
// Repeatedly handles HTTP requests sent to this port number.
// Most of the work is done within routines written in request.c

// Schedule the waiting request in the bufferpool in terms of FIFO (i.e. 
// First In First Out)

struct buf{
    int connfd;
    int fill;
};

struct buf *bufferpool;
pthread_t *workers;

// number of threads needed to be created
int threads;
// number of request connections that can be accepted at one time
int buffers;

pthread_mutex_t mutex;
pthread_cond_t empty, fill;
int count = 0;

// position from which start to search available slot in buffer pool to
// produce content
int pi = 0;
// position from which start to search available slot in buffer pool to
// consume content
int ci = 0;

// Parse string into integer
int stoi(char *arg)
{
    char *pEnd;
    // implicitly cast long int to int
    int rv = strtol(arg, &pEnd, 10);
    if(errno == ERANGE || *pEnd != '\0') 
        return 0;
    return rv;
}

void getargs(int *port, int *threads, int *buffers, int argc, char *argv[])
{
    if (argc != 4) {
	fprintf(stderr, "Usage: %s [portnum] [threads] [buffers]\n", argv[0]);
	exit(1);
    }
    if( (*port = stoi(argv[1])) < 1) {
        fprintf(stderr, "Bad port num\n");
        exit(1);
    }
    if( (*threads = stoi(argv[2])) < 1) {
        fprintf(stderr, "Bad threads num\n");
        exit(1);
    }
    if( (*buffers = stoi(argv[3])) < 1) {
        fprintf(stderr, "Bad buffers num\n");
        exit(1);
    }
}

void produce(int connfd) {
    while(bufferpool[pi].fill == 1) 
        pi = (pi + 1) % buffers;
    bufferpool[pi].connfd = connfd;
    bufferpool[pi].fill = 1;
    count++;
    pi = (pi + 1) % buffers;
}

void producer(int connfd) {
    pthread_mutex_lock(&mutex);
    while(count == buffers)
        pthread_cond_wait(&empty, &mutex);
    produce(connfd);
    pthread_cond_signal(&fill);
    pthread_mutex_unlock(&mutex);
}

int consume(void) {
    int connfd; 
    while(bufferpool[ci].fill == 0)
        ci = (ci + 1) % buffers;
    connfd = bufferpool[ci].connfd;
    bufferpool[ci].fill = 0;
    count--;
    ci = (ci + 1) % buffers;
    return connfd;
}

void *consumer() {
    int connfd;
    while(1) {
        pthread_mutex_lock(&mutex);
        while(count == 0)
            pthread_cond_wait(&fill, &mutex);
        connfd = consume();
        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
        requestHandle(connfd);
	Close(connfd);
    }
}

int main(int argc, char *argv[])
{
    int listenfd, connfd, port, clientlen;
    struct sockaddr_in clientaddr;

    getargs(&port, &threads, &buffers, argc, argv);

    // CS537: Create some threads...
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&empty, NULL);
    pthread_cond_init(&fill, NULL);

    int i;
    workers = (pthread_t *)malloc(sizeof(pthread_t)*threads);
    for(i = 0; i < threads; i++)
        pthread_create(&workers[i], NULL, consumer, NULL);

    bufferpool = (struct buf *)malloc(sizeof(struct buf *)*buffers);
    for(i = 0; i < buffers; i++)
        bufferpool[i].fill = 0;

    listenfd = Open_listenfd(port);
    while (1) {
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, (socklen_t *) &clientlen);

        producer(connfd);
    }

}


    


 
