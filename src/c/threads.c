#include <stdio.h>
#include <pthread.h>

#define BSIZE 5

typedef struct buffer_t {
    char buf[BSIZE];
    pthread_mutex_t lock;
    pthread_cond_t less;
    pthread_cond_t more;
    int nextin;
	int nextout;
	int occupied;
} buffer_t;

buffer_t buffer;

char consume(buffer_t b){
	char item;
	pthread_mutex_lock(&b.lock);
	printf("Consume\n");
	while (b.occupied == 0)
		pthread_cond_wait(&b.more, &b.lock);
	item = b.buf[b.nextout++];
	b.nextout %= BSIZE;
	b.occupied--;
	pthread_cond_signal(&b.less);
	pthread_mutex_unlock(&b.lock);
	return (item);
}

void consumer(buffer_t b){
	char item;
	printf("Consumidor - %d\n", getpid());
	while (1) {
		item = consume(b);
		if (item == '\0') break;
		putchar(item);
	} 
}

char produce(buffer_t b, char item){
	pthread_mutex_lock(&b.lock);
	printf("Produce\n");
	while (b.occupied == BSIZE)
		pthread_cond_wait(&b.less, &b.lock);
	b.buf[b.nextin++] = item;
	b.nextin %= BSIZE;
	b.occupied++;
	pthread_cond_signal(&b.more);
	pthread_mutex_unlock(&b.lock);
}

void producer(buffer_t b) {
	int item;
	printf("Productor\n");
	while (1) {
		item = getchar();
		if (item == EOF) {
			produce(b, '\0');
			break;
		}
		else
			produce(b, (char) item);
	}
}

void prodprod (void *ptr){
    producer(buffer);
}

void conscons (void *ptr){
    consumer(buffer);
}

int main(){
	
	pthread_mutexattr_t mutex_attr;
	pthread_condattr_t cond_attr;
	buffer.occupied = buffer.nextin = buffer.nextout = 0;
	
	pthread_mutexattr_init(&mutex_attr);
	pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&buffer.lock, &mutex_attr);
	
	pthread_condattr_init(&cond_attr);
	pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
	pthread_cond_init(&buffer.less, &cond_attr);
	pthread_cond_init(&buffer.more, &cond_attr);
	
	
	pthread_t cons, prod;
	int idCons = 0, idProd = 1;
	
	pthread_create(&cons, NULL, (void *) conscons, (void *) &idCons);
	pthread_create(&prod, NULL, (void *) prodprod, (void *) &idProd);
	
	pthread_join(cons, NULL);
	pthread_join(prod, NULL);
	
	return 0;
}



