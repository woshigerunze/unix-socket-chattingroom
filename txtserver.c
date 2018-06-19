#include<unp.h>
#define BUF_SIZE 1024  
  
int main() {  
    FILE *fp = fopen("/home/gerunze/UNIX/1.txt","wb");  
    if(fp==NULL){  
        printf("Cannot open file, press any key to exit!\n");  
        system("pause");  
        exit(0);  
    }  
    //create socket  
    int serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
  
    //bind socket with IP and port  
    struct sockaddr_in serv_addr;  
    memset(&serv_addr,0,sizeof(serv_addr));  
    serv_addr.sin_family=AF_INET;  
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");//ip address  
    serv_addr.sin_port=htons(1234);//port  
    bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));  
  
    //Into the listening state,waiting for client  
    listen(serv_sock,20);  
  
    //receiving request  
    struct sockaddr_in clnt_addr;  
    socklen_t clnt_addr_size=sizeof(clnt_addr);  
    int clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);  
  
    //return response  
    char buffer[BUF_SIZE]={0};  
    int nCount;  
        while ((nCount = recv(clnt_sock, buffer, BUF_SIZE, 0)) > 0) {  
            fwrite(buffer,nCount,1, fp);  
        }  
          
  
    fclose(fp);  
    printf("Recieve File From Client Finished!\n");  
  
    //close socket  
    close(clnt_sock);  
    close(serv_sock);  
  
    return 0;  
}  
