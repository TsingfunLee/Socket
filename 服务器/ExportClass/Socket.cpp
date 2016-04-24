#ifndef DLL_FILE
    #define DLL_FILE
#endif
#include "stdafx.h"
#include "Socket.h"


Socket::Socket()
{
	printf("*聊天程序服务端*  （下一步回车）");
	getchar();
	printf("*操作指南：输入信息回车可以聊天，输入#回车可以发送文件*  （下一步回车）");
	getchar();
	printf("输入服务器ID：例：1号服务器（20个字节以内，一个汉字两个字节）\n");
	cin >> ID;

	/* 创建套接字 */
	sockVersion = MAKEWORD(2, 0);
	::WSAStartup(sockVersion, &wsaData);
	listenSocket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET){
		printf("创建套接字失败！\n");
		::WSACleanup();
	}

	/* 设置端口和地址 */
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8888);
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	/* 绑定端口 */
	if (::bind(listenSocket, (LPSOCKADDR)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR){
		printf("Failed bind() \n");
		::WSACleanup();
	}
	
	/* 监听 */
	if (::listen(listenSocket, 3) == SOCKET_ERROR){
		printf("Failed listen()");
		::WSACleanup();
	}else{
		printf("listen on %d\n", 8888);   //监听成功输出  listen on 8888
	}
}


Socket::~Socket()
{
	//关闭接受发送套接字
	::closesocket(connectSocket);
	// 关闭监听套节字
	::closesocket(listenSocket);
	// 释放WS2_32库
	::WSACleanup();
}


/* 接受连接 */
void Socket::Accept()
{
	int nAddrLen = sizeof(clientAddr);          //客户端地址长度
	
	// 接受一个新连接,新建套接字cs
	connectSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &nAddrLen);
	
	// 接受连接成功打印连接的地址
	if (connectSocket != INVALID_SOCKET){
		printf("Receive a link:%s Port:%d\n连接客户端成功\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
	}else{
		printf("Failed accept()");
	}
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
	if (strcmp(pckgHead.fileName,fileName)==0)   //文件名全为0则接收信息，否则接收文件
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
		printf("接收到文件，请在D:\\中查看（关闭程序后可以打开文件）\n");
	}
}

/* 发送消息函数 */
void Socket::Send()
{
	PACKAGE_HEAD pckgHead;                //定义包头
	strcpy(pckgHead.ID, ID);              //将ID赋给包头里的ID
	char Head[256] = { 0 };               //包头缓冲区
	char Data[1024] = { 0 };              //数据缓冲区
	char fileName[64] = { 0 };            //文件名缓冲区
	char fileAddr[256] = { 0 };           //存储文件地址
	memset(Data, 0, 1024);
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
			printf("找不到文件！\n");
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
			printf("%s:%s\n", pckgHead.ID, Data);           //在自己屏幕上打印出消息
		}
		else{
			printf("发送失败！\n");
		}
	}
}


