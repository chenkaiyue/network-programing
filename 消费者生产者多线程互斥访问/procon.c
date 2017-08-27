#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  
#include <pthread.h>  
#include <semaphore.h>  

#define BUFFIZE 9
struct queue_type
{
	int buff[BUFFIZE];
 	int front;
 	int rear;
}Q={{0},0,0};


sem_t empty;
sem_t full;

pthread_mutex_t mutex;

int producer_id = 0;
int consumer_id = 0;


/* 打印缓冲池情况*/ 
void print(){
	int i;
	for(i=0;i<BUFFIZE;i++){
		print("%d",Q.buff[i]);
	}
	print("\n");
}
/*生产者*/  
void *producer(){
	int id = ++producer_id;
	while(1){
		sleep(1);

		sem_wait(&empty);
		pthread_mutex_lock(&mutex);

		Q.buff[Q.rear] = 1;
		print("producer number <%d> thread identifier:%u put into buffer[%d].The buffer is like \t",id,((unsigned int)pthread_self()),Q.rear);
		print();
		Q.rear = (Q.rear+1)%BUFFIZE;
		
		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

/* 消费者*/  
void *consumer(){
	int id = ++consumer_id;
	while(1){
		sleep(1);

		sem_wait(&full);
		pthread_mutex_lock(&mutex);

		Q.buff[Q.front] =0 ;
		print("producer number <%d> thread identifier:%u put into buffer[%d].The buffer is like \t",id,((unsigned int)pthread_self()),Q.front);
		print();
		Q.front = (Q.front+1)%BUFFIZE;

		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}


/*主函数*/
/*
消费者生产者数目
存储线程id
存储线程返回值
/*初始化empty和full私有信号量 */  
/*初始化公有信号量 mutex*/  
/*创建生产者线程*/
/*创建消费者线程*/ 
/*销毁线程*/  


int main(){
	int M,N;
	printf("input the producer numbers");
	scanf("%d",&M);
	printf("input the consumer numbers");
	scanf("%d",&N);

	pthread_t id1[M]; //线程标识符，线程id
	pthread_t id2[N];

	int ret1[M],ret2[N];
	// int ret;

	int ini1 = sem_init(&empty, 0, BUFFIZE);
	int ini2 = sem_init(&full,0,0);
	if ((ini1 || ini2) != 0){
		printf("sem init failed\n");
		exit(1);
	}

	int ini3 = pthread_mutex_init(&mutex, NULL);
	if (ini3 != 0){
		printf("mutex init failed\n");
		exit(1);
	}
	
	int i;
	for (i=0;i<M;i++){
		ret1[i] = pthread_create(&id1[i], NULL, producer, NULL);
		if (ret1[i] != 0){
			printf("producer%d creation failed \n",i);
			exit(1);
		}
	}

	for (i=0;i<N;i++){
		ret2[i] = pthread_create(&id2[i],NULL,consumer,NULL);
		if (ret2[i] != 0){
			printf("consumer%d creation failed \n",i);
			exit(1);
		}
	}

	for (i=0;i<M;i++){
		pthread_join(id1[i],NULL);
	}

	for (i=0;i<N;i++){
		pthread_join(id2[i],NULL);
	}

	exit(0);
}