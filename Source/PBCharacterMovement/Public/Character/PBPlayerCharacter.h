// Copyright 2017-2018 Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "PBPlayerCharacter.generated.h"

class USoundCue;
class UPBMoveStepSound;
class UPBPlayerMovement;

inline float SimpleSpline(float Value)
{
	const float ValueSquared = Value * Value;
	return (3.0f * ValueSquared - 2.0f * ValueSquared * Value);
}

UCLASS(config = Game)
class PBCHARACTERMOVEMENT_API APBPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void ClearJumpInput(float DeltaTime) override;
	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void OnJumped_Implementation() override;
	virtual bool CanJumpInternal_Implementation() const override;

	virtual void RecalculateBaseEyeHeight() override;

	/* Triggered when player's movement mode has changed */
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCustomMode) override;

	[[nodiscard]] float GetLastJumpTime() const
	{
		return LastJumpTime;
	}

private:

	/** cached default eye height */
	float DefaultBaseEyeHeight{ 0.f };

	/** when we last jumped */
	float LastJumpTime{ 0.f };

	/** throttle jump boost when going up a ramp, so we don't spam it */
	float LastJumpBoostTime{ 0.f };

	/** maximum time it takes to jump */
	float MaxJumpTime{ 0.f };

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Camera")
	float BaseTurnRate{ 0.f };

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate.*/
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Camera")
	float BaseLookUpRate{ 0.f };

	/** Automatic bunnyhopping */
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Gameplay")
	bool bAutoBunnyhop{ false };

	/** Move step sounds by physical surface */
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Sounds")
	TMap<TEnumAsByte<EPhysicalSurface>, TSubclassOf<UPBMoveStepSound>> MoveStepSounds;

		/** Minimum speed to play the camera shake for landing */
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Damage")
	double MinLandBounceSpeed{ 0.f };

	/** Don't take damage below this speed - so jumping doesn't damage */
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Damage")
	double MinSpeedForFallDamage{ 0. };

	// In HL2, the player has the Z component for applying momentum to the capsule capped
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = "PB Player|Damage")
	double CapDamageMomentumZ{ 0. };

	/** Pointer to player movement component */
	TWeakObjectPtr<UPBPlayerMovement> MovementPtr;

	/** True if we're sprinting*/
	bool bIsSprinting{ false };

	bool bWantsToWalk{ false };

	/** defer the jump stop for a frame (for early jumps) */
	bool bDeferJumpStop{ false };

	virtual void ApplyDamageMomentum(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;
	
public:
	APBPlayerCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	bool IsSprinting() const
	{
		return bIsSprinting;
	}
	
	UFUNCTION()
	bool DoesWantToWalk() const
	{
		return bWantsToWalk;
	}
	
	[[nodiscard]] FORCEINLINE TSubclassOf<UPBMoveStepSound>* GetMoveStepSound(TEnumAsByte<EPhysicalSurface> Surface)
	{
		return MoveStepSounds.Find(Surface);
	}
	
	UFUNCTION(Category = "PB Getters", BlueprintPure)
	FORCEINLINE float GetBaseTurnRate() const
	{
		return BaseTurnRate;
	}

	UFUNCTION(Category = "PB Setters", BlueprintCallable)
	void SetBaseTurnRate(const float Val)
	{
		BaseTurnRate = Val;
	}
	
	UFUNCTION(Category = "PB Getters", BlueprintPure)
	FORCEINLINE float GetBaseLookUpRate() const
	{
		return BaseLookUpRate;
	}
	
	UFUNCTION(Category = "PB Setters", BlueprintCallable)
	void SetBaseLookUpRate(const float Val);

	UFUNCTION(Category = "PB Getters", BlueprintPure)
	FORCEINLINE bool GetAutoBunnyhop() const
	{
		return bAutoBunnyhop;
	}
	
	UFUNCTION(Category = "PB Setters", BlueprintCallable)
	void SetAutoBunnyhop(const bool bVal)
	{
		bAutoBunnyhop = bVal;
	}
	
	UFUNCTION(Category = "PB Getters", BlueprintPure)
	FORCEINLINE UPBPlayerMovement* GetMovementPtr() const
	{
		return MovementPtr.Get();
	}

	float GetDefaultBaseEyeHeight() const { return DefaultBaseEyeHeight; }

	UFUNCTION()
	void ToggleNoClip() const;

	UFUNCTION(Category = "Player Movement", BlueprintPure)
	double GetMinSpeedForFallDamage() const { return MinSpeedForFallDamage; };

	[[nodiscard]] double GetMinLandBounceSpeed() const { return MinLandBounceSpeed; }

	/** Handles strafing movement, left and right */
	void Move(const FVector& Direction, float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param bIsPure	If true, rate will pass through without delta applied to it
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired
	 * turn rate
	 */
	void Turn(bool bIsPure, float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param bIsPure	If true, rate will pass through without delta applied to it
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired
	 * turn rate
	 */
	void LookUp(bool bIsPure, float Rate);

	virtual bool CanCrouch() const override;
};
