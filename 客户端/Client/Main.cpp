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

void main()
{
	Socket client;      //�����׽���
	HANDLE h[2];

	//�����շ����ݵ��߳�	
	h[0] = (HANDLE)::_beginthreadex(NULL, NULL, RecvThread, &client, 0, NULL);
	h[1] = (HANDLE)::_beginthreadex(NULL, NULL, SendThread, &client, 0, NULL);

	WaitForMultipleObjects(2, h, TRUE, INFINITE);
	CloseHandle(h[0]);
	CloseHandle(h[1]);
}

//�������ݵ��߳�
unsigned int _stdcall RecvThread(LPVOID lpParam)
{
	Socket c = *(Socket*)lpParam;
	while (TRUE)
	{
		c.Receive();
	}

	return 0;
}

//�������ݵ��߳�
unsigned int _stdcall SendThread(LPVOID lpParam)
{
	Socket c = *(Socket*)lpParam;
	while (TRUE)
	{
		c.Send();
	}

	return 0;
}