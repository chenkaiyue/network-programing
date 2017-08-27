#include <cstdio>
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <pthread.h>  
#include <semaphore.h>  
  
#define BUFFSIZE 9 // ����ش�С   
  
struct queue_type     //����ض���   
{  
    int buff[BUFFSIZE];  
    int front;  //��ͷ�������ߴӶ�ͷȡ��"��Ʒ��   
    int rear;   //��β�������ߴӶ�β����"��Ʒ��   
}Q={{0},0,0};         
  
sem_t empty; // ������˽���ź�������ʾ�ջ�������Ŀ ��   
sem_t full;   //������˽�б�������ʾ�в�Ʒ�Ļ�������Ŀ��   
pthread_mutex_t mutex; // �����ź���������ʵ�ֶ��ٽ����������   
  
int producer_id = 0;   //�����߱�ţ���ֵΪ0   
int consumer_id = 0;   //�����߱�ţ���ֵΪ0   
  
/* ��ӡ��������*/  
void print()  
{  
     int i;  
     for(i = 0; i < BUFFSIZE; i++)  
     printf("%d ", Q.buff[i]);  
     printf("\n");  
}  
  
/*������*/  
void *producer()  
{  
     int id=++producer_id;  
  
     while(1)  
      {  
             sleep(1);  //        
    
             sem_wait(&empty);  //����ջ�����   
             pthread_mutex_lock(&mutex);    //������л���   
  
             Q.buff[Q.rear] = 1;   //����Ʒ����rear��ָ��Ļ�����   
             printf("producer number<%d> thread idetifier:%u put into buffer[%d].The buffer is like: \t", id, ((unsigned int)pthread_self()),Q.rear+1);  
             print();    
             Q.rear = (Q.rear+1) % BUFFSIZE;  
    
             pthread_mutex_unlock(&mutex);  //�ͷŶ��л���   
             sem_post(&full);   //�ͷ���������   
      }  
}  
  
/* ������*/  
void *consumer()  
{  
     int id=++consumer_id;  
     while(1)  
      {  
             sleep(1);  
  
             sem_wait(&full);  //������������   
             pthread_mutex_lock(&mutex);     //������л���   
     
             Q.buff[Q.front] = 0;            //��front��ָ��Ļ�����ȡ��Ʒ   
             printf("consumer number<%d> thread idetifier:%u get from buffer[%d].The buffer is like: \t", id,((unsigned int)pthread_self()), Q.front+1);  
             print();  
             Q.front = (Q.front+1) % BUFFSIZE;  
    
             pthread_mutex_unlock(&mutex);    //�ͷŶ��л���   
             sem_post(&empty);                //�ͷſջ�����   
      }  
}  
  
int main()  
{  
    int M,N;  //MΪ����������Ŀ��NΪ��������Ŀ   
    printf("please input the producers number: ");  
    scanf("%d",&M);  
    printf("please input the consumers number: ");  
    scanf("%d",&N); 
    pthread_t id1[M];       //�洢�������߳�ID   
    pthread_t id2[N];       //�洢���������߳�ID   
    int i;  
    int ret1[M],ret2[N];  //�洢���������ߺ��������̵߳ķ���ֵ���ɹ��Ļ�����0  
  
  
/*��ʼ��empty��full˽���ź��� */  
    int ini1 = sem_init(&empty, 0, BUFFSIZE);//��ʼ��empty�ź�����ֵ��СΪBuffersize ����ʼ���ɹ������㣬ʧ�ܷ���-1    
    int ini2 = sem_init(&full, 0, 0);        //��ʼ��full�ź�������СΪ0                                   
    if((ini1 || ini2)!=0)   //  
    {  
             printf("sem init failed \n");  
             exit(1);  
    }  
  
/*��ʼ�������ź��� mutex*/  
    int ini3 = pthread_mutex_init(&mutex, NULL);    //��Ĭ�Ϸ�ʽ���������������ٻ������� ����ʼ��Ϊδ��״̬���ɹ�����0   
    if(ini3 != 0)  
    {  
            printf("mutex init failed \n");  
            exit(1);  
    }  
      
/*�����������߳�*/  
    for(i = 0; i < M; i++)  
    {  
            ret1[i] = pthread_create(&id1[i], NULL, producer, (void *)(&i));    //ѭ�������������̣߳��ɹ�����0   
            if(ret1[i] != 0)  
            {  
               printf("producer%d creation failed \n", i);  
               exit(1);  
            }  
    }  
      
/*�����������߳�*/   
    for(i = 0; i < N; i++)  
    {  
            ret2[i] = pthread_create(&id2[i], NULL, consumer, NULL);         //ѭ�������������̣߳��ɹ�����0   
            if(ret2[i] != 0)  
            {  
               printf("consumer%d creation failed \n", i);  
               exit(1);  
            }  
    }  
      
/*�����߳�*/  
    for(i = 0; i < M; i++)  
    {  
            pthread_join(id1[i],NULL);   //�Դ������������߳̽�����Դ����   
    }  
    for(i = 0; i < N; i++)  
    {   
            pthread_join(id2[i],NULL);   //�Դ������������߳̽�����Դ����   
    }  
  
    exit(0);  
}  