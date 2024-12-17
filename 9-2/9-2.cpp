/*****************************************************************
/*
/*****************************************************************
/*-------------------------------------------------------------
/*����ģʽ
/*	1.	FILE_FLAG_NOBUFFER
/*	2.  FILE_FLAG_SEQUENTIAL_SCAN
/*	3.  FILE_FLAG_BUFFERING|FILE_FLAG_OVERLAPPED
/*------------------------------------------------------------
/*���ֲ���
/*	1.	charactor	+1
/*	2.	charactor	-1
/*	3.	charactor	-32
/*	4.	charactor	+32
/*	5.	charactor	*1
/*---------------------------------------------------------------
*/
#include<iostream>
#include<windows.h>
using namespace std;
//����ģʽ
void filter_nobuffer(char* source, char* sink, void(*func)(char* addr));
void filter_sequen(char* source, char* sink, void(*func)(char* addr));
//Overlap �����첽�ص�����
void filter_overlp(char* source, char* sink, void(*func)(char* addr));
//���ֲ�ͬ���ܵĲ���	
void f1(char* addr);
void f2(char* addr);
void f3(char* addr);
void f4(char* addr);
void f5(char* addr);

#define BUFFER_SIZE 1024	//���建�����Ĵ�С��������Ϊ1024�ֽ�
char*	buffer;				//����Ļ������������char��

void main()
{
	//���仺����
	buffer = new char[BUFFER_SIZE];
	//��¼ִ��filter�����Ŀ�ʼʱ��
	DWORD tick;

	//��������ģʽ���Ե�ƽ��ʱ��
	DWORD nobuffer_average_time = 0;
	DWORD sequen_average_time = 0;
	DWORD overlp_average_time = 0;
	//�����޻���ģʽ����filter����10��
	cout << "*���ļ����ٻ���ģʽ��������......" << endl;

	DWORD nobuffer_start_time = GetTickCount();

	tick = nobuffer_start_time;
	filter_nobuffer("source.txt", "nobuffer_1.txt", f1);
	cout << "nobuffer 0-1:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_nobuffer("source_1.txt", "nobuffer_2.txt", f2);
	cout << "nobuffer 1-2:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_nobuffer("source_2.txt", "nobuffer_3.txt", f3);
	cout << "nobuffer 2-3:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_nobuffer("source_3.txt", "nobuffer_4.txt", f4);
	cout << "nobuffer 3-4:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_nobuffer("source_4.txt", "nobuffer_5.txt", f5);
	cout << "nobuffer 4-5:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_nobuffer("source_5.txt", "nobuffer_6.txt", f1);
	cout << "nobuffer 5-6:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_nobuffer("source_6.txt", "nobuffer_7.txt", f2);
	cout << "nobuffer 6-7:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_nobuffer("source_7.txt", "nobuffer_8.txt", f3);
	cout << "nobuffer 7-8:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_nobuffer("source_8.txt", "nobuffer_9.txt", f4);
	cout << "nobuffer 8-9:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_nobuffer("source_9.txt", "nobuffer_10.txt", f5);
	DWORD nobuffer_end_time = GetTickCount();
	cout << "nobuffer 9-10:" << nobuffer_end_time - tick << " ms." << endl;

	//���ø��ٻ���ģʽ����filter����10��
	cout << "*ʹ���ļ����ٻ���ģʽ��������......" << endl;

		DWORD sequen_start_time = GetTickCount();

	tick = sequen_start_time;
	filter_sequen("source.txt", "sequen_1.txt", f1);
	cout << "sequen 0-1:" << GetTickCount() - tick << " ms. " << endl;

	tick = GetTickCount();
	filter_sequen("source_1.txt", "sequen_2.txt", f2);
	cout << "sequen 1-2:" << GetTickCount() - tick << " ms. " << endl;

	tick = GetTickCount();
	filter_sequen("source_2.txt", "sequen_3.txt", f3);
	cout << "sequen 2-3:" << GetTickCount() - tick << " ms. " << endl;

	tick = GetTickCount();
	filter_sequen("source_3.txt", "sequen_4.txt", f4);
	cout << "sequen 3-4:" << GetTickCount() - tick << " ms. " << endl;

	tick = GetTickCount();
	filter_sequen("source_4.txt", "sequen_5.txt", f5);
	cout << "sequen 4-5:" << GetTickCount() - tick << " ms. " << endl;

	tick = GetTickCount();
	filter_sequen("source_5.txt", "sequen_6.txt", f1);
	cout << "sequen 5-6:" << GetTickCount() - tick << " ms. " << endl;

	tick = GetTickCount();
	filter_sequen("source_6.txt", "sequen_7.txt", f2);
	cout << "sequen 6-7:" << GetTickCount() - tick << " ms. " << endl;


	tick = GetTickCount();
	filter_sequen("source_7.txt", "sequen_8.txt", f3);
	cout << "sequen 7-8:" << GetTickCount() - tick << " ms. " << endl;

	tick = GetTickCount();
	filter_sequen("source_8.txt", "sequen_9.txt", f4);
	cout << "sequen 8-9:" << GetTickCount() - tick << " ms. " << endl;

	tick = GetTickCount();
	filter_sequen("source_9.txt", "sequen_10.txt", f5);
	DWORD sequen_end_time = GetTickCount();
	cout << "sequen 9-10:" << sequen_end_time - tick << " ms. " << endl;

	//�����첽ģʽ����filter����10��
	cout << "�첽����ģʽ��������......" << endl;

	DWORD overlp_start_time = GetTickCount();

	tick = overlp_start_time;
	filter_overlp("source.txt", "overlp_1.txt", f1);
	cout << "overlp 0-1:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_overlp("source_1.txt", "overlp_2.txt", f2);
	cout << "overlp 1-2:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_overlp("source_2.txt", "overlp_3.txt", f3);
	cout << "overlp 2-3:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_overlp("source_3.txt", "overlp_4.txt", f4);
	cout << "overlp 3-4:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_overlp("source_4.txt", "overlp_5.txt", f5);
	cout << "overlp 4-5:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_overlp("source_5.txt", "overlp_6.txt", f1);
	cout << "overlp 5-6:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_overlp("source_6.txt", "overlp_7.txt", f2);
	cout << "overlp 6-7:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_overlp("source_7.txt", "overlp_8.txt", f3);
	cout << "overlp 7-8:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_overlp("source_8.txt", "overlp_9.txt", f4);
	cout << "overlp 8-9:" << GetTickCount() - tick << " ms." << endl;

	tick = GetTickCount();
	filter_overlp("source_9.txt", "overlp_10.txt", f5);
	DWORD overlp_end_time = GetTickCount();
	cout << "overlp 9-10:" << overlp_end_time - tick << " ms." << endl;

	//�������ģʽ�µ�ƽ��ʱ�������Ա�
	cout << "*����ģʽ��ƽ����ʱ���£�" << endl;
cout << ".��ģʽ���ٻ���ģʽƽ����ʱ��" << (nobuffer_end_time - nobuffer_start_time) / 10 << " ms." << endl;
cout << ".ʹ���ļ����ٻ���ģʽƽ����ʱ��" << (sequen_end_time - sequen_start_time) / 10 << " ms." << endl;
// 
cout << ".�첽����ģʽƽ����ʱ:" << (overlp_end_time - overlp_start_time) / 10 << " ms." << endl << endl;
getchar();
system("pause");
	return;
}

/*------------------------------------------------------------------*/
//���ļ����ݽ��е�5�ֲ���
//f1  +1
//f2  -1
//f3  *1
//f4  >>
//f5  <<
void f1(char* addr) { *addr = (unsigned char)*addr + 1; 
    int count = 888;
    int sum = 0;
        for (size_t i = 0; i < count; i++)
        {
            /* code */
            sum +=i;
        }
        
    }
void f2(char* addr) { 
    // *addr = (unsigned char)*addr - 1; 
    int count = 888;
    int sum = 0;
        for (size_t i = 0; i < count; i++)
        {
            /* code */
            sum +=i;
        }}
void f3(char* addr) { 
    // *addr = (unsigned char)*addr * 1; 
    int count = 888;
    int sum = 0;
        for (size_t i = 0; i < count; i++)
        {
            /* code */
            sum +=i;
        }}
void f4(char* addr) { 
    // *addr = (unsigned char)*addr >> 1; 
    int count = 888;
    int sum = 0;
        for (size_t i = 0; i < count; i++)
        {
            /* code */
            sum +=i;
        }}
void f5(char* addr) { 
    // *addr = (unsigned char)*addr << 1; 
    int count = 888;
    int sum = 0;
        for (size_t i = 0; i < count; i++)
        {
            /* code */
            sum +=i;
        }}

/*-------------------------------------------------------------*/
//û���ļ����ٻ����filter����
void filter_nobuffer(char* source, char* sink, void(*func)(char* addr))
{
	HANDLE handle_src, handle_dst;	//����ԭ�ļ���Ŀ���ļ��ľ��
	BOOL cycle;						//�����ж�һ���������Ƿ�д��
	DWORD NumberOfBytesRead, NumberOfBytesWrite, index;	//�����ֽ�����д���ֽ���
														//��ԭ�ļ�
                                                        //��Ϊ��OPEN_EXISTING���Կ�ʼʱ���½�source.txt
handle_src = CreateFile(source, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);

	//����Ŀ���ļ�
handle_dst = CreateFile(sink, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);

	if(handle_src == INVALID_HANDLE_VALUE || handle_dst == INVALID_HANDLE_VALUE)
	{
		cout << "CreatFile Invocation Error!" << endl;
		exit(1);
	}
	cycle = TRUE;

	//����cycle�ж��ļ�ʲôʱ�����
	while (cycle)
	{
		//��ԭ�ļ����������뻺����
if (ReadFile(handle_src, buffer, BUFFER_SIZE, &NumberOfBytesRead, NULL) == FALSE)
		{
			cout << "ReadFile Error!" << endl;
			exit(1);
		}

		//��������һ��������ʱ��˵���ﵽ�ļ�ĩβ������ѭ��
		if (NumberOfBytesRead < BUFFER_SIZE)
			cycle = FALSE;
		//���ļ����ݽ��еĲ���
		for (index = 0; index < NumberOfBytesRead; index++)
			func(&buffer[index]);

		//���������е�����д��Ŀ���ļ�	
if (WriteFile(handle_dst, buffer, NumberOfBytesRead, &NumberOfBytesWrite, NULL) == FALSE)
		{
			cout << "WriteFile Error!" << endl;
			exit(1);
		}
	}

	//�ر��ļ����
	CloseHandle(handle_src);
	CloseHandle(handle_dst);
}



/*-------------------------------------------------------------*/

void filter_sequen(char* source, char* sink, void(*func)(char* addr))
{
	HANDLE handle_src, handle_dst;	//����ԭ�ļ���Ŀ���ļ��ľ��
	BOOL cycle;						//�����ж�һ���������Ƿ�д��
	DWORD NumberOfBytesRead, NumberOfBytesWrite, index;	//�����ֽ�����д���ֽ���
														//��ԭ�ļ�
handle_src = CreateFile(source, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	//����Ŀ���ļ�
handle_dst = CreateFile(sink, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

if(handle_src == INVALID_HANDLE_VALUE || handle_dst == INVALID_HANDLE_VALUE)
	{
		cout << "CreatFile Invocation Error!" << endl;
		exit(1);
	}
	cycle = TRUE;

	//����cycle�ж��ļ�ʲôʱ�����
	while (cycle)
	{
		//��ԭ�ļ����������뻺����
if (ReadFile(handle_src, buffer, BUFFER_SIZE, &NumberOfBytesRead, NULL) == FALSE)
		{
			cout << "ReadFile Error!" << endl;
			exit(1);
		}

		//��������һ��������ʱ��˵���ﵽ�ļ�ĩβ������ѭ��
		if (NumberOfBytesRead < BUFFER_SIZE)
			cycle = FALSE;
		//���ļ����ݽ��еĲ���
		for (index = 0; index < NumberOfBytesRead; index++)
			func(&buffer[index]);

		//���������е�����д��Ŀ���ļ�	
if (WriteFile(handle_dst, buffer, NumberOfBytesRead, &NumberOfBytesWrite, NULL) == FALSE)
		{
			cout << "WriteFile Error!" << endl;
			exit(1);
		}
	}

	//�ر��ļ����
	CloseHandle(handle_src);
	CloseHandle(handle_dst);
}


/*-------------------------------------------------------------*/

void filter_overlp(char* source, char* sink, void(*func)(char* addr))
{
	HANDLE handle_src, handle_dst;	//����ԭ�ļ���Ŀ���ļ��ľ��
	BOOL cycle;						//�����ж�һ���������Ƿ�д��
	DWORD NumberOfBytesRead, NumberOfBytesWrite, index, dwError;	//�����ֽ�����д���ֽ���
	OVERLAPPED overlapped;			//overlapped �ṹ
									//��ԭ�ļ�
handle_src = CreateFile(source, GENERIC_READ, NULL, NULL, OPEN_EXISTING,     FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

	//����Ŀ���ļ�
handle_dst = CreateFile(sink, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);

	if(handle_src == INVALID_HANDLE_VALUE || handle_dst == INVALID_HANDLE_VALUE)
	{
		cout << "CreatFile Invocation Error!" << endl;
		exit(1);
	}
	cycle = TRUE;

	//��overlapped�ṹ��ʼ��
	overlapped.hEvent = NULL;
	overlapped.Offset = -BUFFER_SIZE;
	overlapped.OffsetHigh = 0;

	//����cycle�ж��ļ�ʲôʱ�����
	while (cycle)
	{
		//�����ļ���ƫ����
		overlapped.Offset = overlapped.Offset + BUFFER_SIZE;


		//��ԭ�ļ����������뻺����
if (ReadFile(handle_src, buffer, BUFFER_SIZE, &NumberOfBytesRead, &overlapped) == FALSE)
		{
			switch (dwError = GetLastError())
			{
				//���ļ���β
			case ERROR_HANDLE_EOF:
				cycle = FALSE;
				break;
			case ERROR_IO_PENDING:
if (GetOverlappedResult(handle_src, &overlapped, &NumberOfBytesRead, TRUE) == FALSE)
				{
					cout << "GetOverlappedResult Error!" << endl;
					exit(1);
				}
				break;
			default:
				break;
			}
		}

		//������һ����������ʱ��˵���ﵽ�ļ�ĩβ������ѭ��
		if (NumberOfBytesRead < BUFFER_SIZE)
			cycle = FALSE;

		//���ļ����ݽ��еĲ���
		for (index = 0; index < NumberOfBytesRead; index++)
			func(&buffer[index]);

		//���������е�����д��Ŀ���ļ�	
if (WriteFile(handle_dst, buffer, NumberOfBytesRead, &NumberOfBytesWrite, NULL) == FALSE)
		{
			cout << "WriteFile Error!" << endl;
			exit(1);
		}
	}
	
	//�ر��ļ����
	CloseHandle(handle_src);
	CloseHandle(handle_dst);
	
}

/************************THE END********************************/
