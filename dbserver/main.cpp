#include "pch.h"
#include "dbprocess.h"
#include "timeutil.h"
#ifdef WIN32
#include "minidump.h"
#endif
#ifndef WIN32
#include <google/profiler.h>
#endif

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
	if(argc < 2)
	{
		printf("require config.xml parameter\n");
		return -1;
	}

#ifndef WIN32
	//ProfilerStart("gate.prof");
	signal(SIGPIPE, SIG_IGN);
#endif

	if(!Log_Init("dbserver", argv[1])) return -1;
	LogInfo("[%s] is start ....", argv[0]);

	signal(SIGINT, signal_handler);

#ifdef WIN32
	SetConsoleCtrlHandler(HandlerRoutine, TRUE);
	SetUnhandledExceptionFilter(TopLevelFilter);
#endif

	if (!Process_Setup(argv[1]))
	{
		LogError("process setup failed!\n");
		return -1;
	}

	bool Runing = true;
	while (Runing)
	{
		Runing = Process_Update();
		TimeUtil::Sleep(1);
	}

	Process_Cleanup();

	LogInfo("[%s] is stopped ....", argv[0]);
	Log_Uninit();

	return 0;
}
