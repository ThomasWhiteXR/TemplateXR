// Copyright © 2015-2021 Pico Technology Co., Ltd. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "OnlineSubsystemPicoModule.h"

class FUniqueNetIdPico : public FUniqueNetId
{
private:
	uint64 ID;

protected:
	virtual bool Compare(const FUniqueNetId& Other) const override
	{
		if (Other.GetType() != GetType())
		{
			return false;
		}

		if (Other.GetSize() != sizeof(uint64))
		{
			return false;
		}

		return ID == static_cast<const FUniqueNetIdPico&>(Other).ID;
	}

public:
	FUniqueNetIdPico()
	{
		ID = 0;
	}

	FUniqueNetIdPico(const uint64& id)
	{
		ID = id;
	}

	FUniqueNetIdPico(const FString& id)
	{
		FCString::Strtoui64(*id, NULL, 10);
	}

	explicit FUniqueNetIdPico(const FUniqueNetId& Src)
	{
		if (Src.GetSize() == sizeof(uint64))
		{
			ID = static_cast<const FUniqueNetIdPico&>(Src).ID;
		}
	}

	virtual FName GetType() const override
	{
		return PICO_SUBSYSTEM;
	}

	virtual const uint8* GetBytes() const override
	{
		return reinterpret_cast<const uint8*>(&ID);
	}

	virtual int32 GetSize() const override
	{
		return sizeof(ID);
	}

	virtual bool IsValid() const override
	{
		return ID > 0;
	}

	uint64 GetID() const
	{
		return ID;
	}

	virtual FString ToString() const override
	{
		return FString::Printf(TEXT("%llu"), ID);
	}

	virtual FString ToDebugString() const override
	{
		const FString UniqueNetIdStr = FString::Printf(TEXT("%llu"), ID);
		return TEXT("PicoUserID:") + UniqueNetIdStr;
	}

	friend uint32 GetTypeHash(const FUniqueNetIdPico& A)
	{
		return GetTypeHash((uint64)A.ID);
	}

	static const TSharedRef<const FUniqueNetId>& EmptyId()
	{
		static const TSharedRef<const FUniqueNetId> EmptyId(MakeShared<FUniqueNetIdPico>());
		return EmptyId;
	}
};
