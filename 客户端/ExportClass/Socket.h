#ifndef Socket_H
#define Socket_H
#include <winsock2.h>	// Ϊ��ʹ��Winsock API����
#include <stdio.h>
#include <windows.h>

// ������������WS2_32������
#pragma comment(lib,"WS2_32.lib")


struct PACKAGE_HEAD
{
	int msg_len = 0;
	char ID[20] ;
	char fileName[64] ;
};

#ifdef DLL_FILE
class _declspec(dllexport) Socket
#else
class _declspec(dllimport) Socket
#endif
{
public:
	Socket();
	~Socket();
	void Receive();
	void Send();
private:
	WSADATA wsaData;       //WSADATA���ݽṹ
	WORD sockVersion;      //�׽��ְ汾��
	SOCKET connectSocket;              //�׽���
	sockaddr_in servAddr;  //��������ַ

	char ID[20];           //�û���
};
#endif