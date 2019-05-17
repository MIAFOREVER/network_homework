#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <fstream>
#include <sys/time.h>
using namespace std;
#define MAXLINE 4096 
int listenfd[10],connfd[10]; 
struct sockaddr_in servaddr[10]; 
char buff[MAXLINE]; 
int n[10]; 
int serverInit()
{
    for(int i=0;i<10;i++)
    {
        if( (listenfd[i] = socket(AF_INET,SOCK_STREAM,0)) == -1) 
        { 
            printf(" create socket error: %s (errno :%d)\n",strerror(errno),errno); 
            return 0; 
        } 
        memset(&servaddr[i],0,sizeof(servaddr[i])); 
        servaddr[i].sin_family = AF_INET; 
        servaddr[i].sin_addr.s_addr = htonl(INADDR_ANY); 
        servaddr[i].sin_port = htons(6666+i); 
        if ( bind(listenfd[i], (struct sockaddr*)&servaddr[i], sizeof(servaddr[i])) == -1) 
        { 
            printf(" bind socket error: %s (errno :%d)\n",
            strerror(errno),errno); 
            return 0; 
        } 
        if( listen(listenfd[i],10) == -1) 
        { 
            printf(" listen socket error: %s (errno :%d)\n",strerror(errno),errno); 
            return 0; 
        } 
    }
    for(int i=0;i<10;i++)
    {
        printf("====waiting for client's request=======\n"); 
        if( (connfd[i] = accept(listenfd[i], (struct sockaddr *)NULL, NULL)) == -1) 
        { 
            printf(" accpt socket error: %s (errno :%d)\n",strerror(errno),errno); 
            return 0; 
        }
    }
}
int main() 
{ 
    fstream fout("test2",ios::binary|ios::out);
    serverInit();
    while(1) 
    {  
        for(int i=0;i<10;i++)
        {
            n[i] = recv(connfd[i],buff,MAXLINE,0); 
            fout.write(buff,n[i]);
            fout.sync();
        }
    } 
    for(int i=0;i<10;i++)
        close(listenfd[i]); 
    return 0; 
}
