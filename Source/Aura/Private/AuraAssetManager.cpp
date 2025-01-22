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

	// UE��5.3�汾�Ѿ��Զ����ù��� ��Ƶp116������������������
	//UAbilitySystemGlobals::Get().InitGlobalData();
}
