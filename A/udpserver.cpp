#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

#define SERVER_PORT 2234                 /*监听端口*/
#define MAX_MSG_SIZE 1024*8              /*recv缓冲器大小*/
int sockfd;  //套接字描述符

struct sockaddr_in client;
int  client_len=sizeof(struct sockaddr_in); 
char bufTemp[10320];
void udpser_init()
{
    struct sockaddr_in server;
    bzero(&server,sizeof(server));
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=htonl(INADDR_ANY);
    server.sin_port=htons(SERVER_PORT);
    if(-1 == (sockfd=socket(AF_INET, SOCK_DGRAM,0)))    
    {
        perror("create socket failed");
        exit (1);
    }
    if(-1 == ( bind( sockfd, ( struct sockaddr * )&server, sizeof(server) )) )
    {
        perror("bind error");
        exit (1);    
    }
}

unsigned char message[MAX_MSG_SIZE];
int recv_len=0;
char addr_p[INET_ADDRSTRLEN];
char *sndbuf = "Received a pkg.\n";
typedef struct buffer
{
    char buf[MAX_MSG_SIZE];
    int id;
    int length;
}buffer;
int main(int argc, char *argv[])
{
    udpser_init();
    string filename;
    char buf[1024];
    recv_len=recvfrom(sockfd,buf,1024,0,(struct sockaddr *)&client,(socklen_t*)&client_len);
    if(sendto(sockfd,"Received filename pkg!\n",40,0,(struct sockaddr*)&client,client_len)<0)
    {
        printf("sendto error\n");
        exit(1);
    }

    time_t modefyTime;
    recv_len=recvfrom(sockfd,(char*)&modefyTime,sizeof(time_t),0,(struct sockaddr *)&client,(socklen_t*)&client_len);
    if(sendto(sockfd,"Received modefy time pkg!\n",40,0,(struct sockaddr*)&client,client_len)<0)
    {
        printf("sendto error\n");
        exit(1);
    }
    printf("修改: %s", ctime(&modefyTime));
    filename = buf;
    fstream fout(filename.c_str(),ios::out|ios::binary);
    struct timeval timeout;
    timeout.tv_sec = 1;//秒
    timeout.tv_usec = 0;//微秒
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) 
    {
        cout<<"setsockopt failed\n";
    }
    while(1)
    {
        buffer* temp;
	recv:
        recv_len=recvfrom(sockfd,bufTemp,sizeof(buffer),0,(struct sockaddr *)&client,(socklen_t*)&client_len); //阻塞式
        if(strcmp(temp->buf,"end") == 0)
        {
            if(sendto(sockfd,"Received end pkg!\n",19,0,(struct sockaddr*)&client,client_len)<0)
            {
                printf("sendto error\n");
                exit(1);
            }
            break;
        }
        temp = (buffer*)bufTemp;
        fout.write(temp->buf,temp->length);
        if(recv_len <0)
        {
           printf("recvfrom error\n");
           goto recv;
        }
        inet_ntop(AF_INET,&client.sin_addr,addr_p,sizeof(addr_p));
        printf("client IP is %s, port is %d\n",addr_p,ntohs(client.sin_port));
        message[recv_len]='\0';
        printf("[id]:\t%d\n",temp->id);
        printf("server received %d\n", temp->length);
        if(sendto(sockfd,sndbuf,strlen(sndbuf),0,(struct sockaddr*)&client,client_len)<0)
        {
           printf("sendto error\n");
           exit(1);
        }
    }
    string fileTime = ctime(&modefyTime);
    fileTime = fileTime.substr(0,fileTime.find('\n'));
    fileTime = "touch -d \"" + fileTime + '\"' + " " + filename;
    cout << fileTime << endl;
    system(fileTime.c_str());
    fout.close();
    return 0;
}