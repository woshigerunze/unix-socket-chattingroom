#include<unp.h>
#include<string.h>
#include<mysql/mysql.h>
#include<pthread.h>
#include<time.h>
int maxsize=100;
int listenfd,connfd;
struct sockaddr_in servaddr,cliaddr;
socklen_t clilen;
MYSQL* conn;
char buff[100];
void mysqlconnect()
{
	conn=mysql_init(NULL);

	if(mysql_real_connect(conn,"localhost","root","Geliang8888","test",0,NULL,0))
	{
		printf("数据库链接成功！\n");
	}
	
}
void registerin(char* buff)
{
	char temp1[7]={};
	char temp2[7]={};
	for(int i=0;i<6;i++)
	{
		temp1[i]=buff[i];
		temp2[i]=buff[i+6];
	}
	char func[100];
	char tickschar[100];
	time_t ticks;
	ticks=time(NULL);
	snprintf(tickschar, sizeof(tickschar), "%.24s\r\n", ctime(&ticks));
	sprintf(func,"INSERT INTO account values('%s','%s','%s');",temp1,temp2,tickschar);
	mysql_query(conn,func);
}
void init()
{
	listenfd=Socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(9999);
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	Bind(listenfd,(SA*)&servaddr,sizeof(servaddr));
	Listen(listenfd,LISTENQ);
	
}
int main()
{
	mysqlconnect();
	init();
	while(1)
	{
	clilen=sizeof(cliaddr);
	connfd=Accept(listenfd,(SA*)&cliaddr,&clilen);
	recv(connfd,buff,sizeof(buff),0);
	registerin(buff);
	bzero(&buff,sizeof(buff));
	Close(connfd);
	}
	
}
