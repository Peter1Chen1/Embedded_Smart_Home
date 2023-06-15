#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>//superset of previ - ous
#include <sys/types.h> //See NOTES
#include <arpa/inet.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "uart.h"

#define ERR_MSG(msg)	do{perror("msg");exit(-1);}while(0)
#define ADDR "192.168.150.130"
#define PORT "8888"

unsigned char buf_data[256] = {0};

unsigned char send_to_M0(int uart_fd, int acc_fd){
	unsigned char ret = 0;
	uart_send(uart_fd,buf_data,strlen(buf_data));
	memset(buf_data,0,sizeof(buf_data));
	uart_recv(uart_fd,buf_data,sizeof(buf_data),1);
	
	send(acc_fd,buf_data,strlen(buf_data),0);
	ret = buf_data[0];
	memset(buf_data,0,sizeof(buf_data));
	return ret;
}

int main(){
	int sock_fd = socket(AF_INET, SOCK_STREAM,0);
	//创建套接字（AF_INET->IPv4,流式套接字，默认填0）
	if(sock_fd == -1){
		ERR_MSG("socket");
	}
	
	struct sockaddr_in saddr;
	//memset();
	bzero(&saddr,sizeof(saddr));//clear
	saddr.sin_family = AF_INET;//IPv4协议
	saddr.sin_port = htons(atoi(PORT));
	//主机字节序转网络字节序（host to net short）
	
	//ifconfig查地址
	saddr.sin_addr.s_addr = inet_addr(ADDR);
	//ifconfig ipv4	点分十进制ip地址转网络字节
	
	int ret = bind(sock_fd, (const struct sockaddr *)&saddr, sizeof(saddr));
	if(ret == -1){
		ERR_MSG("bind");
	}
	
	ret = listen(sock_fd, 10);	//使服务器进入被动监听状态
	//监听套接字，监听队列
	if(ret == -1){
		ERR_MSG("listen");
	}
	
	int acc_fd = accept(sock_fd, NULL, NULL);	//创建通信套接字
	if(acc_fd == -1){
		ERR_MSG("accept");
	}
	
	int uart1_fd = 0;
	uart1_fd = uart_open("/dev/ttyUSB0");//打开串口文件
	uart_init(uart1_fd,115200,0,8,1,'N');//初始化串口
	//文件描述符，波特率，流量控制，数据位，停止位，校验位
	
	while(1){
		memset(buf_data, 0, sizeof(buf_data));
		ret = recv(acc_fd, buf_data, sizeof(buf_data),0);
		if(ret == -1){
			perror("recv");
			continue;
		}
		printf("%s\n", buf_data);
		
		if(buf_data[0] == '1'){
			send_to_M0(uart1_fd,acc_fd);
			memset(buf_data,0,sizeof(buf_data));
			strcpy(buf_data,"led_on");
			send(acc_fd,buf_data,strlen(buf_data),0);
		}
		
		else if(buf_data[0] == '2'){
			send_to_M0(uart1_fd,acc_fd);
			memset(buf_data,0,sizeof(buf_data));
			strcpy(buf_data,"led_off");
			send(acc_fd,buf_data,strlen(buf_data),0);
		}
			
		else if(buf_data[0] == '3'){
			send_to_M0(uart1_fd,acc_fd);
			memset(buf_data,0,sizeof(buf_data));
			strcpy(buf_data,"fan_on");
			send(acc_fd,buf_data,strlen(buf_data),0);
		}
		
		else if(buf_data[0] == '4'){
			send_to_M0(uart1_fd,acc_fd);
			memset(buf_data,0,sizeof(buf_data));
			strcpy(buf_data,"fan_off");
			send(acc_fd,buf_data,strlen(buf_data),0);
		}
		
		else if(buf_data[0] == '5'){
			send_to_M0(uart1_fd,acc_fd);
			memset(buf_data,0,sizeof(buf_data));
			strcpy(buf_data,"spk_on");
			send(acc_fd,buf_data,strlen(buf_data),0);
		}
		
		else if(buf_data[0] == '6'){
			send_to_M0(uart1_fd,acc_fd);
			memset(buf_data,0,sizeof(buf_data));
			strcpy(buf_data,"spk_off");
			send(acc_fd,buf_data,strlen(buf_data),0);
		}
		
		//ret = send(acc_fd, buf_data, sizeof(buf_data),0);
		
		/*if(ret == -1){
			ERR_MSG("send");
		}*/
		
		else{
			goto finish;
		}
	}
	

finish:	
	close(acc_fd);
	close(sock_fd);
	
	return 0;
}