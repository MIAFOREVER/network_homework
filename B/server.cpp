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
int main() 
{ 
    fstream fout("test2",ios::binary|ios::out);
    int listenfd,connfd; 
    struct sockaddr_in servaddr; 
    char buff[MAXLINE]; 
    int n; 
    if( (listenfd = socket(AF_INET,SOCK_STREAM,0)) == -1) 
    { 
        printf(" create socket error: %s (errno :%d)\n",strerror(errno),errno); 
        return 0; 
    } 
    memset(&servaddr,0,sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(6666); 
    if ( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) 
    { 
        printf(" bind socket error: %s (errno :%d)\n",
        strerror(errno),errno); 
        return 0; 
    } 
    if( listen(listenfd,10) == -1) 
    { 
        printf(" listen socket error: %s (errno :%d)\n",strerror(errno),errno); 
        return 0; 
    } 
    printf("====waiting for client's request=======\n"); 
    if( (connfd = accept(listenfd, (struct sockaddr *)NULL, NULL)) == -1) 
    { 
        printf(" accpt socket error: %s (errno :%d)\n",strerror(errno),errno); 
        return 0; 
    }
    while(1) 
    {  
        n = recv(connfd,buff,MAXLINE,0); 
        fout.write(buff,n);
        fout.sync();
    } 
    close(listenfd); 
    return 0; 
}
