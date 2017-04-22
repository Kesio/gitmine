#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
int sock;
char ipaddr[20];

void init(){
	//创建套接字
	sock = socket(AF_INET, SOCK_STREAM, 0);
	//向服务器（特定的IP和端口）发起请求
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
	serv_addr.sin_family = AF_INET;  //使用IPv4地址
	serv_addr.sin_addr.s_addr = inet_addr(ipaddr);  //具体的IP地址
	serv_addr.sin_port = htons(1234);  //端口
	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
}
void * recvthread(void * p){
	while(1){
		char buffer[200];

		if(read(sock, buffer, sizeof(buffer)) <= 0){//接收服务端的信息
			printf("服务器停止服务\n");
			close(sock);
			exit(1);
		}
		printf("%s", buffer);//成功接收，显示
	}		
}

int main(int argc, char * argv[]){
	if(argc != 3){
		printf("输入./client username ip\n");
		return -1;
	}

	else{
		pthread_t thread;
		pthread_create(&thread, 0, recvthread, 0);
		char buffer[200];
		char name[20];
		char msg[200];
		strcpy(name, argv[1]);//得到用户名
		strcpy(ipaddr, argv[2]);//得到Ip
		init();
		write(sock, name, sizeof(name));//发送用户名
		scanf("%s", &buffer);//输入
		while(!(strcmp(buffer, "q!") == 0)){
			sprintf(msg, "%s说：%s\n", name, buffer);
			write(sock, msg, sizeof(msg));//发送
			scanf("%s", &buffer);
		}
		
		close(sock);
		return 0;
	}
}
