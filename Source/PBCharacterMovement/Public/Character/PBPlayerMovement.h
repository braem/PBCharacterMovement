// Copyright 2017-2019 Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Runtime/Launch/Resources/Version.h"

#include "PBPlayerMovement.generated.h"

class APBPlayerCharacter;
class USoundCue;

UCLASS()
class PBCHARACTERMOVEMENT_API UPBPlayerMovement : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	/** If the player is using a ladder */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool bOnLadder{ false };

	/** Milliseconds between step sounds */
	float MoveSoundTime{ 0.f };

	/** If we are stepping left, else, right */
	bool StepSide{ false };

	/** The multiplier for acceleration when on ground. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	double GroundAccelerationMultiplier{ 0. };

	/** The multiplier for acceleration when in air. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	double AirAccelerationMultiplier{ 0. };
	
	/* The vector differential magnitude cap when in air. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Jumping / Falling")
	double AirSpeedCap{ 0. };

	/** Time to crouch on ground in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float CrouchTime{ 0.f };

	/** Time to uncrouch on ground in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float UncrouchTime{ 0.f };

	/** Time to crouch in air in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float CrouchJumpTime{ 0.f };

	/** Time to uncrouch in air in seconds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking")
	float UncrouchJumpTime{ 0.f };

	/** the minimum step height from moving fast */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite)
	double MinStepHeight{ 0. };

	/** Time (in millis) the player has to rejump without applying friction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Jumping / Falling", meta=(DisplayName="Rejump Window", ForceUnits="ms"))
	float BrakingWindow{ 0.f };

	/* Progress checked against the Braking Window, incremented in millis. */
	float BrakingWindowTimeElapsed{ 0.f };

	/** If the player has been on the ground past the Braking Window, start braking. */
	bool bBrakingWindowElapsed{ false };

	/** Wait a frame before crouch speed. */
	bool bCrouchFrameTolerated{ false };

	/** If in the crouching transition */
	bool bIsInCrouchTransition{ false };

	/** If in the crouching transition */
	bool bInCrouch{ false };

	/** The PB player character */
	TWeakObjectPtr<APBPlayerCharacter> PBCharacter;

	/** The target ground speed when running. */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	double RunSpeed{ 0. };

	/** The target ground speed when sprinting.  */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	double SprintSpeed{ 0. };

	/** The target ground speed when walking slowly. */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	double WalkSpeed{ 0. };

	/** Speed on a ladder */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Ladder")
	double LadderSpeed{ 0. };

	/** The minimum speed to scale up from for slope movement  */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	double SpeedMultMin{ 0. };

	/** The maximum speed to scale up to for slope movement */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	double SpeedMultMax{ 0. };

	/** The maximum angle we can roll for camera adjust */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (General Settings)")
	double RollAngle{ 0. };

	/** Speed of rolling the camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (General Settings)")
	double RollSpeed{ 0. };

	/** Speed of rolling the camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (General Settings)")
	double BounceMultiplier{ 0. };

	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	double AxisSpeedLimit{ 6667.5 };

	/** Threshold relating to speed ratio and friction which causes us to catch air */
	UPROPERTY(Category = "Character Movement: Walking", EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0"))
	float SlideLimit{ 0.5f };

	/** Fraction of uncrouch half-height to check for before doing starting an uncrouch. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (General Settings)")
	float GroundUncrouchCheckFactor{ 0.75f };

	bool bShouldPlayMoveSounds{ true };

public:
	/** Print pos and vel (Source: cl_showpos) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement (General Settings)")
	uint8 bShowPos : 1 { false };

	UPBPlayerMovement();

	virtual void InitializeComponent() override;
	virtual void OnRegister() override;

	// Overrides for Source-like movement
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void CalcVelocity(float DeltaTime, float Friction, bool bFluid, float BrakingDeceleration) override;
	virtual void ApplyVelocityBraking(float DeltaTime, float Friction, float BrakingDeceleration) override;
	virtual void PhysFalling(float DeltaTime, int32 Iterations) override;
	virtual bool ShouldLimitAirControl(float DeltaTime, const FVector& FallAcceleration) const override;
	virtual FVector NewFallVelocity(const FVector& InitialVelocity, const FVector& Gravity, float DeltaTime) const override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;

	void UpdateSurfaceFriction(bool bIsSliding = false);
	// Crouch transition but not in noclip
	void UpdateCrouching(float DeltaTime, bool bOnlyUnCrouch = false);

	// Overrides for crouch transitions
	virtual void Crouch(bool bClientSimulation = false) override;
	virtual void UnCrouch(bool bClientSimulation = false) override;
	virtual void DoCrouchResize(float TargetTime, float DeltaTime, bool bClientSimulation = false);
	virtual void DoUnCrouchResize(float TargetTime, float DeltaTime, bool bClientSimulation = false);

	virtual bool MoveUpdatedComponentImpl(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit = nullptr, ETeleportType Teleport = ETeleportType::None) override;

	// Jump overrides
	virtual bool CanAttemptJump() const override;
	virtual bool DoJump(bool bClientSimulation) override;

	virtual void TwoWallAdjust(FVector& OutDelta, const FHitResult& Hit, const FVector& OldHitNormal) const override;
	virtual float SlideAlongSurface(const FVector& Delta, float Time, const FVector& Normal, FHitResult& Hit, bool bHandleImpact = false) override;
	virtual FVector ComputeSlideVector(const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const override;
	virtual FVector HandleSlopeBoosting(const FVector& SlideResult, const FVector& Delta, const float Time, const FVector& Normal, const FHitResult& Hit) const override;
	virtual bool ShouldCatchAir(const FFindFloorResult& OldFloor, const FFindFloorResult& NewFloor) override;
	virtual bool IsWithinEdgeTolerance(const FVector& CapsuleLocation, const FVector& TestImpactPoint, const float CapsuleRadius) const override;
	virtual bool IsValidLandingSpot(const FVector& CapsuleLocation, const FHitResult& Hit) const override;
	virtual bool ShouldCheckForValidLandingSpot(float DeltaTime, const FVector& Delta, const FHitResult& Hit) const override;

	void TraceCharacterFloor(FHitResult& OutHit) const;

	// Acceleration
	FORCEINLINE FVector GetAcceleration() const
	{
		return Acceleration;
	}

	/** Is this player on a ladder? */
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	bool IsOnLadder() const
	{
		return bOnLadder;
	}

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	/** Do camera roll effect based on velocity */
	double GetCameraRoll() const;

	void SetNoClip(bool bNoClip);

	/** Toggle no clip */
	void ToggleNoClip();

	[[nodiscard]] bool IsBrakingWindowTolerated() const
	{
		return bBrakingWindowElapsed;
	}

	[[nodiscard]] bool IsInCrouch() const
	{
		return bInCrouch;
	}

	virtual float GetMaxSpeed() const override;

private:
	/** Plays sound effect according to movement and surface */
	void PlayMoveSound(float DeltaTime);

	class UPBMoveStepSound* GetMoveStepSoundBySurface(EPhysicalSurface SurfaceType) const;

	virtual void PlayJumpSound(const FHitResult& Hit, bool bJumped);

	double DefaultStepHeight{ 0. };
	double DefaultWalkableFloorZ{ 0. };
	float SurfaceFriction{ 0.f };

	/** The time that the player can remount on the ladder */
	float OffLadderTicks{ -1.0f };

	bool bHasDeferredMovementMode{ false };
	EMovementMode DeferredMovementMode{ MOVE_None };
};
