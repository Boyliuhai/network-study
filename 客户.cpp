#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") 

int main(int argc, char* argv[])
{
	//初始化WSA  在使用WinSock API之前，必须调用WSAStartup函数对其进行初始化。

    WORD socketVersion = MAKEWORD(2,2);
    WSADATA wsaData; 
    if(WSAStartup(socketVersion, &wsaData) != 0)
    {
        return 0;
    }
	//创建套接字
    SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    //绑定IP和端口
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888);

	//判断第二个参数是否为空
	if(argv[1]!=NULL)
	{
		sin.sin_addr.S_un.S_addr = inet_addr(argv[1]);//连接其它主机
    }
	else
	{
		char ipaddr[20];
		printf("请输入服务端ip地址:");
		scanf("%s", &ipaddr);
		
		//检验ip地址合法性


		sin.sin_addr.S_un.S_addr = inet_addr(ipaddr);//连接本机
	}
	int len = sizeof(sin);
    
	//初始化
    char sendData[255]={'\0'};
	
	while(true)
	{
		//客户端输入
		printf("我：");
		scanf("%s", &sendData);

		//发送信息
		sendto(sclient, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);

		char recvData[255]={'\0'};  
		
		//接收数据
		int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr *)&sin, &len);
		if(ret > 0)
		{
			recvData[ret] = 0x00;
			printf("机器人：%s", recvData);
		}
	}

    closesocket(sclient);
    WSACleanup();
    return 0;
}
