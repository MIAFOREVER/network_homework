#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <errno.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <fstream>
#include <sys/time.h>
using namespace std;
#define MAXLINE 4096 
int main(int argc, char**argv) 
{ 
    int sockfd,n; 
    char recvline[MAXLINE],sendline[MAXLINE]; 
    struct sockaddr_in servaddr; 
    if(argc !=2) 
    { 
        printf("usage: ./client <ipaddress>\n"); 
        return 0; 
    } //创建socket 
    if( (sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1) 
    { 
        printf(" create socket error: %s (errno :%d)\n",strerror(errno),errno); 
        return 0; 
    } 
    memset(&servaddr,0,sizeof(servaddr)); servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(6666); 
    if( inet_pton(AF_INET,argv[1], &servaddr.sin_addr) <=0 ) 
    { 
        printf("inet_pton error for %s\n",argv[1]); 
        return 0; 
    } 
    if( connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) <0) 
    { 
        printf(" connect socket error: %s(errno :%d)\n",strerror(errno),errno); 
        return 0; 
    } 
    printf("send msg to server:\n"); 
    fstream fin("test",ios::binary|ios::in);
    struct timeval begin_time,end_time;
    gettimeofday(&begin_time,NULL);
    while(!fin.eof())
    {
        fin.read(sendline,MAXLINE);
        int length = fin.gcount();
        if ( send(sockfd,sendline,length,0) <0) 
        { 
            printf("send msg error: %s(errno :%d)\n",strerror(errno),errno); 
            return 0; 
        } 
    }
    close(sockfd); 
    gettimeofday(&end_time,NULL);
    printf("%fkb/s\n",87999144/1000.0/(end_time.tv_sec-begin_time.tv_sec+(end_time.tv_usec-begin_time.tv_usec)/1000000.0));
    return 0; 
} 
