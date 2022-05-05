// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "ShowFlags.h"

class FPXRGameFrame : public TSharedFromThis<FPXRGameFrame, ESPMode::ThreadSafe>
{
public:
	uint32 FrameNumber;
	int32 ViewNumber;
	double predictedDisplayTimeMs;
	float WorldToMetersScale;
	FTransform TrackingToWorld;
	FVector Position;
	FQuat Orientation;
	FVector AngularVelocity;
	FVector Acceleration;
	FVector AngularAcceleration;
	FVector Velocity;
	bool bSplashShown;
	FEngineShowFlags ShowFlags;
	union
	{
		struct
		{
			uint64			bSplashIsShown : 1;
		};
		uint64 Raw;
	} Flags;
public:
	FPXRGameFrame();
	TSharedPtr<FPXRGameFrame, ESPMode::ThreadSafe> Clone() const;
};

typedef TSharedPtr<FPXRGameFrame, ESPMode::ThreadSafe> FPXRGameFramePtr;