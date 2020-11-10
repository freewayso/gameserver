#ifndef __HALLPCH_H__
#define __HALLPCH_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>


#ifdef WIN32
//#include <Windows.h>
#include <winsock2.h>
#define WIN32_LEAN_AND_MEAN
#else
#include <arpa/inet.h>
#endif

#include "define.h"
#include "LogInit.h"
#include "singleton.h"
#include "timer.h"
#include "timermgr.h"

#include "pb/enum.pb.h"
#define ForeachCall(S, Fun, ...) do{ for(auto j = S.begin(); j != S.end(); ++j) {(*j)->Fun( __VA_ARGS__);}}while(0)




#endif