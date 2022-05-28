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

void AFlag::Server_CaptureFlag_Implementation(ACapturetheFlagCharacter* Player)
{
	Multicast_CaptureFlag(Player);
}

void AFlag::Multicast_CaptureFlag_Implementation(ACapturetheFlagCharacter* Player)
{
	if (!Player) return;

	Player->bCarryFlag = true;
	Player->CarryingFlag = this;
	CurrentOwner->bHasFlag = false;

	Mesh->SetVisibility(false);
	BoxCollision->Activate(false);
}

void AFlag::Server_ReturnFlag_Implementation()
{
	Multicast_ReturnFlag();
}

void AFlag::Multicast_ReturnFlag_Implementation()
{
	Mesh->SetVisibility(true);
	BoxCollision->Activate(true);
	CurrentOwner->bHasFlag = true;
	SetActorTransform(CurrentOwner->GetActorTransform());
}

void AFlag::Server_DropFlag_Implementation(ACapturetheFlagCharacter* Player)
{
	Multicast_DropFlag(Player);
}

void AFlag::Multicast_DropFlag_Implementation(ACapturetheFlagCharacter* Player)
{
	if (!Player) return;

	Mesh->SetVisibility(true);
	BoxCollision->Activate(true);
	SetActorTransform(Player->GetActorTransform());
}


// Called when the game starts or when spawned
void AFlag::BeginPlay()
{
	Super::BeginPlay();

	if (AFlagBase* FlagOwner = Cast<AFlagBase>(GetOwner()))
	{
		CurrentOwner = FlagOwner;
		if (!Mesh) return;

		if (FlagOwner->FlagBaseTeam == ETeam::Blue)
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

	if (PlayerCharacter->PlayerTeam != CurrentOwner->FlagBaseTeam)
	{
			UE_LOG(LogTemp, Warning, TEXT("Enemy Character!"));
			if (CurrentOwner->PlayerBaseHasFlag(PlayerCharacter))
			{
				UE_LOG(LogTemp, Warning, TEXT("Player Base Has Flag!"));
				Server_CaptureFlag(PlayerCharacter);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Your Flag is Stolen!"));
			}
	}
	else
	{
		if (!CurrentOwner->bHasFlag)
			Server_ReturnFlag();
	}
}