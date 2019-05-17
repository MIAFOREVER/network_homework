/**********************************************************************
* 版权所有 (C)2016, WangSq。
*
* 文件名称：xx.c
* 文件标识：无
* 内容摘要：udpclient演示程序
* 其它说明：无
* 当前版本：V1.0
* 作    者：WangSq
* 完成日期：2016
*
**********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
using namespace std;
/**********************************************************************
* 功能描述：主函数
* 输入参数：无
* 输出参数：无
* 返 回 值：无
* 其它说明：无
* 修改日期        版本号     修改人            修改内容
* -------------------------------------------------------------------
* 2016xxxx       V1.0       WangSq       创建
***********************************************************************/
#define UDP_TEST_PORT       2234

#define UDP_SERVER_IP       "127.0.0.1"

#define MAX_LINE             1024*8

struct timeval begin_time;
struct timeval end_time;

typedef struct buffer
{
    char buf[MAX_LINE];
    unsigned int id;
    unsigned int length;
}buffer;
buffer temp;
int main(int argC, char* arg[])
{
    struct sockaddr_in server;
    int sockfd, len = 0;   
    int server_len = sizeof(struct sockaddr_in);     
    char *sendStr ="i am a client\n"; //默认发送串
    char buf[MAX_LINE];//接收缓冲
    /* setup a socket，attention: must be SOCK_DGRAM */
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
   
        perror("socket");
        exit(1);
    }
    
 
    /*complete the struct: sockaddr_in*/
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(UDP_TEST_PORT);
    server.sin_addr.s_addr = inet_addr(UDP_SERVER_IP);

    struct timeval timeout;
    timeout.tv_sec = 0;//秒
    timeout.tv_usec = 10000;//微秒
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) 
    {
        cout<<"setsockopt failed\n";
    }
    cout<<"Please type filename\n";
    string filename;
    cin >> filename;
    fstream fin(filename,ios::binary|ios::in);
    if(gettimeofday(&begin_time, NULL)==-1)
        cout<<"[Error]:\t get time error\n";
    int i = 0;
    if(sendto(sockfd, filename.c_str(),  filename.size(), 0, (struct sockaddr *)&server, server_len)<0) //sizeof
    {
        printf("sendto error\n");
        exit(1);
    }
    len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&server, (socklen_t*)&server_len);
    if(len <0)
    {
       printf("recvfrom error\n");
    }
    printf("Receive from server: %s\n", buf);

    struct stat bufTemp;
    int result;
    result =stat( filename.c_str(), &bufTemp );
    if( result != 0 )
        perror( "[Error]:\t" );
    else
    {
        printf("文件大小: %d\n", bufTemp.st_size);
        printf("文件创建时间: %s\n", ctime(&bufTemp.st_ctime));
        printf("访问日期: %s\n", ctime(&bufTemp.st_atime));
        printf("最后修改日期: %s\n", ctime(&bufTemp.st_mtime));
    }
    time_t modefyTime = bufTemp.st_mtime;

    if(sendto(sockfd, &modefyTime,  sizeof(modefyTime), 0, (struct sockaddr *)&server, server_len)<0) //sizeof
    {
        printf("sendto error\n");
        exit(1);
    }
    len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&server, (socklen_t*)&server_len);
    if(len <0)
    {
       printf("recvfrom error\n");
    }
    printf("Receive from server: %s\n", buf);

    while(!fin.eof()) {
        fin.read(temp.buf, MAX_LINE);
        temp.length = fin.gcount();
        temp.id = i;
        cout<<"[id]:\t"<<temp.id<<"\t[length]:\t"<<temp.length<<endl;
        send:
        if(sendto(sockfd, &temp,  sizeof(buffer), 0, (struct sockaddr *)&server, server_len)<0) //sizeof
        {
            printf("sendto error\n");
            exit(1);
        }

        memset(buf, 0, sizeof(buf));

        /* recieve the string from server*/

        len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&server, (socklen_t*)&server_len);
        if(len < 0)
        {
           printf("recvfrom error\n");
           goto send;
        }
        printf("Receive from server: %s\n", buf);
        i++;
    }
    if(sendto(sockfd, "end",  4, 0, (struct sockaddr *)&server, server_len)<0) //sizeof
    {
        printf("sendto error\n");
        exit(1);
    }
    len = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&server, (socklen_t*)&server_len);
    if(len <0)
    {
       printf("recvfrom error\n");
       exit(1);
    }
    printf("Receive from server: %s\n", buf);
    if(gettimeofday(&end_time, NULL)==-1)
        cout<<"[Error]:\t get time error\n";
    cout<<"Band width :\t"<<bufTemp.st_size/1024/(((end_time.tv_sec-begin_time.tv_sec)*1000000 + end_time.tv_usec-begin_time.tv_usec)/1000000.0)<<"kB/s"<<endl;
    return 0;
}