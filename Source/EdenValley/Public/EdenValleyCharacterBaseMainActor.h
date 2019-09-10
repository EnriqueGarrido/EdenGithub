// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdenValleyCharacterBase.h"
#include "EdenValleyCharacterBaseMainActor.generated.h"

/** Derived class from Character. */
UCLASS(config = Game)
class EDENVALLEY_API AEdenValleyCharacterBaseMainActor : public AEdenValleyCharacterBase
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Collection sphere. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollectionSphere;

public:
	/** Constructor. */
	AEdenValleyCharacterBaseMainActor();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/** To kwno if this character can be moved through input. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bBlockedMovementInput;

protected:
	/** Called for forwards/backward input. */
	void MoveForward(float Value);

	/** Called for side to side input. */
	void MoveRight(float Value);

	/** Called when input jump is pressed. */
	void StartJump();

	/** Called when input jump is released. */
	void StopJump();

	/** Callede when the character moves forward. */
	UFUNCTION(BlueprintImplementableEvent, Category = "MainActor", Meta = (DisplayName = "OnMoveForward"))
	void OnMoveForward(float Value);

	/** Called when the character moves right. */
	UFUNCTION(BlueprintImplementableEvent, Category = "MainActor", Meta = (DisplayName = "OnMoveRight"))
	void OnMoveRight(float Value);

	/** Implement APawn interface. */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/** Function to check for the closest Interactable in sight and in range. */
	void CheckForInteractables();

	/** Function to collect every AutoPickup in range. */
	void CollectAutoPickup();

public:
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	/** Returns CameraBoom subobject. */
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject. */
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
