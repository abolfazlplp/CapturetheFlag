// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Flag.generated.h"

UCLASS()
class CAPTURETHEFLAG_API AFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlag();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_ReturnFlag(ACapturetheFlagCharacter* Player);
	void Server_ReturnFlag_Implementation(ACapturetheFlagCharacter* Player);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite, Category = Components)
	class UBoxComponent* BoxCollision;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Components)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite,Replicated, Category = Components)
	class AFlagBase* CurrentOwner;

private:

	UPROPERTY(EditDefaultsOnly)
	class UMaterial* RedMaterial;

	UPROPERTY(EditDefaultsOnly)
	class UMaterial* BlueMaterial;

};
