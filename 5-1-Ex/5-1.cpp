// 工程vmeminfo
      # include <windows.h>
      # include <iostream>
      # include <shlwapi.h>
      # include <iomanip>
      #include <stdio.h>
      #include <tchar.h>
      # pragma comment(lib, "shlwapi.lib")
      #define DIV 1024
      #define WIDTH 7
      void main()
      {
          // 首先获得系统信息
          SYSTEM_INFO si;
          :: ZeroMemory(&si, sizeof(si) ) ;
          :: GetNativeSystemInfo(&si) ;//GetNativeSystemInfo

          // 使用外壳辅助程序对一些尺寸进行格式化
          TCHAR szPageSize [MAX_PATH] ;
          :: StrFormatByteSize(si.dwPageSize, szPageSize, MAX_PATH) ;
          DWORD64 dwMemSize = (DWORD64) si.lpMaximumApplicationAddress -
                (DWORD64) si.lpMinimumApplicationAddress;
          TCHAR szMemSize [MAX_PATH] ;
          :: StrFormatByteSize(dwMemSize, szMemSize, MAX_PATH) ;

       // 将内存信息显示出来
          std :: cout << "Virtual memory page size: " << szPageSize
                  << std :: endl;
          std :: cout.fill ('0') ;
          std :: cout << "Minimum application address: 0x"
                  << std :: hex << std :: setw(16)
                  << (DWORD64) si.lpMinimumApplicationAddress
                  << std :: endl;
          std :: cout << "Maximum application address: 0x"
                  << std :: hex << std :: setw(16)
                  << (DWORD64) si.lpMaximumApplicationAddress
                  << std :: endl;
          std :: cout << "Total available virtual memory: "
                  << szMemSize << std :: endl ;
  system("pause");
      }

