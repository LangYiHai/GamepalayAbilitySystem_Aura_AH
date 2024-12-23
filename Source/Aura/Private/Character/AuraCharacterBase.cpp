// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
 	// �ر�ÿ֡����Tick
	PrimaryActorTick.bCanEverTick = false;

	// ���� ��������
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");

	// �����������
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));

	// �ر���ײ
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

