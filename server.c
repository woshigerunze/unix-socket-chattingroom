#include<unp.h>
#include<pthread.h>
#include<mysql/mysql.h>
#include<time.h>
#include<string.h>
int maxsize=100;
int socketfd[100];
int listenfd;
int nowconnect=0;
MYSQL* conn;
MYSQL_RES* res;
MYSQL_ROW row;
struct sockaddr_in servaddr;
void mysqlconnection()
{
	conn=mysql_init(NULL);
	if(mysql_real_connect(conn,"localhost","root","Geliang8888","test",0,NULL,0));
		printf("已经成功连接数据库！\n");
}
void init()
{
	listenfd=Socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(6666);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	Bind(listenfd,(SA*)&servaddr,sizeof(servaddr));
	Listen(listenfd,100);
		
}
int login(int fd)
{
	char	temp[100];
	char str1[7]={};
	char str2[7]={};
	recv(fd,temp,sizeof(temp),0);
	for(int i=0;i<strlen(temp);i++)
	{
		str1[i]=temp[i];
		str2[i]=temp[i+6];
	}
	str1[6]=0;
	str2[6]=0;
	printf("%s\n",str1);
	printf("%s\n",str2);
	int select=mysql_query(conn,"select *from account");
	res=mysql_store_result(conn);
	while(row=mysql_fetch_row(res))
	{
		int t=0;
			if((strcmp(str1,row[t])==0)&&strcmp(str2,row[t+1])==0)
			{
				char buff[]="登录成功!";
				send(fd,buff,strlen(buff),0);
				return 1;
			}
	}
	return 0;
}
void SendMsgToAll(char* buf)
{
	for(int i=0;i<maxsize;i++)
		if(socketfd[i]!=0)
		{
			send(socketfd[i],buf,strlen(buf),0);
		}
}
void* chat(void* arg)
{
	int fd=(int)arg;
	while(1)
	{
		char buf[100]={};
		char temp[100];
		char tickschar[100];
		int i=0;
		time_t ticks;
		if(recv(fd,buf,sizeof(buf),0)<=0)
		{
			for( i=0;i<maxsize;i++)
				if(fd==socketfd[i])
				{
					socketfd[i]=0;
					break;
				}
				printf("客户退出服务器！\n");
				nowconnect--;
				pthread_exit((void*)i);
		}
		SendMsgToAll(buf);
		ticks=time(NULL);
		snprintf(tickschar, sizeof(tickschar), "%.24s\r\n", ctime(&ticks));
		sprintf(temp,"INSERT INTO chat values('%s','%s');",buf,tickschar);
		
		mysql_query(conn,temp);
		
		
		//int res=mysql_query(conn,"INSERT INTO (id,amount) VALUES(2,30)");
	}
}
void service()
{
	printf("服务器启动！\n");
	while(1)
	{
		struct sockaddr_in cliaddr;
		socklen_t len=sizeof(cliaddr);
		int fd=accept(listenfd,(SA*)&cliaddr,&len);
		if(fd==-1)
		{
			printf("客户端连接出错！\n");
			continue;
		}
		nowconnect++;
		if(nowconnect==maxsize)
		{
			char* str="当前连接数满了，请过会再连接!";
			send(fd,str,strlen(str),0);
			close(fd);
		}
		if((login(fd))!=1)
		{
			Close(fd);
			continue;
		}
		for(int i=0;i<maxsize;i++)
			if(socketfd[i]==0)
			{
				socketfd[i]=fd;
				printf("当前连接数:%d\n",nowconnect);
				pthread_t tid;
				pthread_create(&tid,NULL,chat,(void*)fd);
				break;
				
			}
		
	}
}
int main()
{
	mysqlconnection();
	init();
	service();
}






































