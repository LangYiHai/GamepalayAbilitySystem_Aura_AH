// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include <EnhancedInputSubsystems.h>
#include "Interaction/EnemyInterface.h"
#include "Input/AuraInputComponent.h"
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "UI//Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	AuraAbilitySystemComponent = nullptr;

	// Ŀ�ĵػ���
	CachedDestination = FVector::ZeroVector;

	// ����ʱ��
	FollowTime = 0.f;

	// �̰���ֵ
	ShortPressThreshold = 0.5f;
	 
	// �Զ����п���
	bAutoRunning = false;

	bTargeting = false;

	// �Զ����н��ܰ뾶
	AutoRunAcceptanceRadius = 50.f;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");

	bShiftKeyDown = false;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();

	AutoRun();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBolockedHit, bool bIsCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBolockedHit, bIsCriticalHit);
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning)
	{
		return;
	}

	if (APawn* ControlledPawn = GetPawn())
	{
		// ��ȡ��ǰλ����·�������λ��
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		
		// ��ȡ·����ǰ������
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);

		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	// ��ȡ��ǰ�������������ϵͳ
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (Subsystem)
	{
		// ���ӳ��������
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	// ���������ɼ� ���������ʽ
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// �������ģʽΪ������
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// �������¼�ת��Ϊ��ǿ�¼�
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	// ���ƶ��¼�
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);

	AuraInputComponent->BindAbilityActions(InputConfig, this, &AAuraPlayerController::AbilityInputTagPressed, &AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHeld);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	// ��ȡ����Ķ�ά��ֵ
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	//��ȡ��ת��Ϣ
	const FRotator Rotation = GetControlRotation();

	//��ȡ��ǰ��Yaw�Ƕȣ����ڼ����ɫ���������ˮƽ������ת�����Դ�ֱ����ͷ�����
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	// ��ȡ��ǰ����
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	//��ȡ���ҷ���
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>()) 
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::ShiftPressed()
{
	bShiftKeyDown = true;
}

void AAuraPlayerController::ShiftReleased()
{
	bShiftKeyDown = false;
}

void AAuraPlayerController::CursorTrace()
{
	// ��ȡ������µ���ײ��Ϣ
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	//����Ƿ�����ײ����
	if (!CursorHit.bBlockingHit) 
	{
		return;
	}

	// ת��Ϊ��������
	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor != ThisActor) 
	{
		if (LastActor) 
		{
			LastActor->UnHighlightActor();
		}

		if (ThisActor) 
		{
			ThisActor->HighlightActor();
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB)) 
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC() != nullptr)
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}

		return;
	}

	if (GetASC() != nullptr)
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}

	if (!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();

		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			// ͬ������ӵ�ǰ��ɫλ�� (ControlledPawn->GetActorLocation()) ��Ŀ��ص� (CachedDestination) �ĵ���·����(����ʹ���첽�����Ż�����)
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}

				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];

				bAutoRunning = true;
			}
		}

		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC() != nullptr)
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}

		return;
	}

	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC() != nullptr)
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else 
	{
		// ����ʱ��+����һ֡����ǰ֡��������ʱ��
		FollowTime += GetWorld()->GetDeltaSeconds();

		// �Ƿ�ɹ���⵽����·�������
		if (CursorHit.bBlockingHit)
		{
			// �洢���λ�� (Hit.ImpactPoint��ʾ��������ײ����ĽӴ���)
			CachedDestination = CursorHit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn()) 
		{
			// ����Ŀ�귽��������
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();

			// ��÷�������˶�����
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AuraAbilitySystemComponent;
}


