#ifndef __DCMCONFIG_H
#define __DCMCONFIG_H

#include "baseconfig.h"
#include "singleton.h"


class DcmConfig : public Config
{

public:
	DcmConfig();
	~DcmConfig();
	DECLARE_SINGLETON(DcmConfig)
public:
	bool ReadSettings(tinyxml2::XMLElement *element);

};




#endif

