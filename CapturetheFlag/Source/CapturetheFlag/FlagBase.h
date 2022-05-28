// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "CapturetheFlagCharacter.h"

#include "FlagBase.generated.h"

UCLASS()
class CAPTURETHEFLAG_API AFlagBase : public AActor
{
	GENERATED_BODY()


	UPROPERTY(EditDefaultsOnly, Category = Components)
	class UBoxComponent* BoxCollision;
	
public:	
	// Sets default values for this actor's properties
	AFlagBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_SpawnFlag();
	void Server_SpawnFlag_Implementation();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_SpawnFlag();
	void Multicast_SpawnFlag_Implementation();

	UFUNCTION(BlueprintCallable)
	virtual	bool PlayerBaseHasFlag(class ACapturetheFlagCharacter* Player);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	virtual void Server_SetTeamScore(class ACapturetheFlagCharacter* Player);
	virtual void Server_SetTeamScore_Implementation(class ACapturetheFlagCharacter* Player);

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(EditDefaultsOnly, Category = Flag)
	TSubclassOf<class AFlag> FlagClass;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Flag)
	bool bHasFlag = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly,Replicated, Category = Team)
	TEnumAsByte<ETeam> FlagBaseTeam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Flag)
	class AFlag* Flag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = Flag)
	int TeamScore;

};