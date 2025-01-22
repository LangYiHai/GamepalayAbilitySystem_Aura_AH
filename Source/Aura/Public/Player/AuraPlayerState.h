// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:

	AAuraPlayerState();

	// ���� ������Щ������Ҫ�������縴��
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ���ػ�ȡ��Ϸ�������
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// ��ȡ���Լ�
	UAttributeSet* GetAttributeSet() const;

	FORCEINLINE int32 GetPlayerLevel() const;

protected:

	// ����ϵͳ���
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// ���Լ�
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;


private:

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
	int32 Level;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
};
