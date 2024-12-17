
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
// Use to convert bytes
#define DIV 1024
#define WIDTH 7
void _tmain()
{
  MEMORYSTATUSEX statex;
  statex.dwLength = sizeof (statex);
  GlobalMemoryStatusEx (&statex);
      //%*I64d
  _tprintf (TEXT("There are %*I64d total GB of physical memory.\n"),
            WIDTH, statex.ullTotalPhys/DIV/DIV/DIV);
  _tprintf (TEXT("There are %*I64d free  GB of physical memory.\n"),
            WIDTH, statex.ullAvailPhys/DIV/DIV/DIV);
  _tprintf (TEXT("There are %*I64d total MB of paging file.\n"),
            WIDTH, statex.ullTotalPageFile/DIV/DIV);
  _tprintf (TEXT("There are %*I64d free  MB of paging file.\n"),
            WIDTH, statex.ullAvailPageFile/DIV/DIV);
  _tprintf (TEXT("There are %*I64d total TB of virtual memory.\n"),
            WIDTH, statex.ullTotalVirtual/DIV/DIV/DIV/DIV);
  _tprintf (TEXT("There are %*I64d free  TB of virtual memory.\n"),
            WIDTH, statex.ullAvailVirtual/DIV/DIV/DIV/DIV);
      system("pause");
}