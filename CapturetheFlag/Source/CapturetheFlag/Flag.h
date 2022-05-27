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
	virtual void Server_CaptureFlag(class ACapturetheFlagCharacter* Player);
	virtual void Server_CaptureFlag_Implementation(class ACapturetheFlagCharacter* Player);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	virtual void Multicast_CaptureFlag(class ACapturetheFlagCharacter* Player);
	virtual void Multicast_CaptureFlag_Implementation(class ACapturetheFlagCharacter* Player);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_ReturnFlag();
	void Server_ReturnFlag_Implementation();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_ReturnFlag();
	void Multicast_ReturnFlag_Implementation();

	UFUNCTION(BlueprintCallable,Server,Reliable)
	void Server_DropFlag(class ACapturetheFlagCharacter* Player);
	void Server_DropFlag_Implementation(class ACapturetheFlagCharacter* Player);

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Multicast_DropFlag(class ACapturetheFlagCharacter* Player);
	void Multicast_DropFlag_Implementation(class ACapturetheFlagCharacter* Player);

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Replicated, Category = Components)
	class AFlagBase* CurrentOwner;

private:

	UPROPERTY(EditDefaultsOnly)
	class UMaterial* RedMaterial;

	UPROPERTY(EditDefaultsOnly)
	class UMaterial* BlueMaterial;

};