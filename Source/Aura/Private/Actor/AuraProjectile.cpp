// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Aura/Aura.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	// 开启网络复制
	bReplicates = true;

	LifeSpan = 15.f;

	bHit = false;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);

	// 设置球体碰撞类型
	Sphere->SetCollisionObjectType(ECC_PROJECTILE);

	// 设置球体 碰撞体仅用于 查询  可以用于检测碰撞，但不会阻止物体的移动或造成物理响应。
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 设置该碰撞体将忽略所有其他物体的碰撞检测，不会与任何物体发生碰撞
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);

	// 设置不产生物理反应，仅用于检测和触发事件
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	// 设置该碰撞体会与静态物体发生重叠碰撞，但不产生物理反应
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);

	// 设置球形碰撞体会与角色发生重叠碰撞，但不会产生物理反应
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// 设置物体寿命
	SetLifeSpan(LifeSpan);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
}

void AAuraProjectile::Destroyed()
{
	if (!bHit && !HasAuthority()) 
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());

		if (LoopingSoundComponent) 
		{
			LoopingSoundComponent->Stop();
		}
		
	}

	Super::Destroyed();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (DamageEffectSpecHandle.Data.IsValid() && DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor) 
	{
		return;
	}

	if (!bHit) 
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());

		if (LoopingSoundComponent)
		{
			LoopingSoundComponent->Stop();
		}
	}



	// 判断是否在服务器
	if (HasAuthority()) 
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor)) 
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}

		Destroy();
	}
	else 
	{
		bHit = true;
	}
}

