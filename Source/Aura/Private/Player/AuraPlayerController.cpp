// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include <EnhancedInputSubsystems.h>
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(AuraContext);

	// ��ȡ��ǰ�������������ϵͳ
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

	// ���ӳ��������
	Subsystem->AddMappingContext(AuraContext, 0);

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
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	// ���ƶ��¼�
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
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
