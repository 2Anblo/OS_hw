 // procterm项目
  #define _CRT_SECURE_NO_WARNINGS
      # include <windows.h>
      # include <iostream>
      # include <stdio.h>
      static LPCTSTR g_szMutexName = "w2kdg.ProcTerm.mutex.Suicide" ;
      HANDLE hParentProcess = NULL;

      DWORD P_Pid;
       // 创建当前进程的克隆进程的简单方法
      void StartClone()
      {
          // 提取当前可执行文件的文件名
          TCHAR szFilename [MAX_PATH] ;
          :: GetModuleFileName(NULL, szFilename, MAX_PATH) ;
           // 格式化用于子进程的命令行，指明它是一个EXE文件和子进程
          TCHAR szCmdLine[MAX_PATH] ;
          :: sprintf(reinterpret_cast < char* > (szCmdLine), "\"%s\" child" , szFilename) ;

           // 子进程的启动信息结构
          STARTUPINFO si;
          :: ZeroMemory(reinterpret_cast < void* > (&si) , sizeof(si) ) ;
si.cb = sizeof(si) ;				// 应当是此结构的大小
           // 返回的用于子进程的进程信息
          PROCESS_INFORMATION pi;
           // 用同样的可执行文件名和命令行创建进程，并指明它是一个子进程
          BOOL bCreateOK = :: CreateProcess(
              szFilename,					// 产生的应用程序名称 (本EXE文件)
              szCmdLine,					// 告诉我们这是一个子进程的标志
              NULL,						// 用于进程的缺省的安全性
              NULL,						// 用于线程的缺省安全性
              FALSE,						// 不继承句柄
              CREATE_NEW_CONSOLE,		// 创建新窗口，使输出更直观
              NULL,						// 新环境
              NULL,						// 当前目录
              &si,							// 启动信息结构
              &pi ) ;						// 返回的进程信息
         // 释放指向子进程的引用
          if (bCreateOK)
          {
                hParentProcess = GetCurrentProcess();

              :: CloseHandle(pi.hProcess) ;
              :: CloseHandle(pi.hThread) ;
          }
      }


void PrintProcessTimes(HANDLE process) {
    FILETIME creationTime, exitTime, kernelTime, userTime;
    if (GetProcessTimes(process, &creationTime, &exitTime, &kernelTime, &userTime)) {
        ULONGLONG cTime = (((ULONGLONG)creationTime.dwHighDateTime) << 32) | creationTime.dwLowDateTime;
        ULONGLONG eTime = (((ULONGLONG)exitTime.dwHighDateTime) << 32) | exitTime.dwLowDateTime;
        double elapsedSeconds = (eTime - cTime) / 10000000.0;

        std::cout << "Process lifetime: " << elapsedSeconds << " seconds" << std::endl;
    } else {
        std::cerr << "Failed to get process times." << std::endl;
    }
}

      void Parent()
      {
          // 创建"自杀"互斥程序体
          P_Pid = GetCurrentProcessId();
            // hParentProcess = GetCurrentProcess();

          HANDLE hMutexSuicide = :: CreateMutex(
              NULL,					// 缺省的安全性
              TRUE,					// 最初拥有的
              g_szMutexName) ;			// 为其命名
          if (hMutexSuicide != NULL)
          {
              // 创建子进程
              std :: cout << "Creating the child process." << std :: endl;
              :: StartClone() ;
               // 暂停
              ::Sleep(2500) ;
               // 指令子进程"杀"掉自身
      std :: cout << "Telling the child process to quit. " << std :: endl;
                    // ::Sleep(5000) ;
              :: ReleaseMutex(hMutexSuicide) ;



	HANDLE     processHandle = hMutexSuicide;
	DWORD      currentProcessId = GetProcessId(processHandle);
	FILETIME createTime, exitTime, kernelTime, userTime;
	// 获取当前进程的PID
	DWORD pid = GetCurrentProcessId();
	printf("pid: %d\t currentProcessId: %d\n", pid, currentProcessId);

	GetProcessTimes(processHandle, &createTime, &exitTime, &kernelTime, &userTime);
	printf("processHandle: %lu\t currentProcessId: %d\n", HandleToULong(processHandle), currentProcessId);
	printf("Create time: %lu\t       %lu\nExit Time: %lu\t     %lu\nKernel time: %lu\t       %lu\nUser time: %lu\t    %lu\n",
		createTime.dwLowDateTime, createTime.dwHighDateTime,
		exitTime.dwLowDateTime, exitTime.dwHighDateTime,
		kernelTime.dwLowDateTime, kernelTime.dwHighDateTime,
		userTime.dwLowDateTime, userTime.dwHighDateTime);

               // 消除句柄
              :: CloseHandle(hMutexSuicide) ;

          }
}
void Child()
{
          // 打开"自杀"互斥体
          HANDLE hMutexSuicide = :: OpenMutex(
              SYNCHRONIZE,			// 打开用于同步
              FALSE,					// 不需要向下传递
              g_szMutexName) ;			// 名称
          if (hMutexSuicide != NULL)
          {
              // 报告正在等待指令
              std :: cout << "Child waiting for suicide instructions. " << std :: endl;
              :: WaitForSingleObject(hMutexSuicide, INFINITE) ;
 
              // 准备好终止，清除句柄
              std :: cout << "Child quiting. " << std :: endl;

                 // 获取父进程的句柄（假设父进程ID已知）
        DWORD parentPid = P_Pid;  
 

 if (hParentProcess != NULL) {
            DWORD exitCode;
            if (GetExitCodeProcess(hParentProcess, &exitCode)) {
                std::cout << "Parent process exit code: " << exitCode << std::endl;
                if (exitCode == STILL_ACTIVE) {
                    std::cout << "Parent process is active. Terminating..." << std::endl;
                    TerminateProcess(hParentProcess, 0);  // 终止父进程
                    std::cout << "Parent process terminated." << std::endl;
                }
            } else {
                std::cerr << "Failed to get parent process exit code." << std::endl;
            }
            PrintProcessTimes(hParentProcess);  // 打印父进程时间信息
            ::CloseHandle(hParentProcess);
        } else {
            std::cerr << "Failed to open parent process." << std::endl;
        }

        ::CloseHandle(hMutexSuicide);

    std::cout << "Child process exiting." << std::endl;
    Sleep(2500);
    ExitProcess(0);  // 确保子进程正确退出
}
}
int main(int argc, char* argv[] )
{
          // 决定其行为是父进程还是子进程
          if (argc > 1 && :: strcmp(argv[1] , "child" ) == 0)
          {
              Child() ;
          }
          else
          {
              Parent() ;
          }
  
  system("pause");

          return 0;
}
