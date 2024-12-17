#include "windows.h"
#include <conio.h>
#include <stdlib.h>
#include <fstream>
#include <io.h>
#include <string.h>
#include <queue>
#include <stdio.h>
using namespace std;

#define CONSUMER 'C'		//消费者
#define PRODUCER 'P'		//生产者

#define INTE_PER_SEC 1000		//每秒钟中断数目
#define MAX_THREAD_NUM 64		//最大线程数目
#define MAX_FILE_NUM 32			//最大数据文件数目
#define MAX_STR_LEN 32			//字符串长度

#define N 2 //一个buffer最多有2个槽

int buffercount = 0;				//缓冲区数目

queue <int> Buffer;
	
CRITICAL_SECTION CP_Buffer;	//临界区

struct ThreadInfo
{
	int		serial;		//线程序号
	char	entity;		//线程类别
	double	delay;		//线程延迟时间
};

// 定义信号量
HANDLE Mutex;
HANDLE Empty;
HANDLE Full;

void CP_ProducerThread(void *p)
{
	//互斥变量
	Mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "Mutex");

	DWORD wait_for_empty;		//等待互斥变量所有权

	DWORD wait_for_mutex;		//等待互斥变量对缓冲区的所有权

	DWORD m_delay;				//延迟时间
	int m_serial;				//线程序号
								//从参数中获得信息
	m_serial = ((ThreadInfo*)(p))->serial;
	m_delay = (DWORD)(((ThreadInfo*)(p))->delay * INTE_PER_SEC);
	Sleep(m_delay);				//延迟等待

	printf("Producer thread %d sends the producing require.\n", m_serial);
	wait_for_empty = WaitForSingleObject(Empty, INFINITE);
	wait_for_mutex = WaitForSingleObject(Mutex, -1);

	//进入缓冲区
	EnterCriticalSection(&CP_Buffer);

	printf("Producer thread %d is producing.\n", m_serial);//生产过程
	Buffer.push(m_serial);
	LeaveCriticalSection(&CP_Buffer);

	//退出线程
	printf("Producer thread %d finished producing.\n", m_serial);

	ReleaseMutex(Mutex);
	// 函数原型
// 	BOOL ReleaseSemaphore(HANDLE hSemaphore, LONG   lReleaseCount,LPLONG lpPreviousCount );信号量的句柄/要增加的计数值/（可选）指向变量的指针，用于接收信号量释放操作前的计数值
	ReleaseSemaphore(Full, 1, NULL);	//Full信号量+1
}

void CP_ConsumerThread(void *p)
{
	//互斥变量
	Mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "Mutex");

	DWORD wait_for_full;		//等待互斥变量所有权

	DWORD wait_for_mutex;		//等待互斥变量对缓冲区的所有权

	DWORD m_delay;				//延迟时间
	int m_serial;				//线程序号
								//从参数中获得信息
	m_serial = ((ThreadInfo*)(p))->serial;
	m_delay = (DWORD)(((ThreadInfo*)(p))->delay * INTE_PER_SEC);
	Sleep(m_delay);				//延迟等待

	printf("Consumer thread %d sends the consuming require.\n", m_serial);
	wait_for_full = WaitForSingleObject(Full, INFINITE);
	wait_for_mutex = WaitForSingleObject(Mutex, -1);

	//进入缓冲区
	EnterCriticalSection(&CP_Buffer);
	
	printf("Consumer thread %d is consuming %d product.\n", m_serial,Buffer.front());//生产过程

	Buffer.pop();

	LeaveCriticalSection(&CP_Buffer);

	//退出线程
	printf("Consumer thread %d finished consuming.\n", m_serial);

	ReleaseMutex(Mutex);

	ReleaseSemaphore(Empty, 1, NULL);	//Empty信号量+1
}

void ConsumerProducer(char* file)
{
		DWORD n_thread = 0;			//线程数目
	DWORD thread_ID;			//线程ID
	DWORD wait_for_all;			//等待所有线程结束

								//互斥对象
	Mutex = CreateMutex(NULL, FALSE, "Mutex");
	// 参数表 CreateSemaphore(LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCSTR lpName); 
	Empty = CreateSemaphore(NULL, N, N, NULL); //初始值为N最大值为N
	Full = CreateSemaphore(NULL, 0, N, NULL); //初始值为0最大值为N
	
	//线程对象
	HANDLE h_Thread[MAX_THREAD_NUM];
	ThreadInfo thread_info[MAX_THREAD_NUM];


	buffercount = 0;								//初始化readcount
	InitializeCriticalSection(&CP_Buffer);		//初始化临界区
	ifstream  inFile;
	inFile.open(file);							//打开文件
	printf("Consumer Producer Problem:\n\n");
	bool first_in_flag = 1;

// 读取首次的特殊信息
if (first_in_flag && (inFile >> buffercount)) {
    inFile.get();  // 消费掉后面的换行符或其他字符
    // printf("first read\n");
    first_in_flag = 0;  // 确保这段代码不会再次执行
}

// 读取每个线程的信息
while (inFile >> thread_info[n_thread].serial >> thread_info[n_thread].entity >> thread_info[n_thread].delay) {
    inFile.get(); // 处理行尾的换行符
    n_thread++;   // 计数器递增
}

	for (int i = 0; i < (int)(n_thread); i++)
	{
		if (thread_info[i].entity == CONSUMER || thread_info[i].entity == 'c')
		{
			//创建消费者线程
			h_Thread[i] = CreateThread(NULL, 0,
				(LPTHREAD_START_ROUTINE)(CP_ConsumerThread),
				&thread_info[i], 0, &thread_ID);
		}
		else{
			//创建生产者线程
			h_Thread[i] = CreateThread(NULL, 0,
				(LPTHREAD_START_ROUTINE)(CP_ProducerThread),
				&thread_info[i], 0, &thread_ID);
		}
	}

	//等待所有线程结束
	wait_for_all = WaitForMultipleObjects(n_thread, h_Thread, TRUE, -1);
	printf("All reader and writer have finished, operation.\n");
}

////////////////////////////////////////////////////////////////////////////
//主函数

int main(int argc, char* argv[])
{
	char ch;

	while (true)
	{
		//打印提示信息
		printf("*******************************************\n");
		printf("         1:Consumer Producer Problem\n");
		printf("         2:Exit to Windows\n");
		printf("Enter your choice(1 or 2):");
		//如果输入信息不正确，继续输入
		do
		{
			ch = (char)_getch();
		} while (ch != '1' && ch != '2' );

		system("cls");
		//选择2，返回
		if (ch == '2')
			return 0;
		//选择1，生产者消费者问题
		else if (ch == '1')
			ConsumerProducer("thread.dat");
		printf("\nPress Any Key To Continue:");
		_getch();
		system("cls");
	}
	return 0;
}
