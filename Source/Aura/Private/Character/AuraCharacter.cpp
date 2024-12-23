// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

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
