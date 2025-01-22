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

	// 在角色被新控制器接管时调用 用于处理角色控制权转移时的逻辑
	virtual void PossessedBy(AController* NewController) override;

	// 当 **`PlayerState`** 的数据在网络上发生变化时调用 用于客户端同步玩家的状态，更新 UI 或执行其他与玩家状态相关的逻辑
	virtual void OnRep_PlayerState() override;

	/** 战斗接口 */
	virtual int32 GetPlayerLevel() override;

	/** End战斗接口 */

private:

	// 初始化能力演员信息
	virtual	void InitAbilityActorInfo() override;
};
