// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "IPXR_MRCModule.h"

#if PLATFORM_ANDROID
#include "PxrTypes.h"
#endif // PLATFORM_ANDROID
DEFINE_LOG_CATEGORY_STATIC(LogMRC, Log, All);

const int MRCSupportVersion = 0x2000300;

class UTextureRenderTarget2D;
class FPicoXRHMD;
class UPicoXRMRC_State;
class APicoXRMRC_CastingCameraActor;
struct FPXRTrackedCamera;

struct FRawCameraDataFromXML
{
	float x,y,z,w,px,py,pz,fov,width,height;
	bool refreshed;
	FRawCameraDataFromXML()
	{
		x = y = z = w = px = py = pz =fov= width = height = 0;
		refreshed = false;
	}
};

class FPicoXRMRCModule : public IPicoXRMRCModule
{
public:

	FPicoXRMRCModule();
	~FPicoXRMRCModule();

	static inline FPicoXRMRCModule& Get()
	{
		return FModuleManager::GetModuleChecked< FPicoXRMRCModule >("PicoXRMRC");
	}

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static class FPicoXRHMD* PicoXRHMD;
	static FPicoXRHMD* GetPicoXRHMD();

	UPicoXRMRC_State* GetMRState();

public:
	/** Check whether MRC is enabled by PicoHomeAPP */
	bool IsMrcEnabled();

	/** Pass mrc rt to underlying (Set Mode 0:Set Both,1:Only Set BG,2:Only Set FG)*/
	bool SetMRCRenderTargetToUnderlying(UTextureRenderTarget2D* RT_MRC_BG, UTextureRenderTarget2D* RT_MRC_FG,int mode=0);
	
	/** Obtain calibration data */
	bool GetMRCCalibrationData(FPXRTrackedCamera & CameraState);

	bool GetMRCRT(UTextureRenderTarget2D* &Background_RT,UTextureRenderTarget2D* &Forground_RT);

	void EnableForeground(bool enable);

	bool bSimulateEnableMRC;

private:
	
	bool bInitialized;
	UPicoXRMRC_State* MRCState;
	APicoXRMRC_CastingCameraActor* MRActor;
	UWorld* CurrentWorld;
	FDelegateHandle WorldAddedEventBinding;
	FDelegateHandle WorldDestroyedEventBinding;
	FDelegateHandle WorldLoadEventBinding;
	FRawCameraDataFromXML RawCameraDateFromXML;
#if PLATFORM_ANDROID
    PxrPosef MRCPose;
#endif
	//Only call once at fist enter app
	void InitMixedRealityCapture();

	/** Set up the needed settings and actors for MRC in-game */
	void SetupInGameCapture();
	/** Destroy actors for MRC in-game */
	void CloseInGameCapture();
	/** Reset all the MRC settings and state to the config and default */
	void ResetSettingsAndState();

	void OnWorldCreated(UWorld* NewWorld);
	void OnWorldDestroyed(UWorld* NewWorld);

	bool ReadCameraRawDataFromXML();

#if PLATFORM_ANDROID
	bool bActivated;

	FDelegateHandle InitialWorldAddedEventBinding;
	FDelegateHandle InitialWorldLoadEventBinding;
	FDelegateHandle PreWorldTickEventBinding;

	void ChangeCaptureState();
	void OnWorldTick(UWorld* World, ELevelTick Tick, float Delta);
	void OnInitialWorldCreated(UWorld* NewWorld);
#endif

};
