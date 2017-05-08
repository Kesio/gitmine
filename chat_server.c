#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
int serv_sock;//服务端套接字
int clnt_sock;//客户端套接字
struct client{
	char name[20];
	int number;
};//客户信息存储
struct client c[100] = {0};
int size = 0;//客户规模
struct sockaddr_in clnt_addr;
socklen_t clnt_addr_size = sizeof(clnt_addr);
int len = sizeof(clnt_addr);
struct blist{//禁言名单
	char bname[20];
	//int bnum;
} ;
struct blist b[100] = {0};
int bsize = 0;//禁言规模
//初始化
void init(){
	serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	 //将套接字和IP、端口绑定
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
	serv_addr.sin_family = AF_INET;  //使用IPv4地址
	serv_addr.sin_addr.s_addr = inet_addr("172.16.40.45");  //具体的IP地址
	serv_addr.sin_port = htons(1234);  //端口
	bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	 //进入监听状态，等待用户发起请求
	listen(serv_sock, 20);
}
void * nospeak(void * p){
	char buf[100];
//	printf("提供禁言\n");
	while(1){
		scanf("%s", &buf);
		if(strcmp(buf, "speak") == 0){//移除禁言对象
			printf("解除禁言，请输入一个解禁用户名：\n");
			scanf("%s", &buf);
			for(int i = 0; i < bsize; i++){
				if(strcmp(buf, b[i].bname) == 0){
					strcpy(b[i].bname, b[size-1].bname);
				//	b[i].bnum = b[size-1].bnum;
				}
			}
			printf("解禁成功\n");
			bsize--;
		}
		if(strcmp(buf, "nspeak") == 0){//添加禁言对象
			printf("开启禁言，请输入一个被禁用户名：\n");
			scanf("%s", &b[bsize].bname);
			//for(int i = 0; i < size; i++){//找到禁言对象
			//	if(strcmp(b[bsize].bname, c[i].name) == 0){
				//	b[bsize].bnum = c[i].number;
			//	}
		//	}
			printf("禁言成功\n");
			bsize++;
		}
	}
}
void * clntthread(void * clnt_sock){
	getpeername(clnt_sock, (struct sockaddr *)&clnt_addr, &len);
	char str[200];
	char msg[200];
	int num = (int *)clnt_sock;
	c[size].number = num;//存储用户套接字
	char name[20] = {0};
	recv(num, name, sizeof(name), 0);//接收用户名

	for(int i = 0; i < size; i++){//判断是否重名
		if(strcmp(name, c[i].name)==0){
			close(num);//重名关闭客户字
			return 0;
		}
	}

	strcpy(c[size].name, name);//存储用户名
	size++;
	sprintf(msg, "---------(%s)%s上线---------\n", inet_ntoa(clnt_addr.sin_addr), name);//广播上线信息
	for(int i = 0; i<size; i++){
		send(c[i].number, msg, sizeof(msg), 0);   
	}
	printf("%s",msg);
	char nospeak[100];
	strcpy(nospeak, "你已被系统禁言\n");
	while(1){
		if(recv(num, str, sizeof(str), 0) <= 0){//接收客户信息，判断是否在线
			//下线
			for(int i = 0; i < size; i++){
				if(c[i].number == num){//比较，哪个用户下线
					c[i].number = c[size-1].number;//最后一个有效用户信息覆盖当前无效用户
					strcpy(c[i].name,c[size-1].name);
				}
			}
			size--;//有效用户规模减少
			sprintf(msg,"---------(%s)%s已下线---------\n", inet_ntoa(clnt_addr.sin_addr), name);
			printf(msg);
			for(int i = 0; i<size; i++){//广播客户下线信息
				send(c[i].number, msg, sizeof(msg), 0);
			}
			close(num);//关闭此客户套接字
			return 0;
		}
		//在线
		for(int j = 0; j < bsize; j++){
			if(strcmp(name, b[j].bname) == 0){
				send(num, nospeak, sizeof(nospeak), 0);//new
				if(recv(num, str, sizeof(str), 0) <= 0){//接收客户信息，判断是否在线
				//下线
					for(int i = 0; i < size; i++){
						if(c[i].number == num){//比较，哪个用户下线
							c[i].number = c[size-1].number;//最后一个有效用户信息覆盖当前无效用户
							strcpy(c[i].name,c[size-1].name);
						}
					}
					size--;//有效用户规模减少
					sprintf(msg,"---------(%s)%s已下线---------\n", inet_ntoa(clnt_addr.sin_addr), name);
					printf(msg);
					for(int i = 0; i<size; i++){//广播客户下线信息
						send(c[i].number, msg, sizeof(msg), 0);
					}
					close(num);//关闭此客户套接字
					return 0;
				}
				--j;
			}
		}

		printf("%s\n", str);
		sprintf(msg, "(%s) %s", inet_ntoa(clnt_addr.sin_addr), str);
		for(int i = 0; i<size; i++){//广播客户发送的信息
			send(c[i].number, msg, sizeof(msg), 0);
			
		}
	}
}

int main(){
	pthread_t threads;
	pthread_t pid;
	int cli;
	int nos;
	init();
	while(1){
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		nos = pthread_create(&pid, NULL, nospeak, 0);//禁言线程
		cli = pthread_create(&threads, NULL, clntthread, clnt_sock);//多线程
	}
	close(serv_sock);
	
	return 0;
}
