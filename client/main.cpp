#include "pch.h"
#include "process.h"
#include "timeutil.h"
#include "cmdline.h"
#ifdef WIN32
#include "minidump.h"
#endif
#ifndef WIN32
#include <google/profiler.h>
#endif
//#include "task/taskmgr.h"

void signal_handler(int sig)
{
	Process_Stop();
}

#ifdef WIN32
BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType)
{
	if (dwCtrlType == CTRL_CLOSE_EVENT)
	{
		Process_Stop();
		TimeUtil::Sleep(1000000000);
		return TRUE;
	}

	return FALSE;
}
#endif

int main(int argc, char **argv)
{


#ifndef WIN32
	//ProfilerStart("gate.prof");
	signal(SIGPIPE, SIG_IGN);
#endif

	if(!Log_Init("robot", argv[1])) return -1;
	LogDebug("[%s] is start ....", argv[0]);

	signal(SIGINT, signal_handler);

#ifdef WIN32
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	SetUnhandledExceptionFilter(TopLevelFilter);
#endif

	CmdLine::Setup();
	if (!Process_Setup(argc, argv))
	{
		LogError("process setup failed!\n");
		return -1;
	}

	int mainRet = 0;
	bool Runing = true;
	while (Runing)
	{
		Runing = Process_Update(mainRet);
		CmdLine::Run();
		TimeUtil::Sleep(1);
	}

	CmdLine::Stop();
	Process_Cleanup();

	LogDebug("[%s] is stopped ....", argv[0]);
	LogInfo("return %d", mainRet);
	Log_Uninit();
	return mainRet;
}
