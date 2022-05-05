// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Engine/SceneCapture2D.h"
#include "PXR_MRCModule.h"
#include "PXR_MRCCastingCameraActor.generated.h"

class UPicoXRMRC_State;
class UTextureRenderTarget2D;
class UMaterial;
class UMaterialInstanceDynamic;

UCLASS(NotPlaceable, NotBlueprintable)
class APicoXRMRC_CastingCameraActor : public ASceneCapture2D
{
	GENERATED_BODY()
public:
	APicoXRMRC_CastingCameraActor(const FObjectInitializer& ObjectInitializer);

	/** Initialize the MRC settings and states */
	void InitializeStates(UPicoXRMRC_State * MRStateIn);

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

	UPROPERTY()
	ASceneCapture2D* ForegroundCaptureActor;

	UPROPERTY(BlueprintReadWrite, Category = "PXR|PXRMRC")
	UTextureRenderTarget2D* BackgroundRenderTarget;

	UPROPERTY(BlueprintReadWrite, Category = "PXR|PXRMRC")
	UTextureRenderTarget2D* ForegroundRenderTarget;

	bool bEnableForeground;
private:
	
	void InitializeInGameCam();
	void InitializeRTSize();
	void InitializeInGameCamTransInPlayerSpace();
	void UpdateCamMatrixAndDepth();
	void SpawnForegroundCaptureActor();
	void DestroyForeroundCaptureActor();

	float ForegroundMaxDistance;
	bool bHasInitializedInGameCamOnce;
	uint32 MRCLayerID;

private:
	UPROPERTY()
	UPicoXRMRC_State* MRState;

	UPROPERTY()
	UMaterial* M_MRC;

	UPROPERTY()
	UMaterialInstanceDynamic * MI_Background;

	UPROPERTY()
	UMaterialInstanceDynamic*  MI_Foreground;
	
	UFUNCTION()
	void OnHMDRecentered();

	UFUNCTION()
	void OnHMDResume();
};
