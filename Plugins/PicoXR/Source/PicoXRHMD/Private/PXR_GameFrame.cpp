// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.
#include "PXR_GameFrame.h"

FPXRGameFrame::FPXRGameFrame() :
	FrameNumber(0),
	ViewNumber(0),
	predictedDisplayTimeMs(0),
	WorldToMetersScale(100),
	ShowFlags(ESFIM_All0)
{
	Flags.Raw = 0;
	Position = FVector::ZeroVector;
	Orientation = FQuat::Identity;
	AngularVelocity = FVector::ZeroVector;
	Acceleration = FVector::ZeroVector;
	AngularAcceleration = FVector::ZeroVector;
	Velocity = FVector::ZeroVector;
}

TSharedPtr<FPXRGameFrame, ESPMode::ThreadSafe> FPXRGameFrame::Clone() const
{
	TSharedPtr<FPXRGameFrame, ESPMode::ThreadSafe> NewFrame = MakeShareable(new FPXRGameFrame(*this));
	return NewFrame;
}
