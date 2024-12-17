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
#include <thread>
#include <vector>
#include <string>
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

#define BUFFER_SIZE 2048	//���建�����Ĵ�С��������Ϊ2048�ֽ�
char*	buffer;				//����Ļ������������char��

// ������ļ������޹صĲ���
void perform_unrelated_work() {
    const int count = 10e9; // ģ���ʱ����
    volatile int dummy = 0;
    for (int i = 0; i < count; ++i) {
        dummy += i % 7;
    }
}

void process_files_nobuffer(void (*filter_func)(char*, char*, void (*)(char*)), 
                   const std::vector<std::pair<std::string, std::string>>& file_pairs, 
                   void (*funcs[])(char*)) {
    std::vector<std::thread> threads;
	std::vector<std::thread> threads_func1;
	std::vector<std::thread> threads_func2;
    for (size_t i = 0; i < file_pairs.size(); ++i) {
        threads.emplace_back([filter_func, &file_pairs, funcs, i]() {
            filter_func(const_cast<char*>(file_pairs[i].first.c_str()),
                        const_cast<char*>(file_pairs[i].second.c_str()), funcs[i]);
						perform_unrelated_work();
        });
    }
    for (auto& t : threads) {
        t.join();
    }
}

void process_files_sequen(void (*filter_func)(char*, char*, void (*)(char*)), 
                   const std::vector<std::pair<std::string, std::string>>& file_pairs, 
                   void (*funcs[])(char*)) {
    std::vector<std::thread> threads;
	std::vector<std::thread> threads_func1;
	std::vector<std::thread> threads_func2;
    for (size_t i = 0; i < file_pairs.size(); ++i) {
        threads.emplace_back([filter_func, &file_pairs, funcs, i]() {
            filter_func(const_cast<char*>(file_pairs[i].first.c_str()),
                        const_cast<char*>(file_pairs[i].second.c_str()), funcs[i]);
						perform_unrelated_work();
        });
    }
    for (auto& t : threads) {
        t.join();
    }
}

void process_files_overlp(void (*filter_func)(char*, char*, void (*)(char*)), 
                   const std::vector<std::pair<std::string, std::string>>& file_pairs, 
                   void (*funcs[])(char*)) {
    std::vector<std::thread> threads;
	std::vector<std::thread> threads_func1;
	std::vector<std::thread> threads_func2;
    for (size_t i = 0; i < file_pairs.size(); ++i) {
        threads.emplace_back([filter_func, &file_pairs, funcs, i]() {
            filter_func(const_cast<char*>(file_pairs[i].first.c_str()),
                        const_cast<char*>(file_pairs[i].second.c_str()), funcs[i]);
						perform_unrelated_work();
        });
    }
    for (auto& t : threads) {
        t.join();
    }
}
/*------------------------------------------------------------------*/
//���ļ����ݽ��е�5�ֲ���
//f1  +1
//f2  -1
//f3  *1
//f4  >>
//f5  <<

void f1(char* addr) {
    int count = 888;
    int sum = 0;
        for (size_t i = 0; i < count; i++)
        {
            /* code */
            sum +=i;
        }
        //  *addr = (unsigned char)*addr + 1; 
    }
void f2(char* addr) { 
    
    int count = 888;
    int sum = 0;
        for (size_t i = 0; i < count; i++)
        {
            /* code */
            sum +=i;
        }
		//*addr = (unsigned char)*addr - 1; 
		}
void f3(char* addr) { 
    *addr = (unsigned char)*addr * 1; 
    int count = 888;
    int sum = 0;
        for (size_t i = 0; i < count; i++)
        {
            /* code */
            // sum +=i;
        }}
void f4(char* addr) { 
    // *addr = (unsigned char)*addr >> 1; 
    int count = 888;
    int sum = 0;
        for (size_t i = 0; i < count; i++)
        {
            /* code */
            // sum +=i;
        }}
void f5(char* addr) { 
    // *addr = (unsigned char)*addr << 1; 
    int count = 888;
    int sum = 0;
        for (size_t i = 0; i < count; i++)
        {
            /* code */
            // sum +=i;
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

	// �����ļ��Ժͺ���ָ��
    vector<pair<string, string>> file_pairs_nobuffer = {
        {"source.txt", "nobuffer_1.txt"}, {"source_1.txt", "nobuffer_2.txt"},
        {"source_2.txt", "nobuffer_3.txt"}, {"source_3.txt", "nobuffer_4.txt"},
        {"source_4.txt", "nobuffer_5.txt"}, {"source_5.txt", "nobuffer_6.txt"},
        {"source_6.txt", "nobuffer_7.txt"}, {"source_7.txt", "nobuffer_8.txt"},
        {"source_8.txt", "nobuffer_9.txt"}, {"source_9.txt", "nobuffer_10.txt"}};
    
	vector<pair<string, string>> file_pairs_sequen = {
        {"source.txt", "sequen_1.txt"}, {"source_1.txt", "sequen_2.txt"},
        {"source_2.txt", "sequen_3.txt"}, {"source_3.txt", "sequen_4.txt"},
        {"source_4.txt", "sequen_5.txt"}, {"source_5.txt", "sequen_6.txt"},
        {"source_6.txt", "sequen_7.txt"}, {"source_7.txt", "sequen_8.txt"},
        {"source_8.txt", "sequen_9.txt"}, {"source_9.txt", "sequen_10.txt"}};

	vector<pair<string, string>> file_pairs_overlp = {
        {"source.txt", "overlp_1.txt"}, {"source_1.txt", "overlp_2.txt"},
        {"source_2.txt", "overlp_3.txt"}, {"source_3.txt", "overlp_4.txt"},
        {"source_4.txt", "overlp_5.txt"}, {"source_5.txt", "overlp_6.txt"},
        {"source_6.txt", "overlp_7.txt"}, {"source_7.txt", "overlp_8.txt"},
        {"source_8.txt", "overlp_9.txt"}, {"source_9.txt", "overlp_10.txt"}};
    
    void (*funcs[])(char*) = {f1, f2, f3, f4, f5, f1, f2, f3, f4, f5};




    // ��¼ʱ�䲢�����޻���ģʽ
    cout << "*���ļ����ٻ���ģʽ��������......" << endl;
    DWORD start_time = GetTickCount();
    process_files_nobuffer(filter_nobuffer, file_pairs_nobuffer, funcs);
    DWORD nobuffer_time = GetTickCount() - start_time;
    cout << "���ļ����ٻ���ģʽ����ʱ��" << nobuffer_time << " ms." << endl;

    // ��¼ʱ�䲢�����ļ�����ģʽ
    cout << "*ʹ���ļ����ٻ���ģʽ��������......" << endl;
    start_time = GetTickCount();
    process_files_sequen(filter_sequen, file_pairs_sequen, funcs);
    DWORD sequen_time = GetTickCount() - start_time;
    cout << "ʹ���ļ����ٻ���ģʽ����ʱ��" << sequen_time << " ms." << endl;

    	// ��¼ʱ�䲢�����첽ģʽ
    cout << "*�첽����ģʽ��������......" << endl;
     start_time = GetTickCount();
    process_files_overlp(filter_overlp, file_pairs_overlp, funcs);
    DWORD overlp_time = GetTickCount() - start_time;
    cout << "�첽����ģʽ����ʱ��" << overlp_time << " ms." << endl;

    // ���ƽ��ʱ��Ա�
    cout << "*����ģʽ��ƽ����ʱ���£�" << endl;
    cout << ".��ģʽ���ٻ���ģʽƽ����ʱ��" << nobuffer_time / 10 << " ms." << endl;
    cout << ".ʹ���ļ����ٻ���ģʽƽ����ʱ��" << sequen_time / 10 << " ms." << endl;
    cout << ".�첽����ģʽƽ����ʱ��" << overlp_time / 10 << " ms." << endl;

    system("pause");
    return;

}



/************************THE END********************************/
