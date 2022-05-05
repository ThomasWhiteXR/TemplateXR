// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.

#pragma once
#include "Modules/ModuleManager.h"
#include "IInputDeviceModule.h"

class IPicoXRDPInputModule : public IInputDeviceModule
{
public:
	static inline IPicoXRDPInputModule& Get()
	{
		return FModuleManager::LoadModuleChecked< IPicoXRDPInputModule >( "PicoXRDPInput" );
	}

	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded( "PicoXRDPInput" );
	}
};

