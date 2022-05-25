// Fill out your copyright notice in the Description page of Project Settings.


#include "Flag.h"
#include "FlagBase.h"
#include "CapturetheFlagCharacter.h"

#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "Net\UnrealNetwork.h"

// Sets default values
AFlag::AFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");

	if (BoxCollision)
	{
		RootComponent = BoxCollision;
		BoxCollision->SetCollisionProfileName(TEXT("OverlapAll"));
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFlag::OnComponentBeginOverlap);
	}

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	if (Mesh)
	{
		Mesh->SetupAttachment(RootComponent);
	}

}

void AFlag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlag, CurrentOwner);
}

void AFlag::Server_ReturnFlag_Implementation(ACapturetheFlagCharacter* Player)
{
	if (!Player) return;

	if (CurrentOwner)
	{
		if (Player->PlayerTeam == CurrentOwner->FlagBaseTeam)
		{
			SetActorTransform(CurrentOwner->GetActorTransform());
			CurrentOwner->bHasFlag = true;
		}
	}

	//AFlagBase* OwnerBase = Cast<AFlagBase>(GetOwner())
}

// Called when the game starts or when spawned
void AFlag::BeginPlay()
{
	Super::BeginPlay();

	if (CurrentOwner)
	{
		if (!Mesh) return;

		if (CurrentOwner->FlagBaseTeam == ETeam::Blue)
		{
			UE_LOG(LogTemp,Warning,TEXT("Blue Material Created!"))
			Mesh->SetMaterial(0, BlueMaterial);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Red Material Created!"))
			Mesh->SetMaterial(0, RedMaterial);
		}			
	}
}

void AFlag::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	ACapturetheFlagCharacter* PlayerCharacter = Cast<ACapturetheFlagCharacter>(OtherActor);

	if (!PlayerCharacter) return;

	UE_LOG(LogTemp, Warning, TEXT("OverLapped %s"), *PlayerCharacter->GetName())

	Server_ReturnFlag(PlayerCharacter);
}