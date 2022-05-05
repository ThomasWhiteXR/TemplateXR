// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.


#include "PXR_MRCFunctionLibrary.h"
#include "PXR_MRCModule.h"

bool UPicoXRMRCFunctionLibrary::IsMrcEnabled()
{
	if (FPicoXRMRCModule::IsAvailable())
	{
		return FPicoXRMRCModule::Get().IsMrcEnabled();
	}
	return false;
}

bool UPicoXRMRCFunctionLibrary::SetMRCRenderTargetToUnderlying(UTextureRenderTarget2D* RT_MRC_BG, UTextureRenderTarget2D* RT_MRC_FG,int mode)
{
	if (FPicoXRMRCModule::IsAvailable())
	{
		return FPicoXRMRCModule::Get().SetMRCRenderTargetToUnderlying(RT_MRC_BG,RT_MRC_FG,mode);
	}
	return false;
}

bool UPicoXRMRCFunctionLibrary::GetMRCCalibrationData(FPXRTrackedCamera& CameraState)
{
	if (FPicoXRMRCModule::IsAvailable())
	{
		return FPicoXRMRCModule::Get().GetMRCCalibrationData(CameraState);
	}
	return false;
}

bool UPicoXRMRCFunctionLibrary::GetInGameThirdCameraRT(UTextureRenderTarget2D* & Background_RT, UTextureRenderTarget2D*& Forgound_RT)
{
	if (FPicoXRMRCModule::IsAvailable())
	{
		return FPicoXRMRCModule::Get().GetMRCRT(Background_RT,Forgound_RT);
	}
	return false;
}

void UPicoXRMRCFunctionLibrary::SimulateEnableMRC(bool enable)
{
	if (FPicoXRMRCModule::IsAvailable())
	{
		FPicoXRMRCModule::Get().bSimulateEnableMRC=enable;
	}
}

void UPicoXRMRCFunctionLibrary::EnableForeground(bool enable)
{
	if (FPicoXRMRCModule::IsAvailable())
	{
		FPicoXRMRCModule::Get().EnableForeground(enable);
	}
}
