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

// Ӧ��Ч����Ŀ��
void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	// ʹ��UE�Դ�����ͼ�⺯����ȡ����ϵͳ���
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetASC == nullptr) 
	{
		return;
	}

	check(GameplayEffectClass);

	// ������ϷЧ��������
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();

	// ���������е�Դ��������Ϊ����Ч��ʩ���ߣ�������Ӧ��Ч��ʱȷ��Ч������Դ
	EffectContextHandle.AddSourceObject(this);

	//������ϷЧ�����(����������ϷЧ��)(����1ΪЧ�����ݣ�����2ΪЧ��ǿ�ȣ�����3ΪЧ��������(�ṩЧ��Ӧ�õĸ�����Ϣ))
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffectContextHandle);

	// Ӧ����ϷЧ��
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*(EffectSpecHandle.Data.Get()));

	// ��ȡ����ϷЧ���ĳ��������Ƿ�Ϊ ����
	const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

	// �洢���ó�������ϷЧ�� ���ں����Ƴ����ó�����ϷЧ��
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

		// �Ƴ����ó���������Ч��
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


