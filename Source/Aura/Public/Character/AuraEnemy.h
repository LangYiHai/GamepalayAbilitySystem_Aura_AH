// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()
	
public:

	AAuraEnemy();


	/** ���˽ӿ� */
	// ͻ����ʾ	���˽ӿ�
	virtual void HighlightActor() override;

	// ȡ��ͻ����ʾ	���˽ӿ�
	virtual void UnHighlightActor() override;
	/** End���˽ӿ� */


	/** ս���ӿ� */
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	/** Endս���ӿ� */

	UPROPERTY(BlueprintAssignable)	//��ͼ�ɷ���
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)	//��ͼ�ɷ���
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReactiong;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan;

protected:

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
