#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") 

int main(int argc, char* argv[])
{
	//��ʼ��WSA  ��ʹ��WinSock API֮ǰ���������WSAStartup����������г�ʼ����

    WORD socketVersion = MAKEWORD(2,2);
    WSADATA wsaData; 
    if(WSAStartup(socketVersion, &wsaData) != 0)
    {
        return 0;
    }
	//�����׽���
    SOCKET sclient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    //��IP�Ͷ˿�
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888);

	//�жϵڶ��������Ƿ�Ϊ��
	if(argv[1]!=NULL)
	{
		sin.sin_addr.S_un.S_addr = inet_addr(argv[1]);//������������
    }
	else
	{
		char ipaddr[20];
		printf("����������ip��ַ:");
		scanf("%s", &ipaddr);
		
		//����ip��ַ�Ϸ���


		sin.sin_addr.S_un.S_addr = inet_addr(ipaddr);//���ӱ���
	}
	int len = sizeof(sin);
    
	//��ʼ��
    char sendData[255]={'\0'};
	
	while(true)
	{
		//�ͻ�������
		printf("�ң�");
		scanf("%s", &sendData);

		//������Ϣ
		sendto(sclient, sendData, strlen(sendData), 0, (sockaddr *)&sin, len);

		char recvData[255]={'\0'};  
		
		//��������
		int ret = recvfrom(sclient, recvData, 255, 0, (sockaddr *)&sin, &len);
		if(ret > 0)
		{
			recvData[ret] = 0x00;
			printf("�����ˣ�%s", recvData);
		}
	}

    closesocket(sclient);
    WSACleanup();
    return 0;
}
