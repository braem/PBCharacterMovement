// Copyright Project Borealis

#pragma once

#include "CoreMinimal.h"

#include "Engine/EngineTypes.h"

#include "PBMoveStepSound.generated.h"

class USoundCue;

UCLASS(Blueprintable)
class PBCHARACTERMOVEMENT_API UPBMoveStepSound : public UObject
{
	GENERATED_BODY()
	
public:
	TEnumAsByte<enum EPhysicalSurface> GetSurfaceMaterial() const { return SurfaceMaterial; }

	TArray<USoundCue*> GetStepLeftSounds() const { return StepLeftSounds; }

	TArray<USoundCue*> GetStepRightSounds() const { return StepRightSounds; }

	TArray<USoundCue*> GetSprintLeftSounds() const { return SprintLeftSounds; }

	TArray<USoundCue*> GetSprintRightSounds() const { return SprintRightSounds; }

	TArray<USoundCue*> GetJumpSounds() const { return JumpSounds; }

	TArray<USoundCue*> GetLandSounds() const { return LandSounds; }

	[[nodiscard]] float GetWalkVolume() const { return WalkVolume; }

	[[nodiscard]] float GetSprintVolume() const { return SprintVolume; }

private:
	/** The physical material associated with this move step sound */
	UPROPERTY(EditDefaultsOnly, Category = Material)
	TEnumAsByte<EPhysicalSurface> SurfaceMaterial;

	/** The list of sounds to randomly choose from when stepping left */
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	TArray<USoundCue*> StepLeftSounds;

	/** The list of sounds to randomly choose from when stepping right */
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	TArray<USoundCue*> StepRightSounds;

	/** The list of sounds to randomly choose from when sprinting left */
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	TArray<USoundCue*> SprintLeftSounds;

	/** The list of sounds to randomly choose from when sprinting right */
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	TArray<USoundCue*> SprintRightSounds;

	/** The list of sounds to randomly choose from when jumping */
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	TArray<USoundCue*> JumpSounds;

	/** The list of sounds to randomly choose from when landing */
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	TArray<USoundCue*> LandSounds;

	UPROPERTY(EditDefaultsOnly, Category = Volume)
	float WalkVolume{ 0.2f };

	UPROPERTY(EditDefaultsOnly, Category = Volume)
	float SprintVolume{ 0.5f };
};
