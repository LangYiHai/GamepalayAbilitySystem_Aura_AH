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

	// �����������л���ʵ�ʽṹ�壬���������д�˷�����
	virtual UScriptStruct* GetScriptStruct() const;

	virtual FAuraGameplayEffectContext* Duplicate() const;

	// �Զ������л������������д�˷��� ��ΪFGameplayEffectContextû�����л�����
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	// �Ƿ�Ϊ������
	UPROPERTY()
	bool bIsBlockedHit = false;

	// �Ƿ�Ϊ����
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