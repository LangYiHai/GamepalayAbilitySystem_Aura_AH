// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// ���� UAuraAttributeSet ���е� Vigor(����) ���ԡ�
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();

	// ����������ԴΪĿ�꣨��Ҫʩ��Ч���Ķ���
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// ����Ϊ����ʱֵ
	VigorDef.bSnapshot = false;

	// ��VigorDef��ӵ�EffectSpecHandle.Data.Get()->RelevantAttributesToCapture ������
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// ��Դ��Ŀ���ռ���ǩ
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// ��������
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;


	float Vigor = 0.f;

	// ��ȡ Vigor ���Ե�����ֵ
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluationParameters, Vigor);

	// ������ֵ��Ϊ��
	Vigor = FMath::Max<float>(Vigor, 0.f);

	// Spec.GetContext().GetSourceObject()��ȡ����Դ����ͨ���Ǵ���Ч���Ķ�������ʩ�ż��ܵĽ�ɫ����
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
}
