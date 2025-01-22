// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	// ���� UAuraAttributeSet ���е� Vigor(����) ���ԡ�
	IntDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();

	// ����������ԴΪĿ�꣨��Ҫʩ��Ч���Ķ���
	IntDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// ����Ϊ����ʱֵ
	IntDef.bSnapshot = false;

	// ��VigorDef��ӵ�EffectSpecHandle.Data.Get()->RelevantAttributesToCapture ������
	RelevantAttributesToCapture.Add(IntDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// ��Դ��Ŀ���ռ���ǩ
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	// ��������
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	
	float Int = 0.f;

	// ��ȡ Vigor ���Ե�����ֵ
	GetCapturedAttributeMagnitude(IntDef, Spec, EvaluationParameters, Int);

	// ������ֵ��Ϊ��
	Int = FMath::Max<float>(Int, 0.f);

	// Spec.GetContext().GetSourceObject()��ȡ����Դ����ͨ���Ǵ���Ч���Ķ�������ʩ�ż��ܵĽ�ɫ����
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 50.f + 2.5f * Int + 15.f * PlayerLevel;
}
