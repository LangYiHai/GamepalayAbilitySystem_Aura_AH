// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

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
