// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	return *(Cast<UAuraAssetManager>(GEngine->AssetManager));
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitializeNativeGameplayTags();

	// UE在5.3版本已经自动调用过了 视频p116在这里调用了这个函数
	//UAbilitySystemGlobals::Get().InitGlobalData();
}
