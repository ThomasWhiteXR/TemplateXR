// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.

#include "OnlinePicoSettings.h"

UOnlinePicoSettings::UOnlinePicoSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	bEnablePaymentModule(false),
	bIsForeign(false),
	bIsGlobal(false),
	bStartTimeEntitlementCheck(false),
	bEntitlementCheckSimulation(false)
{
}

void UOnlinePicoSettings::PostInitProperties()
{
	Super::PostInitProperties();

	switch (RegionType)
	{
	case ERegionType::China:
	{
		bIsForeign = false;
		bIsGlobal = false;
	}
	break;
	case ERegionType::NonChina:
	{
		bIsForeign = true;
		bIsGlobal = false;
	}
	break;
	case ERegionType::Both:
	{
		bIsForeign = false;
		bIsGlobal = true;
	}
	break;
	default:
		break;
	}

	Super::UpdateSinglePropertyInConfigFile(GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UOnlinePicoSettings, bIsForeign)), GetDefaultConfigFilename());
	Super::UpdateSinglePropertyInConfigFile(GetClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(UOnlinePicoSettings, bIsGlobal)), GetDefaultConfigFilename());
}