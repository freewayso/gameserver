#ifndef __H_DCMSERVER_PROCESS_H__
#define __H_DCMSERVER_PROCESS_H__

bool Process_Setup(const char *file);
bool Process_Update();
void Process_Cleanup();
void Process_Stop();

#endif