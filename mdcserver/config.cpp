#include "pch.h"
#include "config.h"
#include "network/halllink.h"
#include "network/dclink.h"


INSTANCE_SINGLETON(DcmConfig)

DcmConfig::DcmConfig()
{
	SetServerTag("MdcServer");
	HallLink::CreateInstance();
	AddLinkHandler(HallLink::Instance());
	DCLink::CreateInstance();
	AddLinkHandler(DCLink::Instance());

}


DcmConfig::~DcmConfig()
{
	DCLink::DestroyInstance();
	HallLink::DestroyInstance();
}


bool DcmConfig::ReadSettings(tinyxml2::XMLElement *element)
{

	return true;
}