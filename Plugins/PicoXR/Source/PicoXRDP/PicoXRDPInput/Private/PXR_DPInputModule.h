// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.

#pragma once
#include "IPXR_DPInputModule.h"
#include "IInputDevice.h"
#include "Templates/SharedPointer.h"

class FPicoXRDPInputModule : public IPicoXRDPInputModule
{
public:
	FPicoXRDPInputModule();
	virtual ~FPicoXRDPInputModule();
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual TSharedPtr< class IInputDevice > CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override;
private:
	TSharedPtr<class FPicoXRDPInput> InputDevice;
};

