// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	// ���ý�ɫ���ƶ��ķ�����ת
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// ���ý�ɫ��ˮƽ��ת
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// ���ƽ�ɫ��ˮƽ�����ƶ�
	GetCharacterMovement()->bConstrainToPlane = true;

	// ���ý�ɫ�ڿ�ʼʱ�̶��ڵ���
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// ���ý�ɫ��������ת���ɿ���������
	bUseControllerRotationPitch = false;

	// ���ý�ɫ�ķ������ɿ���������
	bUseControllerRotationRoll = false;

	// ���ý�ɫ��ˮƽ��ת���ɿ���������
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// �ӷ�������ʼ������actor��Ϣ
	InitAbilityActorInfo();

	// ��ӽ�ɫ����
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// �ӿͻ��˳�ʼ������actor��Ϣ
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	// ��ȡ���״̬
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	// ��ȡ���״̬
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	// ������ϵͳ��� ��ʼ����ɫ����ϵͳ��Ϣ
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);

	// ����������Ա��Ϣ
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	// ʵ��������ϵͳ���
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();

	// ʵ�������Լ�
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// �ڶ�����Ϸ�������������Ϊ�գ�����Ҫ��������һ���Ƿ�Ϊ��
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController())) 
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD())) 
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}
