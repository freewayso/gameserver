#ifndef __SERVER_PROCESS_H__
#define __SERVER_PROCESS_H__

bool Process_Setup(int argc, char** argv);
bool Process_Update(int& mainRet);
void Process_Cleanup();
void Process_Stop();

#endif