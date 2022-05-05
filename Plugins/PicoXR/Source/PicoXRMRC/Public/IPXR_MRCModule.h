// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.
#pragma once
#include "Modules/ModuleManager.h"

#define PICO_MRC_SUPPORTED_PLATFORMS PLATFORM_ANDROID

class IPicoXRMRCModule : public IModuleInterface
{

public:

	static inline IPicoXRMRCModule& Get()
	{
		return FModuleManager::GetModuleChecked< IPicoXRMRCModule >("PicoXRMRC");
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("PicoXRMRC");
	}
};
