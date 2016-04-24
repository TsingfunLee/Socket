#ifndef Socket_H
#define Socket_H
#include <winsock2.h>	// Ϊ��ʹ��Winsock API����
#include <stdio.h>
#include <windows.h>

// ������������WS2_32������
#pragma comment(lib,"WS2_32.lib")

struct PACKAGE_HEAD
{
	int msg_len = 0;           //Ҫ������Ϣ�ĳ���
	char ID[20] ;              //������ID
	char fileName[64] ;        //�����ļ�������
};

#ifdef DLL_FILE
class _declspec(dllexport) Socket
#else
class _declspec(dllimport) Socket
#endif

{
public:
	Socket();               //��ʼ���׽���
	~Socket();
	void Accept();          //��������
	void Receive();         //��������
	void Send();            //��������
private:
	WSADATA wsaData;
	WORD sockVersion;
	SOCKET listenSocket;             //�����׽���
	SOCKET connectSocket;            //�շ����ݵ��׽���

	sockaddr_in serverAddr;          //��������ַ
	sockaddr_in clientAddr;          //�ͻ��˵�ַ
	char ID[20];    //ID
};
#endif
