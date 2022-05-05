// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.

#include "PXR_DPInputModule.h"
#include "PXR_DPHMD.h"
#include "PXR_DPInput.h"

#define LOCTEXT_NAMESPACE "FPicoXRDPInputModule"

FPicoXRDPInputModule::FPicoXRDPInputModule()
	:InputDevice(nullptr)
{
}

FPicoXRDPInputModule::~FPicoXRDPInputModule()
{
}

void FPicoXRDPInputModule::StartupModule()
{
	IInputDeviceModule::StartupModule();
	FPicoXRDPInput::RegisterKeys();
}

void FPicoXRDPInputModule::ShutdownModule()
{
	IInputDeviceModule::ShutdownModule();
}

TSharedPtr< class IInputDevice > FPicoXRDPInputModule::CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
	InputDevice = MakeShared<FPicoXRDPInput>();
	if (InputDevice)
	{
		InputDevice->SetMessageHandler(InMessageHandler);
	}
	return InputDevice;
}

IMPLEMENT_MODULE(FPicoXRDPInputModule, PicoXRDPInput)
#undef LOCTEXT_NAMESPACE

