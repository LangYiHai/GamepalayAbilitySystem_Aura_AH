// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);

	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	// �ж��Ƿ��ڱ��ؿ��ƵĽ�ɫ
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();

		// ��ӻص� ���������û��߸��µ�ʱ��ִ��OnTargetDataReplicatedCallback
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey)
			.AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		// ���Ŀ�����ݻص��Ƿ񱻵��� û�еĻ��ȴ�Ŀ������ͬ��
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// �ڽ�������Ԥ��ʱ����һ�����������
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	// ��ȡ��ҿ�����
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();

	// ��ȡ����·�����ײ��Ϣ
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	// ����Ŀ������
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	// ��Ŀ�����ݷ��͸�����������ͬ�� 
	// GetAbilitySpecHandle()����ȡ��ǰ�����Ĺ������
	// GetActivationPredictionKey()����ȡ����Ԥ��Ĺؼ���
	// FGameplayTag()���˴�û��ʹ�ñ�ǩ�����������ڸ��ӱ�ǩ��Ϣ��
	// AbilitySystemComponent->ScopedPredictionKey����ǰ�������Ԥ�����
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), 
		GetActivationPredictionKey(), 
		DataHandle, 
		FGameplayTag(), 
		AbilitySystemComponent->ScopedPredictionKey);

	// �㲥����
	if (ShouldBroadcastAbilityTaskDelegates()) 
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	// ��������ϵͳ Ŀ���������յ�(��ǽ��յ���Ŀ������Ϊ�Ѵ���)
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	// ȷ���Ƿ���Թ㲥����������ص�ί��
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
