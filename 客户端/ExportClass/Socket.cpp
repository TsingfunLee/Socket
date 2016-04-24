// Client.cpp : 定义控制台应用程序的入口点。
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
	printf("欢迎进入聊天程序 (\\≥▽≤/)   （下一步回车）");
	getchar();
	printf("*使用手册*：输入信息回车可以聊天，输入#回车可以发送文件  （下一步回车）");
	getchar();
	printf("给自己取个名字吧！（不能超过20个英文字符，一个汉字等于2个英文字符）\n");
	cin >> ID;        //获取用户名

	sockVersion = MAKEWORD(2, 0);
	::WSAStartup(sockVersion, &wsaData);     // 初始化WS2_32.dll
	connectSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);     // 创建套接字
	if (connectSocket == INVALID_SOCKET)    
	{
		printf("出错了（╯＾╰）重启一下吧。（创建套接字失败！）\n");
		::WSACleanup();
	}

	servAddr.sin_family = AF_INET;          //TCP协议 
	servAddr.sin_port = htons(8888);        //端口号   
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");     //IP地址

	while (::connect(connectSocket, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		printf("出错了（╯＾╰）……（连接服务器失败！）\n");
		printf("30秒后重新连接……\n");
		Sleep(30000);
		/*::WSACleanup();*/
	}
	
	printf("开始聊天吧(*^__^*) \n");
	printf("Tips:说中文一次不要超过512个字（包括标点），不然可能会发生很神奇的事情\n");
}


Socket::~Socket()
{
	::closesocket(connectSocket);      // 关闭套接字
	::WSACleanup();       // 释放WS2_32库
}


/* 接受消息函数 */
void Socket::Receive()
{
	PACKAGE_HEAD pckgHead;        //定义包头
	char Head[256] = { 0 };       //包头缓冲区
	char fileName[64] = { 0 };    //全为0的字符数组，用来判断包头内的文件名是否全为0
	char Data[1024] = { 0 };      //数据缓冲区
	int dataLenth = 0;            //接受的数据的长度
	char fileAddr[256] = "D:\\";      //接收的文件保存的地址
	::recv(connectSocket, Head, 256, 0);      //接受包头
	memset(&pckgHead, 0, sizeof(pckgHead));      //清空包头结构体
	memcpy(&pckgHead, Head, sizeof(pckgHead));   //填充包头
	if (strcmp(pckgHead.fileName, fileName) == 0)   //文件名全为0则接收信息，否则接收文件
	{
		dataLenth = ::recv(connectSocket, Data, 1024, 0);        //接受数据，返回实际接受的字节数
		if (dataLenth == pckgHead.msg_len)           //接受数据长度等于包头内的数据长度
		{
			printf("%s:%s\n", pckgHead.ID, Data);    //打印消息
		}
	}
	else
	{
		strcat(fileAddr, pckgHead.fileName);         //把文件名加到保存的地址上，变成文件地址
		//在文件地址以写创建一个文件句柄
		HANDLE hFile = CreateFile((LPCSTR)fileAddr, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD NumberOfBytesRecv = 0;       //接收的数据的字节数
		do{
			NumberOfBytesRecv = ::recv(connectSocket, Data, 1024, 0);         //一次接受收到的字节数
			dataLenth = dataLenth + NumberOfBytesRecv;            //接受到的总字节数
			::WriteFile(hFile, Data, NumberOfBytesRecv, &NumberOfBytesRecv, NULL);      //将数据写入文件地址
		} while (dataLenth != pckgHead.msg_len);            //接受总字节数等于包头内的数据长度时停止接受
		printf("接收到文件，请到D:\\中查看（关闭程序后可以打开文件）\n");
	}
}


void Socket::Send()
{
	PACKAGE_HEAD pckgHead;                //定义包头
	strcpy(pckgHead.ID, ID);              //将ID赋给包头里的ID
	char Head[256] = { 0 };               //包头缓冲区
	char Data[1024] = { 0 };              //数据缓冲区
	char fileName[64] = { 0 };            //文件名缓冲区
	char fileAddr[256] = { 0 };           //存储文件地址
	memset(Data, 0, 1024);    //清空数据缓冲
	cin >> Data;           //输入数据 
	if (Data[0] == '#'&&Data[1] == '\0')             //如果输入#，发文件
	{
		printf("请输入文件所在位置：例：G:\\（注意：冒号是英文）\n");
		cin >> fileAddr;
		printf("请输入文件名：例：file.txt（小于64个字节，一个汉字两个字节）\n");
		cin >> pckgHead.fileName;
		strcat(fileAddr, pckgHead.fileName);    //把文件名加上去，变成文件地址
		//打开已存在的文件，创建文件句柄
		HANDLE hFile = CreateFile((LPCSTR)fileAddr, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		pckgHead.msg_len = GetFileSize(hFile, NULL);    //获得文件的长度
		memset(Head, 0, 256);        //清空包头缓冲区
		memcpy(Head, &pckgHead, sizeof(pckgHead));        //填充包头缓冲区
		if (hFile != INVALID_HANDLE_VALUE)      //文件存在的话发送包头
		{
			::send(connectSocket, Head, sizeof(Head), 0);
			DWORD NumberOfBytesRead = 0;        //读取的文件数据字节数
			do{
				::ReadFile(hFile, Data, sizeof(Data), &NumberOfBytesRead, NULL);
				::send(connectSocket, Data, NumberOfBytesRead, 0);
			} while (NumberOfBytesRead);     //循环读取文件发送文件，直到读取的数据长度为0
		}
		else{
			printf("⊙﹏⊙‖没有找到文件！请重新发送\n");
		}

	}
	else            //发消息
	{
		pckgHead.msg_len = strlen(Data);             //获得消息长度，存入包头
		strcpy(pckgHead.fileName, fileName);         //将包头内的文件名全部置为0
		memset(Head, 0, 256);                        //清空包头缓冲区
		memcpy(Head, &pckgHead, sizeof(pckgHead));   //填充包头缓冲区
		if (::send(connectSocket, Head, sizeof(Head), 0) && ::send(connectSocket, Data, strlen(Data), 0))   //发送包头和消息
		{
			printf("%s:%s\n", pckgHead.ID, Data);          //在自己屏幕上打印出消息
		}
		else{
			printf("发送失败（╯＾╰），换个姿势再来\n");
		}
	}
}

