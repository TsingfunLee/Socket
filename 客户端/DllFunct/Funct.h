#ifndef FUNCT_H
#define FUNCT_H
extern "C" int __declspec(dllexport);
//被extern "C"修饰的变量和函数是按照C语言方式编译和连接的
//	__declspec(dllexport)   声明一个导出函数，一般用于dll中
//	__declspec(dllimport)   声明一个导入函数，一般用于使用某个dll的exe中 
#endif