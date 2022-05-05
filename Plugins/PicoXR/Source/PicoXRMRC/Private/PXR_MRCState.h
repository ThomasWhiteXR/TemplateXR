// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PXR_MRCState.generated.h"

USTRUCT(BlueprintType)
struct FPXRTrackedCamera
{
	GENERATED_USTRUCT_BODY()

	/** The horizontal FOV, in degrees */
	UPROPERTY(BlueprintReadOnly, Category = "PXR|PXRMRC")
		float FieldOfView;

	/** The resolution of the camera frame */
	UPROPERTY(BlueprintReadOnly, Category = "PXR|PXRMRC")
		int32 SizeX;

	/** The resolution of the camera frame */
	UPROPERTY(BlueprintReadOnly, Category = "PXR|PXRMRC")
		int32 SizeY;

	/** The relative pose of the camera to the attached tracking device */
	UPROPERTY(BlueprintReadOnly, Category = "PXR|PXRMRC")
		FRotator CalibratedRotation;

	/** The relative pose of the camera to the attached tracking device */
	UPROPERTY(BlueprintReadOnly, Category = "PXR|PXRMRC")
		FVector CalibratedOffset;

	FPXRTrackedCamera()
		: FieldOfView(90.0f)
		, SizeX(1280)
		, SizeY(720)
		, CalibratedRotation(EForceInit::ForceInitToZero)
		, CalibratedOffset(EForceInit::ForceInitToZero)
	{}
};

UCLASS()
class UPicoXRMRC_State : public UObject
{
	GENERATED_BODY()
	
public:

	UPicoXRMRC_State(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	FPXRTrackedCamera TrackedCamera;

	UPROPERTY()
	FTransform FinalTransform;

};
