#include <windows.h>
#include <iostream>
#include <winioctl.h>
#include <string.h>
using namespace std;
struct Disk		//����Disk�ṹ�Ķ���
{
	HANDLE handle;
	DISK_GEOMETRY disk_info;
};

Disk disk;
HANDLE Floppy;
static _int64 sector;
bool flag;
Disk physicDisk(char driverLetter);										//��������
bool sectorRead(Disk theDisk, unsigned logicSectorNumber, char* buffer);	//��������
void sectorDump(Disk theDisk, int logicSectorNumber);					//��������


Disk physicDisk(char driverLetter)		//�������̾�������򿪴��̣�����disk
{
	flag = true;
	DISK_GEOMETRY* temp = new DISK_GEOMETRY;
	char device[9] = "\\\\.\\C:";
	// char device[9] = "\\\\.\\D:";

	device[4] = driverLetter;
	Floppy = CreateFileA(device,							//��Ҫ�򿪵���������
		GENERIC_READ,					//��ȡ��Ȩ��
		FILE_SHARE_READ | FILE_SHARE_WRITE,//�����Ȩ��
		NULL,							//Ĭ�ϵ�����λ
		OPEN_EXISTING,					//������������ʽ
		0,								//������������������
		NULL);							//ָ��ģ���ļ�������
	// cout<<Floppy;
	if (GetLastError() == ERROR_ALREADY_EXISTS)	//��ʧ�ܣ����ش������
	{	
		cout << "���ܴ򿪴���" << endl;
		cout << GetLastError() << endl;
		flag = false;
		return disk;
	}
	DWORD bytereturned;
	BOOL Result;
	disk.handle = Floppy;
	//���������̵ļ�����Ϣ
	Result = DeviceIoControl(Floppy,
		IOCTL_DISK_GET_DRIVE_GEOMETRY,
		NULL,
		0,
		temp,
		sizeof(*temp),
		&bytereturned,
		(LPOVERLAPPED)NULL);

	if (!Result)				//���ʧ�ܣ����ش������
	{
		cout << "��ʧ��" << endl;
		cout << "�������λ:" << GetLastError() << endl;
		flag = false;
		return disk;
	}
	disk.disk_info = *temp;
	//�����������������̵���Ϣ
	cout << "������Ϊ:" << (unsigned long)disk.disk_info.Cylinders.QuadPart << endl << endl;
	cout << "ÿ����Ĵŵ���Ϊ:" << disk.disk_info.TracksPerCylinder << endl << endl;
	cout << "û�ŵ���������Ϊ:" << disk.disk_info.SectorsPerTrack << endl << endl;
	cout << "ÿ�������ֽ���Ϊ:" << disk.disk_info.BytesPerSector << endl << endl;
	sector = disk.disk_info.Cylinders.QuadPart* (disk.disk_info.TracksPerCylinder)*
		(disk.disk_info.SectorsPerTrack);
	double DiskSize = (double)disk.disk_info.Cylinders.QuadPart*	//��˻�ô��̴�С
		(disk.disk_info.TracksPerCylinder)*
		(disk.disk_info.SectorsPerTrack)*
		(disk.disk_info.BytesPerSector);

	cout << "���̴�СΪ:" << DiskSize / (1024 * 1024) << "MB." << endl;
	cout << driverLetter << "�����ڵĴ��̹���" << (long)sector << "������" << endl;
	delete temp;
	return disk;
}

void sectorDump(Disk theDisk, int logicSectorNumber)	//������ɹ���������Ľ��
{
	BOOL sDresult;
	char* buffer = new char[theDisk.disk_info.BytesPerSector];
	sDresult = sectorRead(disk, logicSectorNumber, buffer);
	if (sDresult)
	{
		for (int i = 0; i<int(theDisk.disk_info.BytesPerSector); i++)
			cout << buffer[i];
		cout << endl;			//����
		cout << endl;
		cout << endl;
	}
	delete buffer;
}

bool sectorRead(Disk theDisk, unsigned logicSectorNumber, char* buffer) //��ָ��������
{
	HANDLE handle1 = theDisk.handle;
	BOOL sDresult, Rresult;
	DWORD numberofread;
	//��λ��������λ��
	sDresult = SetFilePointer(handle1,
		theDisk.disk_info.BytesPerSector* logicSectorNumber,
		NULL,
		FILE_BEGIN);
	//�������������ض��Ľ��
	Rresult = ReadFile(handle1,
		buffer,
		theDisk.disk_info.BytesPerSector,
		&numberofread,
		NULL);
	if (Rresult)		//����������ɹ�
		return true;
	else			//���ʧ��,���ش������
	{
		cout << "��ָ��������ʧ��" << endl;
		cout << "�������Ϊ:" << GetLastError() << endl;
		return false;
	}
}


void main(void)
{
	int secNum;
	char choise;
	disk = physicDisk('c');		//ѡ��Ҫ�鿴�Ĵ���
		// disk = physicDisk('d');		//ѡ��Ҫ�鿴�Ĵ���

	if (flag)
	{
		while (1)
		{
			cout << "********************************************" << endl;
			cout << "*    0. �鿴ָ��Ŀ¼��Ϣ:					*" << endl;
			cout << "*    1. �˳�:							*" << endl;
			cin >> choise;
			switch (choise)
			{
			case '0':		//�鿴ָ��������
				cout << "�ܹ���" << (long)sector << "������!" << endl;
				cout << "������Ҫ�鿴����������:" << endl;
				cin >> secNum;
				if (secNum<sector && secNum >= 0)
					sectorDump(disk, secNum);
				else
					cout << "û���������!!!" << endl;
				//exit
				break;

			case '1':
				exit(0);
				break;
			default:
				cout << "�������!!!����������!!!" << endl;
				cout << endl;
			}
		}
	}


    system("pause");
    return;
}
