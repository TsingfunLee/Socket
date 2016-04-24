// Client.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#ifndef DLL_FILE
#define DLL_FILE
#endif

#include "Socket.h"
#include <iostream>
using namespace std;


Socket::Socket()
{
	printf("��ӭ����������� (\\�ݨ���/)   ����һ���س���");
	getchar();
	printf("*ʹ���ֲ�*��������Ϣ�س��������죬����#�س����Է����ļ�  ����һ���س���");
	getchar();
	printf("���Լ�ȡ�����ְɣ������ܳ���20��Ӣ���ַ���һ�����ֵ���2��Ӣ���ַ���\n");
	cin >> ID;        //��ȡ�û���

	sockVersion = MAKEWORD(2, 0);
	::WSAStartup(sockVersion, &wsaData);     // ��ʼ��WS2_32.dll
	connectSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);     // �����׽���
	if (connectSocket == INVALID_SOCKET)    
	{
		printf("�����ˣ��s�ިt������һ�°ɡ��������׽���ʧ�ܣ���\n");
		::WSACleanup();
	}

	servAddr.sin_family = AF_INET;          //TCPЭ�� 
	servAddr.sin_port = htons(8888);        //�˿ں�   
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");     //IP��ַ

	while (::connect(connectSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		printf("�����ˣ��s�ިt�����������ӷ�����ʧ�ܣ���\n");
		printf("30����������ӡ���\n");
		Sleep(30000);
		/*::WSACleanup();*/
	}
	
	printf("��ʼ�����(*^__^*) \n");
	printf("Tips:˵����һ�β�Ҫ����512���֣�������㣩����Ȼ���ܻᷢ�������������\n");
}


Socket::~Socket()
{
	::closesocket(connectSocket);      // �ر��׽���
	::WSACleanup();       // �ͷ�WS2_32��
}


/* ������Ϣ���� */
void Socket::Receive()
{
	PACKAGE_HEAD pckgHead;        //�����ͷ
	char Head[256] = { 0 };       //��ͷ������
	char fileName[64] = { 0 };    //ȫΪ0���ַ����飬�����жϰ�ͷ�ڵ��ļ����Ƿ�ȫΪ0
	char Data[1024] = { 0 };      //���ݻ�����
	int dataLenth = 0;            //���ܵ����ݵĳ���
	char fileAddr[256] = "D:\\";      //���յ��ļ�����ĵ�ַ
	::recv(connectSocket, Head, 256, 0);      //���ܰ�ͷ
	memset(&pckgHead, 0, sizeof(pckgHead));      //��հ�ͷ�ṹ��
	memcpy(&pckgHead, Head, sizeof(pckgHead));   //����ͷ
	if (strcmp(pckgHead.fileName, fileName) == 0)   //�ļ���ȫΪ0�������Ϣ����������ļ�
	{
		dataLenth = ::recv(connectSocket, Data, 1024, 0);        //�������ݣ�����ʵ�ʽ��ܵ��ֽ���
		if (dataLenth == pckgHead.msg_len)           //�������ݳ��ȵ��ڰ�ͷ�ڵ����ݳ���
		{
			printf("%s:%s\n", pckgHead.ID, Data);    //��ӡ��Ϣ
		}
	}
	else
	{
		strcat(fileAddr, pckgHead.fileName);         //���ļ����ӵ�����ĵ�ַ�ϣ�����ļ���ַ
		//���ļ���ַ��д����һ���ļ����
		HANDLE hFile = CreateFile((LPCSTR)fileAddr, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD NumberOfBytesRecv = 0;       //���յ����ݵ��ֽ���
		do{
			NumberOfBytesRecv = ::recv(connectSocket, Data, 1024, 0);         //һ�ν����յ����ֽ���
			dataLenth = dataLenth + NumberOfBytesRecv;            //���ܵ������ֽ���
			::WriteFile(hFile, Data, NumberOfBytesRecv, &NumberOfBytesRecv, NULL);      //������д���ļ���ַ
		} while (dataLenth != pckgHead.msg_len);            //�������ֽ������ڰ�ͷ�ڵ����ݳ���ʱֹͣ����
		printf("���յ��ļ����뵽D:\\�в鿴���رճ������Դ��ļ���\n");
	}
}


void Socket::Send()
{
	PACKAGE_HEAD pckgHead;                //�����ͷ
	strcpy(pckgHead.ID, ID);              //��ID������ͷ���ID
	char Head[256] = { 0 };               //��ͷ������
	char Data[1024] = { 0 };              //���ݻ�����
	char fileName[64] = { 0 };            //�ļ���������
	char fileAddr[256] = { 0 };           //�洢�ļ���ַ
	memset(Data, 0, 1024);    //������ݻ���
	cin >> Data;           //�������� 
	if (Data[0] == '#'&&Data[1] == '\0')             //�������#�����ļ�
	{
		printf("�������ļ�����λ�ã�����G:\\��ע�⣺ð����Ӣ�ģ�\n");
		cin >> fileAddr;
		printf("�������ļ���������file.txt��С��64���ֽڣ�һ�����������ֽڣ�\n");
		cin >> pckgHead.fileName;
		strcat(fileAddr, pckgHead.fileName);    //���ļ�������ȥ������ļ���ַ
		//���Ѵ��ڵ��ļ��������ļ����
		HANDLE hFile = CreateFile((LPCSTR)fileAddr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		pckgHead.msg_len = GetFileSize(hFile, NULL);    //����ļ��ĳ���
		memset(Head, 0, 256);        //��հ�ͷ������
		memcpy(Head, &pckgHead, sizeof(pckgHead));        //����ͷ������
		if (hFile != INVALID_HANDLE_VALUE)      //�ļ����ڵĻ����Ͱ�ͷ
		{
			::send(connectSocket, Head, sizeof(Head), 0);
			DWORD NumberOfBytesRead = 0;        //��ȡ���ļ������ֽ���
			do{
				::ReadFile(hFile, Data, sizeof(Data), &NumberOfBytesRead, NULL);
				::send(connectSocket, Data, NumberOfBytesRead, 0);
			} while (NumberOfBytesRead);     //ѭ����ȡ�ļ������ļ���ֱ����ȡ�����ݳ���Ϊ0
		}
		else{
			printf("�ѩn�ѡ�û���ҵ��ļ��������·���\n");
		}

	}
	else            //����Ϣ
	{
		pckgHead.msg_len = strlen(Data);             //�����Ϣ���ȣ������ͷ
		strcpy(pckgHead.fileName, fileName);         //����ͷ�ڵ��ļ���ȫ����Ϊ0
		memset(Head, 0, 256);                        //��հ�ͷ������
		memcpy(Head, &pckgHead, sizeof(pckgHead));   //����ͷ������
		if (::send(connectSocket, Head, sizeof(Head), 0) && ::send(connectSocket, Data, strlen(Data), 0))   //���Ͱ�ͷ����Ϣ
		{
			printf("%s:%s\n", pckgHead.ID, Data);          //���Լ���Ļ�ϴ�ӡ����Ϣ
		}
		else{
			printf("����ʧ�ܣ��s�ިt����������������\n");
		}
	}
}

