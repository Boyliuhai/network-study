#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") 

int main(int argc, char* argv[])
{
    WSADATA wsaData;
	//声明winsocket版本
    WORD sockVersion = MAKEWORD(2,2);
	//声明函数
	char* Search(char* path, char* tosearch);//路径和查找的对象
		
	//智能语料库
	char * filepath = "C:\\data.txt";
	
	//激活wsa
    if(WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }
	
	//建立socket
    SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
    if(serSocket == INVALID_SOCKET)
    {
        printf("创建套接字失败 !");
        return 0;
    }
	//绑定IP和端口
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(8888);
    serAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	//开始监听
    if(bind(serSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
    {
        printf("listen error!");
        closesocket(serSocket);
        return 0;
    }


    //循环接收数据

	//remote_addr代表客户端的IP，但它的值不是由客户端提供的，而是服务端根据客户端的ip指定的
    sockaddr_in remoteAddr;
    int nAddrLen = sizeof(remoteAddr); 

	printf("机器人已启动，等待人类的请求...\n");

    while (true)
    {
		//接收到的请求数据
        char recvData[255]={'\0'};
		//应答数据
		char sendData[255]={'\0'};
		
		//接收数据
        int ret = recvfrom(serSocket, recvData, 255, 0, (sockaddr *)&remoteAddr, &nAddrLen);

		//接收成功
        if (ret > 0)
        {
            recvData[ret] = 0x00;
			
			//将一个十进制网络字节序转换为点分十进制IP格式的字符串
            printf("接受到一个来自IP为%s人类的连接...\n", inet_ntoa(remoteAddr.sin_addr));
            printf("人类：%s\n", recvData);          
        }

		//查找答复
		strcat(sendData,Search(filepath, recvData));//连接查找的数据送给sendData

		//答复长度
		int len = strlen(sendData);

		//发送(应答)数据
        sendto(serSocket, sendData, len, 0, (sockaddr *)&remoteAddr, nAddrLen);    

		//打印服务端提示
		printf("机器人：%s", sendData);
    }

	//关闭socket和wsa
    closesocket(serSocket); 
    WSACleanup();

    return 0;
}

//查找答复
char* Search(char* path, char* tosearch)
{
	char* File2String(const char* path);
	int StringFind(const char *pSrc, const char *pDst);//两个参数SRC为路径    DST为
	
	//字符串（源）,定义一个字符指针
	const char* src;
	//回答限定在255个字符以内
	char answer[256]={'\0'};

	//从文件中获取字符串
	src = File2String(path);

	if(src!=NULL)
	{
		//查找字符串
		int i=StringFind(src, tosearch);//本行匹配开始
		int j=i;//本行匹配结束

		//查找成功
		if(j!=-1)
		{
			//遇到本行结束
			while(src[j]!='\n')
			{
				j++;
			}
		}

		//答复是否开始
		bool flag = false;
		int p = 0;
		
		//组织答复信息
		for(int k=i;k<j+1;k++)
		{
			if(src[k]=='\t')
			{
				k++;
				//设置标志位
				flag = true;
			}

			//打印答复
			if(flag)
			{
				answer[p] = src[k];
				p++;
			}
			i++;
		}

		//默认答复
		if(strlen(answer)<=0)
		{
			strcat(answer, "你说什么，我没听太懂\n");
		}
	}
	else
	{
		printf("读取的文件为空！");
	}

	return answer;
}

//查找字符串中指定子串
int StringFind(const char *pSrc, const char *pDst)  
{  
    int i, j; 

	//循环遍历字符串
    for (i=0; pSrc[i]!='\0'; i++) 
    {
		//字符串和子串开头不匹配，则继续进行下一步
        if(pSrc[i]!=pDst[0])  
            continue; 
		
		//若字符串和子串开头匹配
        j = 0;

		//从当前位置循环遍历两个字符串
        while(pDst[j]!='\0' && pSrc[i+j]!='\0') 
        {  
            j++;

			//期间有不匹配的字符串
            if(pDst[j]!=pSrc[i+j])
				break;  
        }

		//如果遍历到子串结尾，代表全部匹配成功，返回字符串中匹配的开始位置
        if(pDst[j]=='\0')  
            return i;  
    }  

	//匹配失败
    return -1;  
} 

//读取文件
char* File2String(const char* path)
{
	FILE *fp;
	
	//打开文件
	if((fp=fopen(path, "r"))==NULL) {
		printf("文件不能打开！/n"); 
		exit(1); 
	}

	//移动文件流的读写位置，
	//欲将读写位置移动到文件开头时:fseek(FILE *stream, 0, SEEK_SET);
	//欲将读写位置移动到文件尾时:fseek(FILE *stream, 0, SEEK_END);
	fseek(fp, 0, SEEK_END);

	//ftell当前的读写位置
	//通过先将文件指针指向文件末尾，再计算文件的读写位置的方式，计算出文件的长度
	int fileLen = ftell(fp);

	//分配内存
	char* src = (char *)malloc(sizeof(char)*(fileLen+1));
	
	//初始化
	for(int i=0;i<=fileLen;i++)
	{
		src[i]='\0';
	}

	//文件指针移动到开始位置
	fseek(fp, 0, SEEK_SET);
	
	//读取文件
	fread(src, fileLen, sizeof(char), fp);

	//关闭文件
	fclose(fp); 

	return src;
}
