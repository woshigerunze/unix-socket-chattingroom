#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

int sockfd;//客户端socket
//char* IP = "127.0.0.1";//服务器的IP
short PORT = 6666;//服务器服务端口
typedef struct sockaddr SA;
char name[30];

void init(){
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    if (connect(sockfd,(SA*)&addr,sizeof(addr)) == -1){
        perror("无法连接到服务器");
        exit(-1);
    }
    printf("客户端启动成功\n");
}

void start(){
    pthread_t id;
    void* recv_thread(void*);
    pthread_create(&id,0,recv_thread,0);
    char buf2[100] = {};
    sprintf(buf2,"%s进入了聊天室",name);
    send(sockfd,buf2,strlen(buf2),0);
    while(1){
        char buf[100] = {};
        scanf("%s",buf);
        char msg[131] = {};
        sprintf(msg,"%s:%s",name,buf);
        send(sockfd,msg,strlen(msg),0);
        if (strcmp(buf,"bye") == 0){
            memset(buf2,0,sizeof(buf2));
            sprintf(buf2,"%s退出了聊天室",name);
            send(sockfd,buf2,strlen(buf2),0);
            break;
        }
    }
    close(sockfd);
}

void* recv_thread(void* p){
    while(1){
        char buf[100] = {};
        if (recv(sockfd,buf,sizeof(buf),0) <= 0){
            return;
        }
        printf("%s\n",buf);
    }
}

int main(){
    init();
    printf("请输入您的名字：");
    scanf("%s",name);
    start();
    return 0;
}
