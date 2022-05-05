// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.

#include "PXR_Splash.h"
#include "PXR_HMD.h"
#include "Misc/ScopeLock.h"
#include "RenderingThread.h"
#include "Engine/Classes/Kismet//StereoLayerFunctionLibrary.h"
#include "Runtime/HeadMountedDisplay/Public/XRThreadUtils.h"
#include "PXR_HMDFunctionLibrary.h"

FPicoXRSplash::FPicoXRSplash( FPicoXRHMD* InPicoXRHMD)
	:bInitialized(false)
	,bIsShown(false)
	,PicoXRHMD(InPicoXRHMD)
	,bTickerStarted(false)
	,bFirtInGame(true)
{
}

FPicoXRSplash::~FPicoXRSplash()
{
	Shutdown();
	check(!Ticker.IsValid());	
}

void FPicoXRSplash::Tick_RenderThread(float DeltaTime)
{
	check(IsInRenderingThread())
	const double TimeInSeconds = FPlatformTime::Seconds();
	const double DeltaTimeInSeconds = TimeInSeconds - LastTimeInSeconds;

	if (DeltaTimeInSeconds > 2.f * SystemDisplayInterval && Layers_RenderThread_DeltaRotation.Num() > 0)
	{
		FScopeLock ScopeLock(&RenderThreadLock);
		//TODO:Splash rotation
		LastTimeInSeconds = TimeInSeconds;
	}

	RenderFrame_RenderThread(FRHICommandListExecutor::GetImmediateCommandList());
}

void FPicoXRSplash::Startup()
{
	check(IsInGameThread());
	if (!bInitialized)
	{
		Frame = PicoXRHMD->CreateNewGameFrame();

		SystemDisplayInterval = 1.0f / UPicoXRHMDFunctionLibrary::PXR_GetCurrentDisplayFrequency();
		LoadSettings();
		bInitialized = true;
	}
}

void FPicoXRSplash::LoadSettings()
{
	PicoSettings = GetMutableDefault<UPicoXRSettings>();
	check(PicoSettings);
	ClearSplashes();
	for(const FPicoSplashDesc& SplashDesc : PicoSettings->SplashDescs)
	{
		if (SplashDesc.TexturePath !=nullptr)
		{
			AddSplash(SplashDesc);
		}
		else
		{
			UE_LOG(LogHMD, Warning, TEXT("Pxr_UE FPicoXRSplash::LoadSettings()  the TexturePath is null"));
		}		
	}

	UStereoLayerFunctionLibrary::EnableAutoLoadingSplashScreen(PicoSettings->bSplashScreenAutoShow);
	if (PicoSettings->bSplashScreenAutoShow)
	{
		if (!LoadLevelDelegate.IsValid())
		{
			LoadLevelDelegate = FCoreUObjectDelegates::PreLoadMap.AddSP(this, &FPicoXRSplash::OnPreLoadMap);
		}
	}
	else
	{
		if (LoadLevelDelegate.IsValid())
		{
			FCoreUObjectDelegates::PreLoadMap.Remove(LoadLevelDelegate);
			LoadLevelDelegate.Reset();
		}
	}


}

void FPicoXRSplash::Shutdown()
{
	check(IsInGameThread());
	if (bInitialized)
	{
		ExecuteOnRenderThread([this]()
		{
			if (Ticker)
			{
				Ticker->Unregister();
				Ticker = nullptr;
			}
			ExecuteOnRHIThread([this]()
            {
                SplashLayers.Reset();
            });
		});
		SplashLayersMap.Reset();
		bInitialized = false;
	}
}

void FPicoXRSplash::OnPreLoadMap(const FString&)
{
	if (PicoSettings->bSplashScreenAutoShow && !bFirtInGame)
	{
		if (!bIsShown)
		{
			Show();
		}	
	}	
	bFirtInGame = false;
}

void FPicoXRSplash::StartTicker()
{
	check(IsInGameThread());
	if (!Ticker.IsValid())
	{
		Ticker = MakeShareable(new FTicker(this));
		ExecuteOnRenderThread([this]()
        {
			LastTimeInSeconds = FPlatformTime::Seconds();
            Ticker->Register();		
			bTickerStarted = true;
			UE_LOG(LogTemp, Warning, TEXT("Splash StartTicker!"));
        });	
	}
}

void FPicoXRSplash::StopTicker()
{
	ExecuteOnRenderThread([this]()
	{
		if (Ticker.IsValid())
		{
            Ticker->Unregister();
            Ticker = nullptr;
			bTickerStarted = false;
			bIsShown = false;
			UE_LOG(LogTemp, Warning, TEXT("Splash StopTicker!"));
        }
	});	
	UnloadTextures();
}

void FPicoXRSplash::Show()
{
	check(IsInGameThread());
	UnloadTextures();
	{
		for (int32 SplashLayerIndex = 0; SplashLayerIndex < SplashLayers.Num(); ++SplashLayerIndex)
		{
			FPicoSplashLayer& SplashLayer =  SplashLayers[SplashLayerIndex];;
			if (SplashLayer.Desc.TexturePath.IsValid())
			{
				LoadTexture(SplashLayer);
			}
			if (SplashLayer.Desc.LoadingTexture && SplashLayer.Desc.LoadingTexture->IsValidLowLevel())
			{
				SplashLayer.Desc.LoadingTexture->UpdateResource();
			}
		}
		FlushRenderingCommands();
	}
	for (int32 SplashLayerIndex = 0; SplashLayerIndex < SplashLayers.Num(); ++SplashLayerIndex)
	{
		FPicoSplashLayer& SplashLayer = SplashLayers[SplashLayerIndex];
		if (SplashLayer.Desc.LoadingTexture->IsValidLowLevel())
		{
			if (SplashLayer.Desc.LoadingTexture->Resource && SplashLayer.Desc.LoadingTexture->Resource->TextureRHI)
			{
				SplashLayer.Desc.LoadedTexture = SplashLayer.Desc.LoadingTexture->Resource->TextureRHI;
			}
			else
			{
				UE_LOG(LogHMD, Warning, TEXT("Splash, %s - no Resource"), *SplashLayer.Desc.LoadingTexture->GetDesc());
			}
		}
		
		if (SplashLayer.Desc.LoadedTexture)
		{
			const int32 LayerID = PicoXRHMD->NextLayerId++;
			SplashLayer.Layer = MakeShareable(new FPicoXRStereoLayer(PicoXRHMD,LayerID,StereoLayerDescFromPicoSplashDesc(SplashLayer.Desc)));
		}
		SplashLayer.Layer->bSplashLayer = true;
	}

	SplashLayersMap.Empty( SplashLayers.Num());
	for (int32 SplashLayerIndex = 0; SplashLayerIndex < SplashLayers.Num(); ++SplashLayerIndex)
	{
		SplashLayers[SplashLayerIndex].Layer->MarkCreateLayer();
		SplashLayersMap.Add(SplashLayers[SplashLayerIndex].Layer->GetLayerID(),SplashLayers[SplashLayerIndex].Layer);
	}

	StartTicker();

	bIsShown = true;
}

void FPicoXRSplash::Hide()
{
	check(IsInGameThread());
	bIsShown = false;

	StopTicker();
}

void FPicoXRSplash::AutoShow(bool AutoShowSplash)
{
	check(IsInGameThread());
	PicoSettings->bSplashScreenAutoShow =AutoShowSplash;
}

void FPicoXRSplash::AddSplash(const FPicoSplashDesc& Desc)
{
	check(IsInGameThread());
	FScopeLock ScopeLock(&RenderThreadLock);
	SplashLayers.Add(FPicoSplashLayer(Desc));
}

void FPicoXRSplash::ShowLoadingScreen()
{
	UE_LOG(LogTemp, Warning, TEXT("Splash ShowLoadingScreen!"));
	bShouldShowSplash = true;
	bNeedSplashUpdate = true;
}

void FPicoXRSplash::HideLoadingScreen()
{
	UE_LOG(LogTemp, Warning, TEXT("Splash HideLoadingScreen!"));
	bShouldShowSplash = false;
	bNeedSplashUpdate = bIsShown;
}

void FPicoXRSplash::ClearSplashes()
{
	check(IsInGameThread());
	FScopeLock ScopeLock(&RenderThreadLock);
	SplashLayers.Reset();
}

void FPicoXRSplash::AddSplash(const FSplashDesc& Splash)
{
	FPicoSplashDesc SplashDesc;
	SplashDesc.TransformInMeters = Splash.Transform;
	SplashDesc.QuadSizeInMeters = Splash.QuadSize;
	SplashDesc.DeltaRotation = Splash.DeltaRotation;
	SplashDesc.bNoAlphaChannel = Splash.bIgnoreAlpha;
	SplashDesc.bIsDynamic = Splash.bIsDynamic || Splash.bIsExternal;
	SplashDesc.TextureOffset = Splash.UVRect.Min;
	SplashDesc.TextureScale = Splash.UVRect.Max;
	SplashDesc.LoadedTexture = Splash.Texture;
	AddSplash(SplashDesc);
}

void FPicoXRSplash::UpdateLoadingScreen_GameThread()
{
	if (bNeedSplashUpdate)
	{
		if (bShouldShowSplash)
		{
			Show();
		}
		else
		{
			Hide();
		}

		bNeedSplashUpdate = false;
	}
}

void FPicoXRSplash::UnloadTextures()
{
	FScopeLock ScopeLock(&RenderThreadLock);
	for (int32 SplashLayerIndex = 0; SplashLayerIndex < SplashLayers.Num(); ++SplashLayerIndex)
	{
		if (SplashLayers[SplashLayerIndex].Desc.TexturePath.IsValid())
		{
			UnloadTexture(SplashLayers[SplashLayerIndex]);
		}
	}
}

void FPicoXRSplash::UnloadTexture(FPicoSplashLayer& InSplashLayer)
{
	check(IsInGameThread());
	InSplashLayer.Desc.LoadingTexture = nullptr;
	InSplashLayer.Desc.LoadedTexture = nullptr;
	InSplashLayer.Layer.Reset();
}

void FPicoXRSplash::LoadTexture(FPicoSplashLayer& InSplashLayer)
{
	check(IsInGameThread());
	if (InSplashLayer.Desc.LoadingTexture && InSplashLayer.Desc.LoadingTexture->IsValidLowLevel())
	{
		InSplashLayer.Desc.LoadingTexture = nullptr;
	}
	InSplashLayer.Desc.LoadedTexture = nullptr;
	InSplashLayer.Layer.Reset();
	InSplashLayer.Desc.LoadingTexture = Cast<UTexture>(InSplashLayer.Desc.TexturePath.TryLoad());
	InSplashLayer.Desc.LoadedTexture = nullptr;
	InSplashLayer.Layer.Reset();
}

void FPicoXRSplash::RenderFrame_RenderThread(FRHICommandListImmediate& RHICmdList)
{
	check(IsInRenderingThread());
	FScopeLock ScopeLock(&RenderThreadLock);
	FPXRGameFramePtr XFrame = Frame->Clone();
	XFrame->FrameNumber = PicoXRHMD->NextFrameNumber;
	XFrame->ShowFlags.Rendering = true;
#if PLATFORM_ANDROID
	if (Pxr_IsRunning() && PicoXRHMD->WaitFrameNumber != Frame->FrameNumber)
	{
		UE_LOG(LogTemp, Warning, TEXT("Splash WaitToBeginFrame %u,FrameNum:%u,LastWaitNumCurrentLevel:%u"), XFrame->FrameNumber, Frame->FrameNumber,LastWaitNumCurrentLevel);
		int CurrentVersion = 0;
		Pxr_GetConfigInt(PxrConfigType::PXR_API_VERSION, &CurrentVersion);
		if (CurrentVersion >= 0x2000304 && XFrame->FrameNumber != LastWaitNumCurrentLevel)//Skip the last Pxr_WaitFrame call of the current level,already called in StartGameFrame_GameThread.
		{
			Pxr_WaitFrame();
		}
		PicoXRHMD->WaitFrameNumber = XFrame->FrameNumber;
		PicoXRHMD->NextFrameNumber = XFrame->FrameNumber + 1;
	}
	else
	{
		XFrame->ShowFlags.Rendering = false;
	}
#endif
	ExecuteOnRHIThread([this,XFrame]()
		{
			if (XFrame->ShowFlags.Rendering)
			{
				UE_LOG(LogTemp, Warning, TEXT("Splash BeginFrame %u"), XFrame->FrameNumber);
				PicoXRHMD->BeginFrame_RHIThread();
			}
		});
	
	PicoXRHMD->SplashShow(RHICmdList);
	
	ExecuteOnRHIThread([this, XFrame]()
				{
			if (XFrame->ShowFlags.Rendering)
			{
				UE_LOG(LogTemp, Warning, TEXT("Splash EndFrame %u"), XFrame->FrameNumber);
				PicoXRHMD->EndFrame_RHIThread();
			}		
		});
}

IStereoLayers::FLayerDesc FPicoXRSplash::StereoLayerDescFromPicoSplashDesc(FPicoSplashDesc PicoDesc)
{
	IStereoLayers::FLayerDesc LayerDesc;
#if ENGINE_MINOR_VERSION > 24
	LayerDesc.SetShape<FQuadLayer>();
#else
	LayerDesc.ShapeType = IStereoLayers::QuadLayer;
#endif
	LayerDesc.Transform = PicoDesc.TransformInMeters;
	LayerDesc.QuadSize = PicoDesc.QuadSizeInMeters;
	LayerDesc.UVRect = FBox2D(PicoDesc.TextureOffset, PicoDesc.TextureOffset + PicoDesc.TextureScale);
	LayerDesc.Priority = INT32_MAX - (int32)(PicoDesc.TransformInMeters.GetTranslation().X * 1000.f);
	LayerDesc.PositionType = IStereoLayers::FaceLocked;
	LayerDesc.Texture = PicoDesc.LoadedTexture;
	LayerDesc.Flags = IStereoLayers::LAYER_FLAG_QUAD_PRESERVE_TEX_RATIO |
		(PicoDesc.bNoAlphaChannel ? IStereoLayers::LAYER_FLAG_TEX_NO_ALPHA_CHANNEL : 0) |
		(PicoDesc.bIsDynamic ? IStereoLayers::LAYER_FLAG_TEX_CONTINUOUS_UPDATE : 0);
	return LayerDesc;
}
