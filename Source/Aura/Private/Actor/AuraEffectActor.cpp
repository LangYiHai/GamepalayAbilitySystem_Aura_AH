// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bDestroyOnEffectRemoveal = false;

	InstantEffectApplicationPolicy = EEFFectApplicationPolicy::DoNotApply;

	DurationEffectApplicationPolicy = EEFFectApplicationPolicy::DoNotApply;

	InfiniteEffectApplicationPolicy = EEFFectApplicationPolicy::DoNotApply;

	InfiniteEffectRemovalPolicy = EEFFectRemovalPolicy::RemoveOnEndOverlap;

	ActorLevel = 1.f;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

// 应用效果到目标
void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// 使用UE自带的蓝图库函数获取能力系统组件
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetASC == nullptr) 
	{
		return;
	}

	check(GameplayEffectClass);

	// 创建游戏效果上下文
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();

	// 将上下文中的源对象设置为自身（效果施放者）方便在应用效果时确定效果的来源
	EffectContextHandle.AddSourceObject(this);

	//创建游戏效果规格(定义具体的游戏效果)(参数1为效果内容，参数2为效果强度，参数3为效果上下文(提供效果应用的附加信息))
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);

	// 应用游戏效果
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*(EffectSpecHandle.Data.Get()));

	// 获取该游戏效果的持续策略是否为 永久
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

	// 存储永久持续的游戏效果 用于后续移除永久持续游戏效果
	if (bIsInfinite && InfiniteEffectRemovalPolicy == EEFFectRemovalPolicy::RemoveOnEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEFFectApplicationPolicy::ApplyOnOverlap) 
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEFFectApplicationPolicy::ApplyOnOverlap) 
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEFFectApplicationPolicy::ApplyOnOverlap) 
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEFFectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEFFectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEFFectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEFFectRemovalPolicy::RemoveOnEndOverlap) 
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

		if (!IsValid(TargetASC)) 
		{
			return;
		}

		// 移除永久持续的能力效果
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles) 
		{
			if (TargetASC == HandlePair.Value) 
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}

		for (auto& Handle : HandlesToRemove) 
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}


