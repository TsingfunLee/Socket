// Server.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <process.h>
#include <iostream>
using namespace std;
#include "..\ExportClass\Socket.h"����//����������ͷ�ļ�

#pragma comment(lib,"..\\debug\\ExportClass.lib")


//�������ݵ��߳�
unsigned int _stdcall RecvThread(LPVOID lpParam);

//�������ݵ��߳�
unsigned int _stdcall SendThread(LPVOID lpParam);


int _tmain(int argc, _TCHAR* argv[])
{
	Socket server;    //����Socket����
	HANDLE h[2];     //�߳̾��

	server.Accept();   //��������

	//�����շ����ݵ������߳�
	h[0] = (HANDLE)::_beginthreadex(NULL, NULL, RecvThread, &server, 0, NULL);
	h[1] = (HANDLE)::_beginthreadex(NULL, NULL, SendThread, &server, 0, NULL);
	
	::WaitForMultipleObjects(2, h, TRUE, INFINITE);
	for (int i = 0; i < 2;i++)
	{
		::CloseHandle(h[i]);
	}

	return 0;
}

//�������ݵ��߳�
unsigned int _stdcall RecvThread(LPVOID lpParam)
{
	Socket s = *(Socket*)lpParam;
	while (TRUE)
	{
		s.Receive();
	}

	return 0;
}

//�������ݵ��߳�
unsigned int _stdcall SendThread(LPVOID lpParam)
{
	Socket s = *(Socket*)lpParam;
	while (TRUE)
	{
		s.Send();
	}

	return 0;
}

