// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PicoXRMRC/Private/PXR_MRCState.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PXR_MRCFunctionLibrary.generated.h"


class UTextureRenderTarget2D;
struct FPXRTrackedCamera;
class UMaterialInstanceDynamic;
/**
 * 
 */
UCLASS()
class PICOXRMRC_API UPicoXRMRCFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    //UFUNCTION(BlueprintCallable,Category="PXR|PXRMRC")
	static bool IsMrcEnabled();

	/** Pass mrc rt to underlying (Mode 0:Both,1:Only BG,2:Only FG)*/
	//UFUNCTION(BlueprintCallable, Category = "PXR|PXRMRC")
	static bool SetMRCRenderTargetToUnderlying(UTextureRenderTarget2D* RT_MRC_BG, UTextureRenderTarget2D* RT_MRC_FG,int mode=0);

	//UFUNCTION(BlueprintCallable, Category = "PXR|PXRMRC")
	static bool GetMRCCalibrationData( FPXRTrackedCamera& CameraState);

	//UFUNCTION(BlueprintCallable,Category= "PXR|PXRMRC")
	static bool GetInGameThirdCameraRT(UTextureRenderTarget2D* &Background_RT, UTextureRenderTarget2D*& Forgound_RT);

	//UFUNCTION(BlueprintCallable, Category = "PXR|PXRMRC")
	static void SimulateEnableMRC(bool enable);

	//UFUNCTION(BlueprintCallable,Category="PXR|PXRMRC")
	static void EnableForeground(bool enable);

};
