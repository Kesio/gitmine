#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <pthread.h>

#define PORT 9800
#define MAXLENGTH 1024+1
#define WEB_SOCKET_KEY_LEN_MAX 256 
//客户离开
void clntLeave(int num);

//群发信息
void sendMsgToAll(int num,char* msg);

//客户收发线程
void * clntthread(void * clntsock);

//禁言客户线程
void * nospeak(void * p);

//base64编码
int base64_encode(char *in_str, int in_len, char *out_str);

//获取Sec-websocket-key
char * fetchSecKey(const char * buf);  

//解析数据
char * analyData(const char * buf,const int bufLen);

//打包数据
char * packData(const char * message,unsigned long * len);  

//发送数据
void response(int connfd,const char * message);

const char * GUID="258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

struct client{
	char name[20];
	int number;
	};//客户信息存储
struct client c[100] = {0};
int size = 0;//客户规模

struct blist{//禁言名单
	char bname[20];
} ;
struct blist b[100] = {0};
int bsize = 0;//禁言规模

void clntLeave(int num){
	char msg[200];
	for(int i = 0; i < size; i++){
		//比较，哪个用户下线
		if(c[i].number == num){
			sprintf(msg,"---------%s已下线---------\n",c[i].name);
			printf("%s",msg);
			sendMsgToAll(num,msg);
			//最后一个有效用户信息覆盖当前无效用户
			c[i].number = c[size-1].number;
			strcpy(c[i].name,c[size-1].name);
		}
	}
	//有效用户规模减少
	size--;
	close(num);	
	
}

void * nospeak(void * p){
	char buf[100];
	while(1){
		scanf("%s", buf);
		//移除禁言对象
		if(strcmp(buf, "speak") == 0){
			printf("解除禁言，请输入一个解禁用户名：\n");
			scanf("%s", buf);
			for(int i = 0; i < bsize; i++){
				if(strcmp(buf, b[i].bname) == 0){
					strcpy(b[i].bname, b[size-1].bname);
				}
			}
			printf("解禁成功\n");
			bsize--;
		}
		//添加禁言对象
		if(strcmp(buf, "nspeak") == 0){
			printf("开启禁言，请输入一个被禁用户名：\n");
			scanf("%s", b[bsize].bname);
			printf("禁言成功\n");
			bsize++;
		}
	}
}

void sendMsgToAll(int num,char* msg){
	int i = 0;
	for(;i<size;i++){
		if(num!=c[i].number)
			response(c[i].number,msg);
	}
}

void * clntthread(void * clntsock){
	int n;
	char buf[MAXLENGTH];
        char request[MAXLENGTH];
	char sec_accept[32];
	size_t length;
	unsigned char hash[SHA_DIGEST_LENGTH];
	char *data;
	char *username;
	char *key;  
	key=(char *)malloc(WEB_SOCKET_KEY_LEN_MAX);  
	memset(key,0, WEB_SOCKET_KEY_LEN_MAX);
	
	struct sockaddr_in clnt_addr;
	int len = sizeof(clnt_addr);
	int num=*(int *)clntsock;
	getpeername(num, (struct sockaddr *)&clnt_addr, &len);
		
	char str[100];
	char msg[100];
	
	//存储用户套接字
	c[size].number = num;

	bzero(request,MAXLENGTH);
        strcat(request,"HTTP/1.1 101 Switching Protocols\r\n");
	strcat(request,"Upgrade:websocket\r\n");
        strcat(request,"Connection:Upgrade\r\n");
	bzero(buf,MAXLENGTH);
	n=recv(num, buf, sizeof(buf), 0);
	key=fetchSecKey(buf);
	strcat(key,GUID);
	length = strlen(key);
	SHA1(key, length, hash);
	base64_encode(hash,strlen(hash),sec_accept);
	sprintf(request, "%sSec-WebSocket-Accept: %s\r\n\r\n", request, sec_accept);
	send(num,request,MAXLENGTH,0);
	//username
	bzero(buf,MAXLENGTH);
	n=recv(num,buf,MAXLENGTH,0);
	username=analyData(buf,n); 
		
	//判断是否重名
 	for(int i = 0; i < size; i++){
		if(strcmp(username, c[i].name)==0){
			strcpy(msg,"用户名重复，登录失败\n");
			response(num,msg);
			//重名关闭客户字
			close(num);
			return 0;
		}
	} 
	//存储用户名
	strcpy(c[size].name, username);
	size++;
	//广播上线信息
	bzero(msg,sizeof(msg));
	sprintf(msg, "---------(%s)%s上线---------\n", inet_ntoa(clnt_addr.sin_addr), username);
	sendMsgToAll(num,msg);

	printf("%s",msg);
	
	char nospeak[20];
	strcpy(nospeak, "你已被系统禁言\n"); 
	while(1){
		bzero(str,sizeof(str));
		n=recv(num, str, sizeof(str), 0);
		data=analyData(str,n);
		//客户不在线
		if(!strcmp(data,"quit!")){
			clntLeave(num);
			return 0;
		}

		//客户在线
		//判断禁言
		for(int j = 0; j < bsize; j++){
			if(strcmp(username, b[j].bname) == 0){
				response(num,nospeak);
				bzero(str,sizeof(str));
				n=recv(num, str, sizeof(str), 0);
				data=analyData(str,n);
				//判断是否在线
				if(!strcmp(data,"quit!")){
				//下线
					clntLeave(num);
					return 0;
				}
				--j;
			}
		}
		bzero(msg,sizeof(msg));
		sprintf(msg, "(%s)%s说: %s", inet_ntoa(clnt_addr.sin_addr),username, data);
		printf("%s\n",msg);
		sendMsgToAll(num,msg);
		
	}
}

int base64_encode(char *in_str, int in_len, char *out_str){
	BIO *b64, *bio;
	BUF_MEM *bptr = NULL;
	size_t size = 0;
	if (in_str == NULL || out_str == NULL)
		return -1;
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);
	BIO_write(bio, in_str, in_len);
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bptr);
	memcpy(out_str, bptr->data, bptr->length);
	out_str[bptr->length-1] = '\0';
	size = bptr->length;
	BIO_free_all(bio);
	return size;
}

char * fetchSecKey(const char * buf){  
	 char *key;  
	 char *keyBegin;  
	 char *flag="Sec-WebSocket-Key: ";  
	 int i=0, bufLen=0;  
		  
	 key=(char *)malloc(WEB_SOCKET_KEY_LEN_MAX);  
	 memset(key,0, WEB_SOCKET_KEY_LEN_MAX);

	 if(!buf) {  
		return NULL;  
	 }  
		         
	 keyBegin=strstr(buf,flag);  
	 if(!keyBegin){
		 return NULL;  
	 }  
	 keyBegin+=strlen(flag);  
			      
	 bufLen=strlen(buf);  
	 for(i=0;i<bufLen;i++){  
		 if(keyBegin[i]==0x0A||keyBegin[i]==0x0D){  
			 break;  
		 }  
		 key[i]=keyBegin[i];  
	 }  
				    
	 return key;  
}  

char * analyData(const char * buf,const int bufLen){  
	char * data;  
	char fin, maskFlag,opcode,masks[4];  
	char * payloadData;  
	char temp[8];  
	unsigned long n, payloadLen=0;  
	unsigned short usLen=0;  
	int i=0;   
  
	if (bufLen < 2){  
		return NULL;  
	}  
	// 1bit，1表示最后一帧  
	fin = (buf[0] & 0x80) == 0x80;   
	if (!fin){  
		// 超过一帧暂不处理 
		return NULL;  
	}  
	// 是否包含掩码  
	maskFlag = (buf[1] & 0x80) == 0x80;   
	if (!maskFlag){  
		// 不包含掩码的暂不处理  
		return NULL;
	}  
	opcode=(buf[0]&0x08)==0x08;
	if(opcode){
		char * out;
		out="quit!";
		return out;
	}
	// 数据长度 
	payloadLen = buf[1] & 0x7F;   
	if (payloadLen == 126){        
		memcpy(masks,buf+4, 4);        
		payloadLen =(buf[2]&0xFF) << 8 | (buf[3]&0xFF);    
		payloadData=(char *)malloc(payloadLen);  
		memset(payloadData,0,payloadLen);  
		memcpy(payloadData,buf+8,payloadLen);  
	}  
	else if (payloadLen == 127){  
		memcpy(masks,buf+10,4);    
		for ( i = 0; i < 8; i++){  
			temp[i] = buf[9 - i];  
		}   
		memcpy(&n,temp,8);    
		payloadData=(char *)malloc(n);   
		memset(payloadData,0,n);   
		memcpy(payloadData,buf+14,n); 
		payloadLen=n;      
	}  
	else{     
		memcpy(masks,buf+2,4);      
		payloadData=(char *)malloc(payloadLen);  
		memset(payloadData,0,payloadLen);  
		memcpy(payloadData,buf+6,payloadLen);   
	}  
  
  
	for (i = 0; i < payloadLen; i++){  
		payloadData[i] = (char)(payloadData[i] ^ masks[i % 4]);  
	}
	payloadData[payloadLen] = '\0';
	return payloadData;  
}  

char *  packData(const char * message,unsigned long * len){  
    char * data=NULL;  
    unsigned long n;  
  
    n=strlen(message);  
        if (n < 126){  
			data=(char *)malloc(n+2);  
			memset(data,0,n+2);      
			data[0] = 0x81;  
			data[1] = n;  
			memcpy(data+2,message,n);  
			*len=n+2;  
        }  
        else if (n < 0xFFFF){  
			data=(char *)malloc(n+4);  
			memset(data,0,n+4);  
			data[0] = 0x81;  
			data[1] = 126;  
			data[2] = (n>>8 & 0xFF);  
			data[3] = (n & 0xFF);  
			memcpy(data+4,message,n);      
			*len=n+4;  
        }  
        else{  
       
            // 暂不处理超长内容    
			*len=0;  
        }  
    
  
        return data;  
 }  
  
void response(int connfd,const char * message){  
		char * data;  
		unsigned long n=0;  
 
		data=packData(message,&n);   
   
		if(!data||n<=0){  
			printf("data is empty!\n");  
			return;  
		}   
		
		
			send(connfd,data,n,0);  
		
		
}  

int main(int argc,char ** argv){
        int sockfd,len,newfd;
		int reuse=1;
		pthread_t threads;
		pthread_t pid;
        struct sockaddr_in l_addr;
        struct sockaddr_in c_addr;
		char request[MAXLENGTH];
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
        if(sockfd<0)
                perror("socket");
        l_addr.sin_port=htons(PORT);
        l_addr.sin_family=AF_INET;
        l_addr.sin_addr.s_addr=INADDR_ANY;
        if(bind(sockfd,(struct sockaddr*)&l_addr,sizeof(struct sockaddr))<0)
                perror("bind");
        listen(sockfd,20);
        len=sizeof(struct sockaddr);
	
        bzero(request,MAXLENGTH);
		void * clntsock = &newfd;
        while(1){
            newfd=accept(sockfd,(struct sockaddr*)&c_addr,&len);
			//客户收发线程	
			pthread_create(&threads, NULL, clntthread, clntsock);
			//禁言客户线程
			pthread_create(&pid, NULL, nospeak, 0);
        }
        return 0;
}
