#ifndef Socket_H
#define Socket_H
#include <winsock2.h>	// 为了使用Winsock API函数
#include <stdio.h>
#include <windows.h>

// 告诉连接器与WS2_32库连接
#pragma comment(lib,"WS2_32.lib")

struct PACKAGE_HEAD
{
	int msg_len = 0;           //要发送消息的长度
	char ID[20] ;              //发送者ID
	char fileName[64] ;        //发送文件的名字
};

#ifdef DLL_FILE
class _declspec(dllexport) Socket
#else
class _declspec(dllimport) Socket
#endif

{
public:
	Socket();               //初始化套接字
	~Socket();
	void Accept();          //接受连接
	void Receive();         //发送数据
	void Send();            //接受数据
private:
	WSADATA wsaData;
	WORD sockVersion;
	SOCKET listenSocket;             //监听套接字
	SOCKET connectSocket;            //收发数据的套接字

	sockaddr_in serverAddr;          //服务器地址
	sockaddr_in clientAddr;          //客户端地址
	char ID[20];    //ID
};
#endif
