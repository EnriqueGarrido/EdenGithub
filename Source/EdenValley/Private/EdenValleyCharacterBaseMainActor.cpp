// Fill out your copyright notice in the Description page of Project Settings.


#include "EdenValleyCharacterBaseMainActor.h"
#include "EngineMinimal.h"
#include "EdenValleyPlayerController.h"
#include "Interaction/Interactable.h"
#include "Interaction/AutoPickup.h"

AEdenValleyCharacterBaseMainActor::AEdenValleyCharacterBaseMainActor()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement.
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate.
	GetCharacterMovement()->JumpZVelocity = 425.0f;
	GetCharacterMovement()->AirControl = 0.02f;

	// Create a camera boom (pulls in towards the player if there is a collision).
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character.	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller.

	// Create a follow camera.
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation.
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm.

	// Create the collection sphere.
	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(200.f);

	bBlockedMovementInput = false;
}

void AEdenValleyCharacterBaseMainActor::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !bBlockedMovementInput)
	{
		// Find out which way is forward.
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Add movement in that direction.
		AddMovementInput(Direction, Value);

		// Fire move forward event.
		OnMoveForward(Value);
	}
}

void AEdenValleyCharacterBaseMainActor::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !bBlockedMovementInput)
	{
		// Find out which way is right.
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get right vector. 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement in that direction.
		AddMovementInput(Direction, Value);

		// Fire move right event.
		OnMoveRight(Value);
	}
}

void AEdenValleyCharacterBaseMainActor::StartJump()
{
	if (!bBlockedMovementInput)
	{
		bPressedJump = true;
	}
}

void AEdenValleyCharacterBaseMainActor::StopJump()
{
	bPressedJump = false;
	StopJumping();
}

// Called to bind functionality to input
void AEdenValleyCharacterBaseMainActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AEdenValleyCharacterBaseMainActor::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AEdenValleyCharacterBaseMainActor::StopJump);

	PlayerInputComponent->BindAxis("MoveForward", this, &AEdenValleyCharacterBaseMainActor::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AEdenValleyCharacterBaseMainActor::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AEdenValleyCharacterBaseMainActor::CheckForInteractables()
{
	// Create a LineTrace to check for a hit
	FHitResult HitResult;

	int32 Range = 500; // Specified range to interact with an object
	FVector StartTrace = FollowCamera->GetComponentLocation();
	FVector EndTrace = (FollowCamera->GetForwardVector() * Range) + StartTrace;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	AEdenValleyPlayerController* PlayerController = Cast<AEdenValleyPlayerController>(GetController());
	if (PlayerController)
	{
		// Check if something is hit
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, QueryParams))
		{
			// Cast the actor to AInteractable
			AInteractable* Interactable = Cast<AInteractable>(HitResult.GetActor());
			// If the cast is successful
			if (Interactable)
			{
				PlayerController->CurrentInteractable = Interactable;
				return;
			}
		}

		PlayerController->CurrentInteractable = nullptr;
	}
}

void AEdenValleyCharacterBaseMainActor::CollectAutoPickup()
{
	// Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	AEdenValleyPlayerController* PlayerController = Cast<AEdenValleyPlayerController>(GetController());

	// For each collected Actor
	for (int32 Collected = 0; Collected < CollectedActors.Num(); ++Collected)
	{
		// Cast the actor to AAutoPickup
		AAutoPickup* const TestPickup = Cast<AAutoPickup>(CollectedActors[Collected]);

		// If the cast is successful and the pickup is valid and active 
		if (TestPickup && !TestPickup->IsPendingKill())
		{
			TestPickup->Collect(PlayerController);
		}
	}
}

// Called every frame
void AEdenValleyCharacterBaseMainActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CollectAutoPickup();
	CheckForInteractables();
}