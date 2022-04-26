// Copyright (c) 2019-2021 Drowning Dragons Limited. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ModularGameplayActors/GSCModularCharacter.h"
#include "VITypes.h"
#include "Pawn/VIPawnInterface.h"
#include "VIModularCharacter.generated.h"

class UVIMotionWarpingComponent;
class UVIPawnVaultComponent;
class UVIAbilitySystemComponent;
class AController;

/**
 * Bridges GASCompanion Plugin with VaultIt
 */
UCLASS()
class VIGASCOMPANIONBRIDGE_API AVIModularCharacter : public AGSCModularCharacter, public IVIPawnInterface
{
	GENERATED_BODY()
	
public:
	/**
	 * Motion Warping Component used for vaulting
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	UVIMotionWarpingComponent* MotionWarping;

	/**
	 * Pawn Vault Component used for core vaulting logic
	 * 
	 * This is added in Blueprint and must be returned via
	 * the IVIPawnInterface::GetPawnVaultComponent function
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	UVIPawnVaultComponent* VaultComponent;

public:
	UPROPERTY(EditDefaultsOnly, Category = Vault)
	FVIAnimSet VaultAnimSet;

	UPROPERTY(EditDefaultsOnly, Category = Vault)
	FVITraceSettings VaultTraceSettings;

protected:
	/** Simulated proxies use this to update their vaulting state based on server values */
	UPROPERTY(Replicated, BlueprintReadWrite, Category = Vault)
	bool bRepIsVaulting;

	/** Used to detect changes in vaulting state and call StopVaultAbility() */
	UPROPERTY()
	bool bWasVaulting;

	/** 
	 * Simulated proxies use this to reproduce motion matching results provided
	 * by server in the GA_Vault gameplay ability
	 *
	 * Local players use this as a cache for FBIK testing (returned via GetVaultLocationAndDirection)
	 * 
	 * Net Serialized to one decimal point of precision
	 */
	UPROPERTY(ReplicatedUsing="OnRep_MotionMatch", BlueprintReadWrite, Category = Vault)
	FVIRepMotionMatch RepMotionMatch;

public:
	AVIModularCharacter(const FObjectInitializer& OI);

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_Controller() override;

	virtual void CheckJumpInput(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	virtual void Jump() override;
	virtual void StopJumping() override;

	/** Called from gameplay ability when vault stops */
	UFUNCTION(BlueprintCallable, Category = Vault)
	void StopVaultAbility();

	UFUNCTION(BlueprintImplementableEvent, Category = Vault)
	void OnStopVaultAbility();

protected:
	UFUNCTION()
	void OnRep_MotionMatch();

public:
	/**
	 * @return True if vaulting
	 * Correct value must be returned based on net role here
	 * Simulated proxies return bRepIsVaulting
	 * Server & Authority must return CMC bIsVaulting
	 */
	UFUNCTION(BlueprintPure, Category = Vault)
	virtual bool IsVaulting() const;

	// *********************************************** //
	// *********** Begin IVIPawnInterface ************ //
	// *********************************************** //

	// Read VIPawnInterface.h for detailed descriptions of these functions or look
	// inside their functions themselves

	virtual USkeletalMeshComponent* GetMeshForVaultMontage_Implementation() const override { return GetMesh(); }
	virtual FVector GetVaultDirection_Implementation() const override;
	virtual bool CanVault_Implementation() const override;
	virtual void StartVaultAbility_Implementation() override;
	virtual void OnLocalPlayerVault_Implementation(const FVector& Location, const FVector& Direction) override;
	virtual void GetVaultLocationAndDirection_Implementation(FVector& OutLocation, FVector& OutDirection) const override;
	virtual void ReplicateMotionMatch_Implementation(const FVIRepMotionMatch& MotionMatch) override;
	virtual bool IsWalkable_Implementation(const FHitResult& HitResult) const override;
	virtual bool CanAutoVaultInCustomMovementMode_Implementation() const override;

	virtual UVIPawnVaultComponent* GetPawnVaultComponent_Implementation() const override { return VaultComponent; }
	virtual UVIMotionWarpingComponent* GetMotionWarpingComponent_Implementation() const override { return MotionWarping; }

	virtual FVIAnimSet GetVaultAnimSet_Implementation() const override { return VaultAnimSet; }
	virtual FVITraceSettings GetVaultTraceSettings_Implementation() const override { return VaultTraceSettings; }

	// *********************************************** //
	// ************* End IVIPawnInterface ************ //
	// *********************************************** //
};
