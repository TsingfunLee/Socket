#ifndef Socket_H
#define Socket_H
#include <winsock2.h>	// 为了使用Winsock API函数
#include <stdio.h>
#include <windows.h>

// 告诉连接器与WS2_32库连接
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
	WSADATA wsaData;       //WSADATA数据结构
	WORD sockVersion;      //套接字版本号
	SOCKET connectSocket;              //套接字
	sockaddr_in servAddr;  //服务器地址

	char ID[20];           //用户名
};
#endif