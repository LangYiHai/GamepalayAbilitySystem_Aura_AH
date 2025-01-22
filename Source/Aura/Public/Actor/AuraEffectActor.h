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
	ApplyOnOverlap,		// 开始重叠
	ApplyOnEndOverlap,	// 结束重叠
	DoNotApply			// 没有应用
};

UENUM(BlueprintType)
enum class EEFFectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,		// 结束重叠
	DoNotRemoval			// 没有应用
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

	// 实时游戏效果
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	// 实时生效的效果应用策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEFFectApplicationPolicy InstantEffectApplicationPolicy;

	// 持续游戏效果
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	// 持续生效的效果应用策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEFFectApplicationPolicy DurationEffectApplicationPolicy;

	// 永久游戏效果
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect>InfiniteGameplayEffectClass;

	// 永久生效的效果应用策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEFFectApplicationPolicy InfiniteEffectApplicationPolicy;

	// 永久生效的效果移除策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEFFectRemovalPolicy InfiniteEffectRemovalPolicy;

	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	float ActorLevel;
};
