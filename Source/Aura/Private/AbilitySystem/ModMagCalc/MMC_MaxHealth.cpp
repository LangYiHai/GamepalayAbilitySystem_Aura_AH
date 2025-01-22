// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// 捕获 UAuraAttributeSet 类中的 Vigor(精神) 属性。
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();

	// 设置属性来源为目标（想要施加效果的对象）
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// 设置为捕获即时值
	VigorDef.bSnapshot = false;

	// 将VigorDef添加到EffectSpecHandle.Data.Get()->RelevantAttributesToCapture 数组中
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 从源和目标收集标签
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 评估参数
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;


	float Vigor = 0.f;

	// 获取 Vigor 属性的最终值
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);

	// 让最终值不为负
	Vigor = FMath::Max<float>(Vigor, 0.f);

	// Spec.GetContext().GetSourceObject()获取到的源对象通常是触发效果的对象（例如施放技能的角色）。
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
