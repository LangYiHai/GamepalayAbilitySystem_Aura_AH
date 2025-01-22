#pragma once


#include "GameplayEffectTypes.h"

#include "AuraAbilityTypes.generated.h"


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsCriticalHit() const;

	bool IsBlockedHit() const;

	void SetIsCriticalHit(bool bInIsCriticalHit);

	void SetIsBlockedHit(bool bInIsBlockedHit);

	// 返回用于序列化的实际结构体，子类必须重写此方法！
	virtual UScriptStruct* GetScriptStruct() const;

	virtual FAuraGameplayEffectContext* Duplicate() const;

	// 自定义序列化，子类必须重写此方法 因为FGameplayEffectContext没有序列化方法
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	// 是否为格挡命中
	UPROPERTY()
	bool bIsBlockedHit = false;

	// 是否为暴击
	UPROPERTY()
	bool bIsCriticalHit = false;
};

template<>
struct TStructOpsTypeTraits< FAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FAuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};