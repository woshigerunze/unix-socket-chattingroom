#include<unp.h>
int sockfd;
struct sockaddr_in servaddr;
char passname[100];
char password[100];
void init()
{
	sockfd=Socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(9999);
	Connect(sockfd,(SA*)&servaddr,sizeof(servaddr));
	printf("链接成功！\n");
}
int main()
{
	init();
	printf("请输入用户名：\n");
	scanf("%s",passname);
	printf("请输入密码:\n");
	scanf("%s",password);
	send(sockfd,passname,strlen(passname),0);
	send(sockfd,password,strlen(password),0);
	exit(0);
}

