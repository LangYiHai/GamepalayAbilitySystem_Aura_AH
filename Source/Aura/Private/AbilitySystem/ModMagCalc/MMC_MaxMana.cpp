// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	// 捕获 UAuraAttributeSet 类中的 Vigor(精神) 属性。
	IntDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();

	// 设置属性来源为目标（想要施加效果的对象）
	IntDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// 设置为捕获即时值
	IntDef.bSnapshot = false;

	// 将VigorDef添加到EffectSpecHandle.Data.Get()->RelevantAttributesToCapture 数组中
	RelevantAttributesToCapture.Add(IntDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 从源和目标收集标签
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// 评估参数
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	
	float Int = 0.f;

	// 获取 Vigor 属性的最终值
	GetCapturedAttributeMagnitude(IntDef, Spec, EvaluationParameters, Int);

	// 让最终值不为负
	Int = FMath::Max<float>(Int, 0.f);

	// Spec.GetContext().GetSourceObject()获取到的源对象通常是触发效果的对象（例如施放技能的角色）。
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 50.f + 2.5f * Int + 15.f * PlayerLevel;
}
