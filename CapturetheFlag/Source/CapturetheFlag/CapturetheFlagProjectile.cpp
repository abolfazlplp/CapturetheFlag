// Copyright Epic Games, Inc. All Rights Reserved.

#include "CapturetheFlagProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include "DamageInterface.h"

#include "CapturetheFlag\CapturetheFlagCharacter.h"

ACapturetheFlagProjectile::ACapturetheFlagProjectile() 
{
	bReplicates = true;
	SetReplicateMovement(true);

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ACapturetheFlagProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 5 seconds by default
	InitialLifeSpan = 5.0f;
}

void ACapturetheFlagProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority()) return;
	
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if(OtherComp->IsSimulatingPhysics())
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		if (OtherActor->Implements<UDamageInterface>())
		{
			Server_SendDamage(OtherActor);
		}

		Destroy();
	}
}

void ACapturetheFlagProjectile::Server_SendDamage_Implementation(AActor* Actor)
{

	if(IDamageInterface* DamageTaker = Cast<IDamageInterface>(Actor))
	{
		DamageTaker->DeliverDamage(ProjectileDamage, this);
	}
}
