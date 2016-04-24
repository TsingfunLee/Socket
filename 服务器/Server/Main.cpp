// Server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <process.h>
#include <iostream>
using namespace std;
#include "..\ExportClass\Socket.h"　　//包含类声明头文件

#pragma comment(lib,"..\\debug\\ExportClass.lib")


//接收数据的线程
unsigned int _stdcall RecvThread(LPVOID lpParam);

//发送数据的线程
unsigned int _stdcall SendThread(LPVOID lpParam);


int _tmain(int argc, _TCHAR* argv[])
{
	Socket server;    //创建Socket对象
	HANDLE h[2];     //线程句柄

	server.Accept();   //接受连接

	//创建收发数据的两个线程
	h[0] = (HANDLE)::_beginthreadex(NULL, NULL, RecvThread, &server, 0, NULL);
	h[1] = (HANDLE)::_beginthreadex(NULL, NULL, SendThread, &server, 0, NULL);
	
	::WaitForMultipleObjects(2, h, TRUE, INFINITE);
	for (int i = 0; i < 2;i++)
	{
		::CloseHandle(h[i]);
	}

	return 0;
}

//接收数据的线程
unsigned int _stdcall RecvThread(LPVOID lpParam)
{
	Socket s = *(Socket*)lpParam;
	while (TRUE)
	{
		s.Receive();
	}

	return 0;
}

//发送数据的线程
unsigned int _stdcall SendThread(LPVOID lpParam)
{
	Socket s = *(Socket*)lpParam;
	while (TRUE)
	{
		s.Send();
	}

	return 0;
}

