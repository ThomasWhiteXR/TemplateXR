// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.

#include "PXR_MRCCastingCameraActor.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PXR_MRCState.h"
#include "VRNotificationsComponent.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"
#include "Runtime/Launch/Resources/Version.h"
#include "PXR_HMD.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "XRThreadUtils.h"

#if PICO_MRC_SUPPORTED_PLATFORMS
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "PxrApi.h"
#endif

APicoXRMRC_CastingCameraActor::APicoXRMRC_CastingCameraActor(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	,BackgroundRenderTarget(nullptr)
	,bEnableForeground(true)
	,ForegroundMaxDistance(300.f)
	,bHasInitializedInGameCamOnce(false)
	,MRState(nullptr)
	,M_MRC(nullptr)
	,MI_Background(nullptr)
	,MI_Foreground(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	
	ForegroundCaptureActor = NULL;

#if PLATFORM_ANDROID
	BackgroundRenderTarget = NewObject<UTextureRenderTarget2D>();
	BackgroundRenderTarget->RenderTargetFormat = RTF_RGBA8_SRGB;

	ForegroundRenderTarget = NewObject<UTextureRenderTarget2D>();
	ForegroundRenderTarget->RenderTargetFormat = RTF_RGBA8_SRGB;

#endif
}

void APicoXRMRC_CastingCameraActor::InitializeStates(UPicoXRMRC_State* MRStateIn)
{
	MRState = MRStateIn;
}

void APicoXRMRC_CastingCameraActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogMRC, Log, TEXT("In-Game Camera BeginPlay!"));

	FTimerHandle BeginCalibrateHandle;
	GetWorldTimerManager().SetTimer(BeginCalibrateHandle, this, &APicoXRMRC_CastingCameraActor::InitializeInGameCam, 0.1, false);
}

void APicoXRMRC_CastingCameraActor::EndPlay(EEndPlayReason::Type Reason)
{
	DestroyForeroundCaptureActor();
	if (FPicoXRMRCModule::IsAvailable() && FPicoXRMRCModule::Get().GetPicoXRHMD())
	{
		UE_LOG(LogMRC, Warning, TEXT("Destroy MRCLayer ID:%d"), MRCLayerID);
		FPicoXRMRCModule::Get().GetPicoXRHMD()->DestroyLayer(MRCLayerID);
	}
	Super::EndPlay(Reason);
}

void APicoXRMRC_CastingCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bHasInitializedInGameCamOnce)
	{
		UpdateCamMatrixAndDepth();
		
		
		if (bEnableForeground&&!ForegroundCaptureActor)
		{
			SpawnForegroundCaptureActor();
		}
		else if(!bEnableForeground&&ForegroundCaptureActor)
		{
			DestroyForeroundCaptureActor();
			if (!GetCaptureComponent2D()->IsVisible())
			{
				GetCaptureComponent2D()->SetVisibility(true);
			}
		}
		
		if (ForegroundCaptureActor && GetCaptureComponent2D()->IsVisible())
		{
			ForegroundCaptureActor->GetCaptureComponent2D()->TextureTarget = ForegroundRenderTarget;
			GetCaptureComponent2D()->SetVisibility(false);
			ForegroundCaptureActor->GetCaptureComponent2D()->SetVisibility(true);
			
		}
		else if (ForegroundCaptureActor && ForegroundCaptureActor->GetCaptureComponent2D()->IsVisible())
		{
			GetCaptureComponent2D()->TextureTarget = BackgroundRenderTarget;
			ForegroundCaptureActor->GetCaptureComponent2D()->SetVisibility(false);
			GetCaptureComponent2D()->SetVisibility(true);
		}

	}
}

void APicoXRMRC_CastingCameraActor::BeginDestroy()
{
	Super::BeginDestroy();
}

void APicoXRMRC_CastingCameraActor::SpawnForegroundCaptureActor()
{
	if (bEnableForeground&&!ForegroundCaptureActor)
	{
		UE_LOG(LogMRC, Log, TEXT("Begin Spawn forground MRC Capture Actor!"));
		ForegroundCaptureActor = GetWorld()->SpawnActor<ASceneCapture2D>();
#if ENGINE_MINOR_VERSION >=26
		ForegroundCaptureActor->GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
#else
		ForegroundCaptureActor->GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
#if ENGINE_MINOR_VERSION >=25
		ForegroundCaptureActor->GetCaptureComponent2D()->bDisableFlipCopyGLES = true;
#endif
#endif
		ForegroundCaptureActor->GetCaptureComponent2D()->SetVisibility(false);
		ForegroundCaptureActor->GetCaptureComponent2D()->MaxViewDistanceOverride = ForegroundMaxDistance;
		float x = MRState->TrackedCamera.SizeX;
		float y = MRState->TrackedCamera.SizeY;
		ForegroundCaptureActor->GetCaptureComponent2D()->FOVAngle = MRState->TrackedCamera.FieldOfView * (x / y);
		ForegroundCaptureActor->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
		UE_LOG(LogMRC, Log, TEXT("Spawn forground MRC Capture Actor Over!"));
	}
}

void APicoXRMRC_CastingCameraActor::DestroyForeroundCaptureActor()
{
	if (ForegroundCaptureActor)
	{
		UE_LOG(LogMRC, Log, TEXT("Destroy forground MRC Capture Actor!!!"));
		ForegroundCaptureActor->Destroy();
		ForegroundCaptureActor = nullptr;
	}
}

void APicoXRMRC_CastingCameraActor::InitializeInGameCamTransInPlayerSpace()
{
	APlayerController* myController = GetWorld()->GetFirstPlayerController();
	APawn* myPawn = myController->GetPawn();
	TArray<USceneComponent*>ChildComp;
	myPawn->GetRootComponent()->GetChildrenComponents(true, ChildComp);
	USceneComponent* camParent=nullptr;
	for (auto & comp :ChildComp)
	{
		UCameraComponent* cameraComp = Cast<UCameraComponent>(comp);
		if (cameraComp)
		{
			camParent = cameraComp->GetAttachParent();
			break;
		}
	}
	if (camParent)
	{
		UE_LOG(LogMRC, Log, TEXT("Pawn_CamPar_Location:%s,Rotation:%s"), *camParent->GetComponentLocation().ToString(), *camParent->GetComponentRotation().ToString());
		MRState->TrackedCamera.CalibratedOffset = UKismetMathLibrary::TransformLocation(camParent->GetComponentTransform(), MRState->TrackedCamera.CalibratedOffset);
		MRState->TrackedCamera.CalibratedRotation = UKismetMathLibrary::TransformRotation(camParent->GetComponentTransform(), MRState->TrackedCamera.CalibratedRotation);
	}
	else
	{
		UE_LOG(LogMRC, Log, TEXT("Pawn_Location:%s,Rotation:%s"), *myPawn->GetActorLocation().ToString(), *myPawn->GetActorRotation().ToString());
		MRState->TrackedCamera.CalibratedOffset = UKismetMathLibrary::TransformLocation(myPawn->GetActorTransform(), MRState->TrackedCamera.CalibratedOffset);
		MRState->TrackedCamera.CalibratedRotation = UKismetMathLibrary::TransformRotation(myPawn->GetActorTransform(), MRState->TrackedCamera.CalibratedRotation);
	}

	if (FPicoXRMRCModule::Get().PicoXRHMD)
	{
		FQuat currentQuat; FVector currentPose;
		FPicoXRMRCModule::Get().PicoXRHMD->GetCurrentPose(0, currentQuat, currentPose);//TODO:currentPose add to CalibratedOffset.
		MRState->TrackedCamera.CalibratedOffset.Z += currentPose.Z;
#if PLATFORM_ANDROID
		float offsetZ = 0;
		Pxr_GetConfigFloat(PXR_MRC_POSITION_Y_OFFSET,&offsetZ);
		MRState->TrackedCamera.CalibratedOffset.Z -= offsetZ*100.0f;
#endif
		UE_LOG(LogMRC, Log, TEXT("HMD Location Offset:%s,Rotation Offset:%s"), *currentPose.ToString(),*FRotator(currentQuat).ToString());
	}
	/*MRState->TrackedCamera.CalibratedRotation=UKismetMathLibrary::MakeRotationFromAxes(
			UKismetMathLibrary::GetForwardVector(MRState->TrackedCamera.CalibratedRotation)*-1,
			UKismetMathLibrary::GetRightVector(MRState->TrackedCamera.CalibratedRotation)*-1,
			UKismetMathLibrary::GetUpVector(MRState->TrackedCamera.CalibratedRotation)*-1);*/
	UE_LOG(LogMRC, Log, TEXT("AfTrans_Location:%s,Rotation:%s"), *MRState->TrackedCamera.CalibratedOffset.ToString(), *MRState->TrackedCamera.CalibratedRotation.ToString());
	AttachToActor(myPawn, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,true));
	FTransform FinalTransform(MRState->TrackedCamera.CalibratedRotation, MRState->TrackedCamera.CalibratedOffset);
	MRState->FinalTransform = FinalTransform;
	RootComponent->SetWorldTransform(MRState->FinalTransform);
}

void BuildProjectionMatrix(float YMultiplier, float FOV, float FarClipPlane, FMatrix& ProjectionMatrix)
{
	if (FarClipPlane < GNearClippingPlane)
	{
		FarClipPlane = GNearClippingPlane;
	}

	if ((int32)ERHIZBuffer::IsInverted)
	{
		ProjectionMatrix = FReversedZPerspectiveMatrix(
			FOV,
			FOV,
			1.0f,
			YMultiplier,
			GNearClippingPlane,
			FarClipPlane
		);
	}
	else
	{
		ProjectionMatrix = FPerspectiveMatrix(
			FOV,
			FOV,
			1.0f,
			YMultiplier,
			GNearClippingPlane,
			FarClipPlane
		);
	}
}

void APicoXRMRC_CastingCameraActor::UpdateCamMatrixAndDepth()
{
	float Distance = 0.0f;
	FQuat HeadOrientation;
	FVector HeadPosition;
	HeadPosition = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();
	FVector HeadToCamera = HeadPosition - MRState->FinalTransform.GetLocation();
	Distance = FVector::DotProduct(MRState->FinalTransform.GetRotation().GetForwardVector().GetSafeNormal2D(), HeadToCamera);
	ForegroundMaxDistance = FMath::Max(Distance, GMinClipZ);
	if (ForegroundCaptureActor)
	{
		ForegroundCaptureActor->GetCaptureComponent2D()->MaxViewDistanceOverride = ForegroundMaxDistance;
	}

	float FOV = GetCaptureComponent2D()->FOVAngle * (float)PI / 360.0f;

	int ViewWidth = MRState->TrackedCamera.SizeX;
	int ViewHeight = MRState->TrackedCamera.SizeY;
	FIntPoint CameraTargetSize = FIntPoint(ViewWidth, ViewHeight);

	// Use custom projection matrix for far clip plane and to use camera aspect ratio instead of rendertarget aspect ratio
	float YMultiplier = (float)CameraTargetSize.X / (float)CameraTargetSize.Y;
	GetCaptureComponent2D()->bUseCustomProjectionMatrix = true;
	BuildProjectionMatrix(YMultiplier, FOV, GNearClippingPlane, GetCaptureComponent2D()->CustomProjectionMatrix);
	if (ForegroundCaptureActor)
	{
		ForegroundCaptureActor->GetCaptureComponent2D()->bUseCustomProjectionMatrix = true;
		BuildProjectionMatrix(YMultiplier, FOV, ForegroundMaxDistance, ForegroundCaptureActor->GetCaptureComponent2D()->CustomProjectionMatrix);
	}
}


void APicoXRMRC_CastingCameraActor::InitializeRTSize()
{
	int ViewWidth = MRState->TrackedCamera.SizeX;
	int ViewHeight = MRState->TrackedCamera.SizeY;

	FIntPoint CameraTargetSize = FIntPoint(ViewWidth, ViewHeight);

	BackgroundRenderTarget->ResizeTarget(ViewWidth, ViewHeight);
	
	ForegroundRenderTarget->ResizeTarget(ViewWidth, ViewHeight);	

}

void APicoXRMRC_CastingCameraActor::InitializeInGameCam()
{
	FPicoXRMRCModule::Get().GetMRCCalibrationData(MRState->TrackedCamera);

	InitializeInGameCamTransInPlayerSpace();
	
	if (!bHasInitializedInGameCamOnce)
	{
		InitializeRTSize();
	
		// LDR for gamma correction and post process
#if ENGINE_MINOR_VERSION >=26
		GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
#else
		GetCaptureComponent2D()->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
#if ENGINE_MINOR_VERSION >=25
		GetCaptureComponent2D()->bDisableFlipCopyGLES = true;
#endif
#endif
		float x = MRState->TrackedCamera.SizeX;
		float y = MRState->TrackedCamera.SizeY;
		GetCaptureComponent2D()->FOVAngle = MRState->TrackedCamera.FieldOfView * (x / y);
		UE_LOG(LogMRC, Log, TEXT("Final FOV:%f"), GetCaptureComponent2D()->FOVAngle);
	
		SpawnForegroundCaptureActor();
		if (FPicoXRMRCModule::IsAvailable()&& FPicoXRMRCModule::Get().GetPicoXRHMD())
		{
			FTextureRHIRef BG=nullptr, FG=nullptr;
			ExecuteOnRenderThread([this,&BG,&FG]()
				{
					ExecuteOnRHIThread([this,&BG,&FG]()
						{
							BG = BackgroundRenderTarget->Resource->TextureRHI;
							FG = ForegroundRenderTarget->Resource->TextureRHI;
						});
				});
			if (BG && FG)
			{
				UE_LOG(LogMRC, Log, TEXT("BG Texture ID:%d"), *static_cast<int32_t*>(BG->GetNativeResource()));
				UE_LOG(LogMRC, Log, TEXT("FG Texture ID:%d"), *static_cast<int32_t*>(FG->GetNativeResource()));
			}
			
#if PLATFORM_ANDROID
		    MRCLayerID=FPicoXRMRCModule::Get().GetPicoXRHMD()->CreateMRCStereoLayer(BG,FG);		
#endif
		    FPicoXRMRCModule::Get().PicoXRHMD->EventManager->LongHomePressedDelegate.AddDynamic(this, &APicoXRMRC_CastingCameraActor::OnHMDRecentered);
		    UE_LOG(LogMRC, Log, TEXT("LongHomePressedDelegate binded!"));
		    FPicoXRMRCModule::Get().PicoXRHMD->EventManager->ResumeDelegate.AddDynamic(this, &APicoXRMRC_CastingCameraActor::OnHMDResume);
		    UE_LOG(LogMRC, Log, TEXT("HMDResume binded!"));
	    }
	}
	bHasInitializedInGameCamOnce = true;
}

void APicoXRMRC_CastingCameraActor::OnHMDRecentered()
{
	InitializeInGameCam();
	UE_LOG(LogMRC,Log,TEXT("OnHMDRecentered called!"));
}

void APicoXRMRC_CastingCameraActor::OnHMDResume()
{
	FTimerHandle ResumeHandle;
	//Delay in obtaining calibration data
	GetWorldTimerManager().SetTimer(ResumeHandle, this, &APicoXRMRC_CastingCameraActor::InitializeInGameCam, 0.1, false);
	UE_LOG(LogMRC, Log, TEXT("ResumeMRC called!"));
}
