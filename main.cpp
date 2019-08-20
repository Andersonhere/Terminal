#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "libar8020.h"
typedef void * PORT;
pthread_mutex_t lock;
pthread_cond_t is_read;
pthread_cond_t is_send;
int flag=0;

int  getString(char *cbuff)
{
  printf(">>>");
  gets((char *)cbuff);
  if(cbuff[0]=='0')
  return -1;
  return 0;
}

void *sendCmd(void *cbuff)
{

  while(1)
  {
    pthread_mutex_lock(&lock);
    if(!getString((char *)cbuff))
    pthread_cond_signal(&is_read);
    else
    {
      exit(0);
    }
    pthread_cond_wait(&is_send,&lock);
      flag=1;
      pthread_mutex_unlock(&lock);

  }

}

void *recvCmd(void * cbuff)
{
    while(1)
    {
        pthread_mutex_lock(&lock);
        if(strlen((char *)cbuff)==0)
        pthread_cond_wait(&is_read,&lock);
        
        puts((char *)cbuff);
        memset(cbuff,0,sizeof(1024));
        pthread_cond_signal(&is_send);
        pthread_mutex_unlock(&lock);
    }
}

int main()
{
  printf(">>>:input 0 for quit!\n");
  char *sendBuff;
  char *recvBuff;
  sendBuff = (char *)malloc(1024);
  recvBuff = (char *)malloc(1024);
  void *retval,*retvalRecv;

  pthread_t send_thread,recv_thread;

  pthread_mutex_init(&lock,NULL);
  pthread_cond_init(&is_send,NULL);
  pthread_cond_init(&is_read,NULL);

  int reSend = pthread_create(&send_thread,NULL,sendCmd,(void *)sendBuff);
  int reRecv = pthread_create(&recv_thread,NULL,recvCmd,(void *)sendBuff);

  int sendExit =  pthread_join(send_thread, &retval);
  int recvExit =  pthread_join(recv_thread, &retvalRecv);

  free(sendBuff);
  free(recvBuff);
  return 0;
}
