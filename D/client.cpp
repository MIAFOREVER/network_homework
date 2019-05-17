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
int sockfd[10],n[10]; 
char sendline[MAXLINE]; 
struct sockaddr_in servaddr[10]; 
const char* ip = "127.0.0.1";
int clientInit()
{
    for(int i=0;i<10;i++)
    {
        if( (sockfd[i] = socket(AF_INET,SOCK_STREAM,0)) == -1) 
        { 
            printf(" create socket error: %s (errno :%d)\n",strerror(errno),errno); 
            return 0; 
        } 
        memset(&servaddr[i],0,sizeof(servaddr[i])); servaddr[i].sin_family = AF_INET; 
        servaddr[i].sin_port = htons(6666+i); 
        if( inet_pton(AF_INET,ip, &servaddr[i].sin_addr) <=0 ) 
        { 
            printf("inet_pton error for %s\n",ip); 
            return 0; 
        } 
        if( connect(sockfd[i],(struct sockaddr*)&servaddr[i],sizeof(servaddr[i])) <0) 
        { 
            printf(" connect socket error: %s(errno :%d)\n",strerror(errno),errno); 
            return 0; 
        } 
    }
}
int main(int argc, char**argv) 
{ 
    
    if(argc !=2) 
    { 
        printf("usage: ./client <ipaddress>\n"); 
        return 0; 
    } //创建socket 
    clientInit();
    printf("send msg to server:\n"); 
    
    fstream fin("test",ios::binary|ios::in);
    struct timeval begin_time,end_time;
    gettimeofday(&begin_time,NULL);
    while(!fin.eof())
    {
        for(int i=0;i<10;i++)
        {
            fin.read(sendline,MAXLINE);
            int length = fin.gcount();
            if ( send(sockfd[i],sendline,length,0) <0) 
            { 
                printf("send msg error: %s(errno :%d)\n",strerror(errno),errno); 
                return 0; 
            } 
        }
    }
    for(int i=0;i<10;i++)
        close(sockfd[i]); 
    gettimeofday(&end_time,NULL);
    printf("%fkb/s\n",87999144/1000.0/(end_time.tv_sec-begin_time.tv_sec+(end_time.tv_usec-begin_time.tv_usec)/1000000.0));
    return 0; 
} 
