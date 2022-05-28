// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "DamageInterface.h"

#include "CapturetheFlagCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;

UENUM(BlueprintType)
enum ETeam
{
	Blue UMETA(DisplayName = "Blue"),
	Red UMETA(DisplayName = "Red")
};

UCLASS(config=Game)
class ACapturetheFlagCharacter : public ACharacter, public IDamageInterface
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* MeshTP;

	UPROPERTY(EditDefaultsOnly)
	class UMaterial* RedMaterial;

	UPROPERTY(EditDefaultsOnly)
	class UMaterial* BlueMaterial;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMotionControllerComponent* L_MotionController;

public:
	ACapturetheFlagCharacter();

protected:
	virtual void BeginPlay();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ACapturetheFlagProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* DamageAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Replicated, Category = Team)
	TEnumAsByte<ETeam> PlayerTeam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = Health)
	float PlayerHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Health)
	float CurrentPlayerHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Flag)
	bool bCarryFlag = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = Flag)
	class AFlag* CarryingFlag = nullptr;

	FTimerHandle RespawnTimerHandle;

	UPROPERTY(EditDefaultsOnly,Category = Respawn)
	float RespawnTime = 5.f;

	FTransform SpawnTransform;
	FTransform MeshTPInitialTransform;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

public:
	UFUNCTION(Server, Reliable)
	void Server_Fire();
	void Server_Fire_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_PlayAnimMontage(UAnimMontage* AnimMontage);
	void Server_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAnimMontage(UAnimMontage* AnimMontage);
	void Multicast_PlayAnimMontage_Implementation(UAnimMontage* AnimMontage);

	void PlayerDead();
	UFUNCTION(Server, Reliable)
	void Server_PlayerDead();
	void Server_PlayerDead_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayerDead();
	void Multicast_PlayerDead_Implementation();

	void PlayerRespawn();
	UFUNCTION(Server, Reliable)
	void Server_PlayerRespawn();
	void Server_PlayerRespawn_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayerRespawn();
	void Multicast_PlayerRespawn_Implementation();

	virtual void DeliverDamage(float DamageAmount, AActor* DamageCauser) override;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	void ResetHealth();


};

