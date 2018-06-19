#include<unp.h>
#define BUF_SIZE 1024  
  
int main() {  
      
    //check file  
    FILE *fp = fopen("/home/gerunze/UNIX/2.txt", "rb");  
    if (fp == NULL) {  
        printf("Cannot open file, press any key to exit!\n");  
        system("pause");  
        exit(0);  
    }  
  
    //create socket  
    int sock = socket(AF_INET, SOCK_STREAM, 0);  
  
    //send request to server(specific ip and port)  
    struct sockaddr_in serv_addr;  
    memset(&serv_addr, 0, sizeof(serv_addr));  
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//server's IP address.  
    serv_addr.sin_port = htons(1234);//server's port  
    connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));  
  
    //send file  
    char buffer[BUF_SIZE] = {0};  
    int nCount;  
    while ((nCount = fread(buffer, 1, BUF_SIZE, fp)) > 0) {  
        send(sock, buffer, nCount, 0);  
    }  
      
    fclose(fp);  
      
    printf("File Transfer Finished!\n");  
  
  
    //close socket  
    close(sock);  
  
    return 0;  
}  
