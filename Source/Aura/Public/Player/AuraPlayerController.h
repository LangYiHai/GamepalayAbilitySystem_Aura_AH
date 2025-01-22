// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"



class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBolockedHit, bool bIsCriticalHit);

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	FHitResult CursorHit;

	void Move(const FInputActionValue& InputActionValue);

	void ShiftPressed();
	void ShiftReleased();
	bool bShiftKeyDown;

	// ������
	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);

	void AbilityInputTagReleased(FGameplayTag InputTag);

	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	// Ŀ�ĵػ���
	FVector CachedDestination; 

	// ����ʱ��
	float FollowTime;

	// �̰���ֵ
	float ShortPressThreshold;

	// �Զ����п���
	bool bAutoRunning;

	// �Ƿ�ΪĿ��
	bool bTargeting;

	// �Զ����н��ܰ뾶
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius;

	// ��������
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	// �Զ�����
	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
