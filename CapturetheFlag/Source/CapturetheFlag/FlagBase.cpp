// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagBase.h"
#include "Net\UnrealNetwork.h"
#include "Components/BoxComponent.h"

#include "Flag.h"

// Sets default values
AFlagBase::AFlagBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");

	if (BoxCollision)
	{
		RootComponent = BoxCollision;
		BoxCollision->SetCollisionProfileName(TEXT("OverlapAll"));
		BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFlagBase::OnComponentBeginOverlap);
	}

}

// Called when the game starts or when spawned
void AFlagBase::BeginPlay()
{
	Super::BeginPlay();

	if(HasAuthority())
		Server_SpawnFlag();
}

void AFlagBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFlagBase, bHasFlag);
	DOREPLIFETIME(AFlagBase, Flag);
	DOREPLIFETIME(AFlagBase, TeamScore);
}

void AFlagBase::Server_SpawnFlag_Implementation()
{
	Multicast_SpawnFlag();
}

void AFlagBase::Multicast_SpawnFlag_Implementation()
{
	Flag = GetWorld()->SpawnActor<AFlag>(FlagClass, GetActorTransform());

	Flag->SetOwner(this);
	Flag->CurrentOwner = this;
}

void AFlagBase::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;

	ACapturetheFlagCharacter* PlayerCharacter = Cast<ACapturetheFlagCharacter>(OtherActor);

	if (!PlayerCharacter) return;

	if (bHasFlag)
	{

	}else
	{
		
	}
}