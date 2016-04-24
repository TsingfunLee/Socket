#include "stdafx.h"
#include <process.h>
#include <iostream>
using namespace std;

#include "..\ExportClass\Socket.h"　　//包含类声明头文件

#pragma comment(lib,"..\\debug\\ExportClass.lib")


//发送数据的线程
unsigned int _stdcall RecvThread(LPVOID lpParam);
//发送数据的线程
unsigned int _stdcall SendThread(LPVOID lpParam);

void main()
{
	Socket client;      //创建套接字
	HANDLE h[2];

	//创建收发数据的线程	
	h[0] = (HANDLE)::_beginthreadex(NULL, NULL, RecvThread, &client, 0, NULL);
	h[1] = (HANDLE)::_beginthreadex(NULL, NULL, SendThread, &client, 0, NULL);

	WaitForMultipleObjects(2, h, TRUE, INFINITE);
	CloseHandle(h[0]);
	CloseHandle(h[1]);
}

//接收数据的线程
unsigned int _stdcall RecvThread(LPVOID lpParam)
{
	Socket c = *(Socket*)lpParam;
	while (TRUE)
	{
		c.Receive();
	}

	return 0;
}

//发送数据的线程
unsigned int _stdcall SendThread(LPVOID lpParam)
{
	Socket c = *(Socket*)lpParam;
	while (TRUE)
	{
		c.Send();
	}

	return 0;
}