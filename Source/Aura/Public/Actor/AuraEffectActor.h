// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffect.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEFFectApplicationPolicy : uint8
{
	ApplyOnOverlap,		// ��ʼ�ص�
	ApplyOnEndOverlap,	// �����ص�
	DoNotApply			// û��Ӧ��
};

UENUM(BlueprintType)
enum class EEFFectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,		// �����ص�
	DoNotRemoval			// û��Ӧ��
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraEffectActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectRemoveal;

	// ʵʱ��ϷЧ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	// ʵʱ��Ч��Ч��Ӧ�ò���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEFFectApplicationPolicy InstantEffectApplicationPolicy;

	// ������ϷЧ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	// ������Ч��Ч��Ӧ�ò���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEFFectApplicationPolicy DurationEffectApplicationPolicy;

	// ������ϷЧ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect>InfiniteGameplayEffectClass;

	// ������Ч��Ч��Ӧ�ò���
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEFFectApplicationPolicy InfiniteEffectApplicationPolicy;

	// ������Ч��Ч���Ƴ�����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEFFectRemovalPolicy InfiniteEffectRemovalPolicy;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	float ActorLevel;
};
