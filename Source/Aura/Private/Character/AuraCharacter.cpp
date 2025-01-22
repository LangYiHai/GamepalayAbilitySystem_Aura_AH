// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	// 设置角色朝移动的方向旋转
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 设置角色在水平旋转
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// 限制角色在水平面上移动
	GetCharacterMovement()->bConstrainToPlane = true;

	// 设置角色在开始时固定在地面
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 设置角色的上下旋转不由控制器控制
	bUseControllerRotationPitch = false;

	// 设置角色的翻滚不由控制器控制
	bUseControllerRotationRoll = false;

	// 设置角色的水平旋转不由控制器控制
	bUseControllerRotationYaw = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 从服务器初始化能力actor信息
	InitAbilityActorInfo();

	// 添加角色能力
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// 从客户端初始化能力actor信息
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	// 获取玩家状态
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	// 获取玩家状态
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);

	// 绑定能力系统组件 初始化角色能力系统信息
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);

	// 设置能力演员信息
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	// 实例化能力系统组件
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();

	// 实例化属性集
	AttributeSet = AuraPlayerState->GetAttributeSet();

	// 在多人游戏中玩控制器可能为空，所以要在这里检查一下是否为空
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController())) 
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD())) 
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}
