// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <Windows.h>
#include <stdio.h>
#include "funct.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		/*printf("\nprocess attach of dll\n");*/
		break;
	case DLL_THREAD_ATTACH:
		/*printf("\nthread attach of dll");*/
		break;
	case DLL_THREAD_DETACH:
		/*printf("\nthread detach of dll");*/
		break;
	case DLL_PROCESS_DETACH:
		/*printf("\nprocess detach of dll\n");*/
		break;
	}
	return TRUE;
}