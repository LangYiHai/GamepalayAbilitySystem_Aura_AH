// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
	
public:

	AAuraCharacter();

	// �ڽ�ɫ���¿������ӹ�ʱ���� ���ڴ����ɫ����Ȩת��ʱ���߼�
	virtual void PossessedBy(AController* NewController) override;

	// �� **`PlayerState`** �������������Ϸ����仯ʱ���� ���ڿͻ���ͬ����ҵ�״̬������ UI ��ִ�����������״̬��ص��߼�
	virtual void OnRep_PlayerState() override;

	/** ս���ӿ� */
	virtual int32 GetPlayerLevel() override;

	/** Endս���ӿ� */

private:

	// ��ʼ��������Ա��Ϣ
	virtual	void InitAbilityActorInfo() override;
};
